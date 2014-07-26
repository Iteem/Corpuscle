#include <iostream>

#include <SFML/Graphics.hpp>

#include "rendermanager.hpp"

const sf::Vector2u screenSize( 800, 600 );

int main()
{
	// Window.
	sf::RenderWindow window( sf::VideoMode(screenSize.x, screenSize.y), "Corpuscle" );
	window.setVerticalSyncEnabled( true );


	// Set up sprite to display.
	sf::Image img;
	img.create( screenSize.x, screenSize.y, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );

	// Set up RenderManager and start the rendering process.
	RenderManager rm( screenSize );
	rm.loadSceneFromFile( "data/scene.json" );
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
				img.saveToFile(  "corpuscle-" + std::to_string( rm.getSamples() ) + "SPP.png" );
			}
		}

		if( prevSamples != samples ){
			// Only update the image every 10th sample.
			if( samples % 10 == 0 ){
				prevSamples = samples;

				// Update Image.
				img = rm.getImage();
				tex.loadFromImage( img );

				rm.setUpdateImage( false );
			}
			// Don't forget to request a image update before.
			else if( samples % 9 == 0 ){
				rm.setUpdateImage( true );
			}
		}

		window.setTitle( "Corpuscle - SPP: " + std::to_string( samples ) );

		// Drawing.
		window.clear();
		window.draw( sprite );
		window.display();
	}
}
