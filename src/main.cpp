#include <iostream>

#include <SFML/Graphics.hpp>

#include "rendermanager.hpp"

const sf::Vector2u  r360p(   640,   360 );
const sf::Vector2u  r480p(   854,   480 );
const sf::Vector2u  r720p(  1280,   720 );
const sf::Vector2u r1080p(  1920,  1080 );
const sf::Vector2u r1440p(  2560,  1440 );
const sf::Vector2u    r4K(  3840,  2160 );
const sf::Vector2u    r8K(  7680,  4320 );
const sf::Vector2u   r16K( 15360,  8640 );

const sf::Vector2u screenSize( sf::Vector2u( 1920 / 2, 1080 / 4 ) );
const sf::Vector2u renderSize( sf::Vector2u( 1920 * 2, 1080 ) );
const int sampleDisplayInterval = 10;
const unsigned int numThreads = 4;

int main()
{
	// Window.
	sf::RenderWindow window( sf::VideoMode(screenSize.x, screenSize.y), "Corpuscle" );
	window.setVerticalSyncEnabled( true );


	// Set up sprite to display.
	sf::Image img;
	img.create( renderSize.x, renderSize.y, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );
	sprite.scale(
		static_cast<float>(screenSize.x)/static_cast<float>(renderSize.x),
		static_cast<float>(screenSize.y)/static_cast<float>(renderSize.y)
	);

	// Set up RenderManager and start the rendering process.
	RenderManager rm( renderSize , numThreads );
	rm.loadSceneFromFile( "data/background.json" );
	rm.setUpdateImage( false );
	rm.startRendering();

	unsigned int prevSamples = 0;

	// Main loop.
	while( window.isOpen() ){
		unsigned int samples = rm.getSamples();

		// Event loop.
		sf::Event event;
		while( window.pollEvent( event ) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}
			else if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2 ){
				img.saveToFile(
					"corpuscle-"+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
					"-" + std::to_string( rm.getSamples() ) + "SPP.png"
				);
			}
		}

		if( prevSamples != samples ){
			// Only update the image every Nth sample.
			if( samples % sampleDisplayInterval == 0 ){
				prevSamples = samples;

				// Update Image.
				img = rm.getImage();
				tex.loadFromImage( img );

				rm.setUpdateImage( false );
			}
			// Don't forget to request a image update before.
			else if( samples % (sampleDisplayInterval-1) == 0 ){
				rm.setUpdateImage( true );
			}
		}

		window.setTitle(
			"Corpuscle @ "+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
				" & " + std::to_string(numThreads) + " Threads - SPP: " + std::to_string( samples )
		);

		// Drawing.
		window.clear();
		window.draw( sprite );
		window.display();
	}
}
