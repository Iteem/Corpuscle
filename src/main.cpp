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

std::vector<std::unique_ptr<Object>> spheres;
std::mt19937 gen;

const sf::Vector2i screen( 800, 600 );

sf::Vector3f radiance( Ray ray, int depth, unsigned int prevId )
{
	// find collision
	float t = inf;
	size_t id = 0;
	for( unsigned int i = 0; i < spheres.size(); ++i ){
		float tmp = spheres[i]->intersect( ray );
		if( tmp < t && tmp > 0.f && i != prevId ){
			t = tmp;
			id = i;
		}
	}

	// no collision, return black
	if( t == inf ){
		return sf::Vector3f();
	}

	// maximal depth, return emission only
	if( --depth < 0 ){
		return spheres[id]->getEmission();
	}

	// calculate new ray
	sf::Vector3f normal( spheres[id]->collisionNormal( ray ) );

	ray.origin = ray.evaluate( t );

	sf::Vector3f randomVec( uniformOnSphere(gen) );
	float d = thor::dotProduct( randomVec, normal);
	ray.direction = d > 0.f ? randomVec : randomVec - 2.f * d * normal;


	return spheres[id]->getEmission() + thor::cwiseProduct( spheres[id]->getColor(), radiance( ray, depth - 1, id ) );
}

int main()
{
	sf::RenderWindow window( sf::VideoMode(screen.x, screen.y), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	gen.seed( std::time( nullptr ) );

	// initialize scene
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( -1e5+1,40.8,81.6 ), sf::Vector3f(.75f,.25f,.25f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 1e5+99,40.8,81.6 ), sf::Vector3f(.25f,.25f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8, -1e5 ),    sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8,1e5+200 ),  sf::Vector3f(0.f,0.f,0.f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50, -1e5, 81.6 ),   sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,1e5+81.6,81.6 ), sf::Vector3f(.75f,.75f,.75f) ) );
	spheres.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 27,16.5,47 ),       sf::Vector3f(1,1,1)*.999f ) );
	spheres.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 73,16.5,78 ),       sf::Vector3f(1,1,1)*.999f ) );
	spheres.emplace_back( make_unique<Sphere>( 600.f, sf::Vector3f(50,681.6-.27,81.6),  sf::Vector3f(1,1,1), sf::Vector3f(1,1,1)*12.f ) );

	std::vector< sf::Vector3f > pixels( screen.x*screen.y );
	unsigned int samples = 0;

	sf::Image img;
	img.create( screen.x, screen.y, sf::Color::Black );

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

		samples++;
		for ( int x = 0; x < screen.x; x++ ){
			for ( int y = 0 ; y < screen.y; y++ ){
				// camera
				float xpart = ( static_cast<float>(x - screen.x / 2) ) / screen.x * 0.9f;
				float ypart = ( static_cast<float>(y - screen.y / 2) ) / screen.x * 0.9f;

				Ray ray( sf::Vector3f( 50.f, 45.f, 295.f ), thor::unitVector( sf::Vector3f(xpart, ypart, -1.f) ) );

				pixels[x + screen.x * y] += radiance( ray, 5, -1 );
				sf::Vector3i col( gammmaCorrected( clamp( pixels[x + screen.x * y] / static_cast<float>( samples ) ), 1.f/2.2f ) * 255.f );
				img.setPixel( x, screen.y - y - 1, sf::Color( col.x, col.y, col.z) );
			}
		}
		window.setTitle( "Corpuscle - SPP: " + std::to_string( samples ) );
		tex.loadFromImage( img );

		window.clear();
		window.draw( sprite );
		window.display();
	}
}
