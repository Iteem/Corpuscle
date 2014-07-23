#include <iostream>

#include <SFML/Graphics.hpp>

#include "renderer.hpp"
#include "scene.hpp"

const sf::Vector2u screen( 800, 600 );

int main()
{
	// Window.
	sf::RenderWindow window( sf::VideoMode(screen.x, screen.y), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	// Scene and Renderer.
	Scene scene;
	scene.loadFromJSON( "data/scene.json" );

	Renderer renderer( &scene, screen );

	// Set up sprite to display.
	sf::Image img;
	img.create( screen.x, screen.y, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );

	// Main loop.
	while( window.isOpen() ){
		// Event loop.
		sf::Event event;
		while( window.pollEvent( event ) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}
		}

		// Render one more sample.
		renderer.render();

		// Update Image.
		img = renderer.getImage();

		window.setTitle( "Corpuscle - SPP: " + std::to_string( renderer.getSPP() ) );
		tex.loadFromImage( img );

		// Drawing.
		window.clear();
		window.draw( sprite );
		window.display();
	}
}
