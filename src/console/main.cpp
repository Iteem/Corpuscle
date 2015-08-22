#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include "renderer/rendermanager.hpp"

const unsigned int numThreads = 4;
const unsigned int numSamplesSave = 100;
const unsigned int maxSamples = 100000; // Probably a bit much.

int main(int argc, char **argv)
{
	if( argc < 2 ){
		std::cerr << "Too few arguments, you need to specify a scene file." << std::endl;
		return 1;
	}

	// Load scene.
	Scene scene;
	if( !scene.loadFromJSON( std::string( argv[1] ) ) ){
		std::cout << "Failed to load scene file " + std::string( argv[1] ) << "." << std::endl;
		return 1;
	}

	// Set up RenderManager.
	RenderManager rm( scene, numThreads );
	rm.setUpdateImage( true );

	// Clock for measuring the frame time
	sf::Clock renderPassClock;
	unsigned int prevSamples = 0;

	// Do the thing.
	rm.startRendering();

	// Main loop.
	while( prevSamples < maxSamples ){
		sf::sleep( sf::milliseconds( 100 ) ); // Check every 0.1 seconds, no need to update more often.

		unsigned int samples = rm.getSamples();

		if( prevSamples != samples ){
			sf::Vector2u renderSize( rm.getResolution().x, rm.getResolution().y );
			int sps = std::floor( renderSize.x * renderSize.y / renderPassClock.restart().asSeconds() );

			std::cout << "\r" << std::setw(6) << samples << " samples;" << std::setw(9) << sps << " SPS   " << std::flush;

			if( samples % numSamplesSave == 0 ) {
				sf::Image image( rm.getImage() );
				std::string fileName =
						"corpuscle-" + std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
						"-" + std::to_string( samples ) + "SPP.png";

				image.saveToFile( fileName );
				std::cout << std::endl << "Saved image to file '" << fileName << "'" << std::endl;
			}

			prevSamples = samples;
		}
	}
}
