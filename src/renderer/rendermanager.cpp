#include "rendermanager.hpp"

#include <functional>
#include <iostream>

#include "renderer.hpp"
#include "utility.hpp"

RenderManager::RenderManager( Scene& scene, unsigned int numThreads ) :
	m_scene( scene ),
	m_samples( 0 ),
	m_updateImage( true ),
	m_currentLine( 0 ),
	m_numRunningThreads( 0 ),
	m_isRunning( false ),
	m_numThreads( numThreads )
{
	reset();
}

RenderManager::~RenderManager()
{
	stopRendering();
}

void RenderManager::setUpdateImage( bool updateImage )
{
	m_updateImage = updateImage;
}

bool RenderManager::getUpdateImage() const
{
	return m_updateImage;
}

glm::uvec2 RenderManager::getResolution() const
{
	return m_scene.getCamera().getResolution();
}

void RenderManager::setResolution( glm::uvec2 resolution )
{
	stopRendering();

	Camera camera( m_scene.getCamera() );
	camera.setResolution( resolution );
	m_scene.setCamera( camera );

	reset();
}

void RenderManager::startRendering()
{
	m_isRunning = true;

	for( unsigned int i = 0; i < m_numThreads; ++i ){
		m_threads.emplace_back( std::bind( &RenderManager::workerFunction, this ) );
	}
}

void RenderManager::stopRendering()
{
	m_isRunning = false;
	for( auto& thread : m_threads ){
		thread.join();
	}
}

bool RenderManager::isRendering() const
{
	return m_isRunning;
}

unsigned int RenderManager::getSamples() const
{
	return m_samples;
}

sf::Image RenderManager::getImage() const
{
	std::lock_guard<std::mutex> lock( m_imageMutex );

	return m_image;
}

void RenderManager::reset()
{
	stopRendering();

	auto resolution = m_scene.getCamera().getResolution();

	// Reset pixel data and make sure not to waste any memory.
	m_pixels.resize( resolution.x * resolution.y, glm::vec3() );
	m_pixels.shrink_to_fit();

	m_samples = 0;
}

void RenderManager::workerFunction()
{
	++m_numRunningThreads;

	auto resolution = m_scene.getCamera().getResolution();

	// Set up renderer
	Renderer renderer( &m_scene, resolution );

	int line;
	while( true ){
		line = getNextJob();

		// Check if we should stop the thread.
		if( line == -1 )
			return;

		renderer.render( m_pixels, glm::ivec2( 0, line ), glm::ivec2( resolution.x, 1 ) );
	}

}

int RenderManager::getNextJob()
{
	auto resolution = m_scene.getCamera().getResolution();

	int ret = -1;
	do{
		ret = m_currentLine++;

		if( ret >= static_cast<int>( resolution.y ) ){
			// Nothing to do here, wait for the others.
			unsigned int numRunningThreads = --m_numRunningThreads;

			// First check if we are the last thread.
			if( numRunningThreads == 0 ){
				// Don't let any other thread interfere before we're done setting up the next round.
				std::unique_lock<std::mutex> lock( m_mutex );

				++m_samples;
				m_currentLine = 0;

				// Save the progress to the image if requested.
				if( m_updateImage ){
					std::lock_guard<std::mutex> lock( m_imageMutex );

					if( m_image.getSize().x != resolution.x && m_image.getSize().y != resolution.y ){
						m_image.create( resolution.x, resolution.y );
					}

					for ( unsigned int x = 0; x < resolution.x; x++ ){
						for ( unsigned int y = 0 ; y < resolution.y; y++ ){
							glm::ivec3 col( gammmaCorrected( clamp( m_pixels[x + resolution.x * y] / static_cast<float>( m_samples ) ), 1.f/2.2f ) * 255.f );
							m_image.setPixel( x, resolution.y - y - 1, sf::Color( col.x, col.y, col.z) );
						}
					}
				}


				// Make sure the party doesn't start without us.
				++m_numRunningThreads;

				lock.unlock();

				// Tell everyone to go back to work.
				m_conVar.notify_all();
			}
			else {
				// Wait for more work.
				std::unique_lock<std::mutex> lock( m_mutex );
				m_conVar.wait( lock, [this, resolution](){ return m_currentLine < resolution.y; } );

				// We're back working.
				++m_numRunningThreads;
			}

		}
	} while( ret >= static_cast<int>( resolution.y ) );

	// Check if we need to stop rendering.
	if( !m_isRunning )
		return -1;

	return ret;
}
