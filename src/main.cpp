#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

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

	// little ray tracer
	std::vector<std::unique_ptr<Object>> spheres;
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( -1e5+1,40.8,81.6 ), sf::Vector3f(.75f,.25f,.25f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 1e5+99,40.8,81.6 ), sf::Vector3f(.25f,.25f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8, -1e5 ),    sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8,1e5+200 ),  sf::Vector3f(0.f,0.f,0.f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50, -1e5, 81.6 ),   sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,1e5+81.6,81.6 ), sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 27,16.5,47 ),       sf::Vector3f(1,1,1)*.999f ) );
	spheres.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 73,16.5,78 ),       sf::Vector3f(1,1,1)*.999f ) );
	spheres.emplace_back( make_unique<Sphere>( 600.f, sf::Vector3f(50,681.6-.27,81.6),  sf::Vector3f(1,1,1) ) );

	sf::Image img;
	img.create( 800, 600, sf::Color::Black );

	for ( int x = 0; x < 800; x++ ){
		for ( int y = 0 ; y < 600; y++ ){
			// camera
			float xpart = ( static_cast<float>(x - 800 / 2) ) / 800.f * 0.9f;
			float ypart = ( static_cast<float>(y - 600 / 2) ) / 800.f * 0.9f;

			Ray ray( sf::Vector3f( 50.f, 45.f, 295.f ), thor::unitVector( sf::Vector3f(xpart, ypart, -1.f) ) );

			// find collision
			float t = 1e20;
			size_t id = 0;
			for( unsigned int i = 0; i < spheres.size(); ++i ){
				boost::optional<float> tmp = spheres[i]->intersect( ray );
				if( tmp && *tmp < t && *tmp > 0.f ){
					t = *tmp;
					id = i;
				}
			}

			sf::Vector3i col( clamp( spheres[id]->getColor() ) * 255.f );

			img.setPixel( x, 600 - y - 1, sf::Color( col.x, col.y, col.z) );
		}
	}

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );

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
