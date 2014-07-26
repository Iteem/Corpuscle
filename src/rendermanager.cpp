#include "rendermanager.hpp"

#include <functional>
#include <iostream>

#include "utility.hpp"
#include "renderer.hpp"

RenderManager::RenderManager( sf::Vector2u dimension ) :
	m_dimension( dimension ),
	m_samples( 0 ),
	m_updateImage( true ),
	m_currentLine( 0 ),
	m_numRunningThreads( 0 ),
	m_isRunning( false )
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

sf::Vector2u RenderManager::getDimension() const
{
	return m_dimension;
}

void RenderManager::setDimension( sf::Vector2u dimension )
{
	stopRendering();

	m_dimension = dimension;
	reset();
}

bool RenderManager::loadSceneFromFile( const std::string& path )
{
	// Reset the everything before loading a new scene.
	reset();

	return m_scene.loadFromJSON( path );
}

void RenderManager::startRendering()
{
	m_isRunning = true;

	for( unsigned int i = 0; i < 4; ++i ){
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

	// Reset pixel data and make sure not to waste any memory.
	m_pixels.resize( m_dimension.x * m_dimension.y, sf::Vector3f() );
	m_pixels.shrink_to_fit();

	m_samples = 0;
}

void RenderManager::workerFunction()
{
	m_numRunningThreads++;

	// Set up renderer
	Renderer renderer( &m_scene, m_dimension );

	int line;
	while( true ){
		line = getNextJob();

		// Check if we should stop the thread.
		if( line == -1 )
			return;

		sf::IntRect rect( 0, line, m_dimension.x, 1);
		renderer.render( m_pixels, rect );
	}

}

int RenderManager::getNextJob()
{
	int ret = -1;
	do{
		ret = m_currentLine++;

		if( ret >= static_cast<int>( m_dimension.y ) ){
			// Nothing to do here, wait for the others.
			// But first check if we are the last thread.
			unsigned int numRunningThreads = --m_numRunningThreads;

			if( numRunningThreads == 0 ){
				// Save the progress to the image if requested.
				if( m_updateImage ){
					std::lock_guard<std::mutex> lock( m_imageMutex );

					if( m_image.getSize() != m_dimension ){
						m_image.create( m_dimension.x, m_dimension.y );
					}

					for ( unsigned int x = 0; x < m_dimension.x; x++ ){
						for ( unsigned int y = 0 ; y < m_dimension.y; y++ ){
							sf::Vector3i col( gammmaCorrected( clamp( m_pixels[x + m_dimension.x * y] / static_cast<float>( m_samples ) ), 1.f/2.2f ) * 255.f );
							m_image.setPixel( x, m_dimension.y - y - 1, sf::Color( col.x, col.y, col.z) );
						}
					}
				}

				m_currentLine = 0;
				m_samples++;

				// Tell everyone to go back to work.
				m_conVar.notify_all();
			} else {
				// Wait for more work.
				std::unique_lock<std::mutex> lock( m_mutex );
				m_conVar.wait( lock );
			}

			// We're back working
			m_numRunningThreads++;
		}
	} while( ret >= static_cast<int>( m_dimension.y) );

	// Check if we need to stop rendering.
	if( !m_isRunning )
		return -1;

	return ret;
}
