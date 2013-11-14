#include <ctime>
#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>

#include "random.hpp"
#include "sphere.hpp"
#include "utility.hpp"

int main()
{
	sf::RenderWindow window( sf::VideoMode(800, 600), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	std::mt19937 gen( std::time( nullptr ) );

	// test uniformOnSphere
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

	// test Sphere::intersect
	Sphere s( 0.5f, sf::Vector3f(1, 1, 1) );
	Ray r1( sf::Vector3f(), sf::Vector3f( 1, 0, 0 ) );
	Ray r2( sf::Vector3f(), thor::unitVector( sf::Vector3f( 1, 1, 1) ) );

	std::cout << "Ray one " << (s.intersect(r1) ? "does" : "doesn't") << " intersect with the Sphere!" << std::endl;
	std::cout << "Ray two " << (s.intersect(r2) ? "does" : "doesn't") << " intersect with the Sphere!" << std::endl;

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
