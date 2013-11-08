#include <ctime>
#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>

#include "random.hpp"

int main()
{
	sf::RenderWindow window( sf::VideoMode(800, 600), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	std::mt19937 gen( std::time( nullptr ) );

	sf::Image img;
	img.create( 100, 100, sf::Color::Black );

	for( int i = 0; i < 5000; ++i ){
		sf::Vector3f vec = uniformOnSphere( gen );
		img.setPixel( 50 + static_cast<int>( 49 * vec.x),
					  50 + static_cast<int>( 49 * vec.y),
					  sf::Color::White );
	}

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );
	sprite.setScale(4, 4);

	// main loop
	while( window.isOpen() ){
		// event loop
		sf::Event event;
		while( window.pollEvent( event ) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}
		}

		window.clear();

		window.draw( sprite );

		window.display();
	}
}
