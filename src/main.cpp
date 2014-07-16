#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>

#include "cube.hpp"
#include "random.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "utility.hpp"

Scene scene;
std::mt19937 gen;

const sf::Vector2i screen( 800, 600 );

sf::Vector3f specularReflection( const sf::Vector3f &direction, const sf::Vector3f &normal )
{
	return direction - 2.f * thor::dotProduct( direction, normal ) * normal;
}

sf::Vector3f diffuseReflection( const sf::Vector3f &normal )
{
	sf::Vector3f randomVec( uniformOnSphere(gen) );
	float d = thor::dotProduct( randomVec, normal);
	return d > 0.f ? randomVec : randomVec - 2.f * d * normal;
}

sf::Vector3f radiance( Ray ray, int depth, const Object *prevObject = nullptr )
{
	auto collisionPair = scene.getCollision( ray, prevObject );
	float t = collisionPair.first;
	const Object *obj = collisionPair.second;

	// no collision, return black
	if( t == inf ){
		return sf::Vector3f();
	}

	// maximal depth, return emission only
	if( --depth < 0 ){
		return obj->getEmission();
	}

	// calculate new ray
	sf::Vector3f normal( obj->collisionNormal( ray ) );

	ray.origin = ray.evaluate( t );

	switch( obj->getMaterial().type ){
		case Material::Type::Diffuse:
			ray.direction = diffuseReflection( normal );
			break;
		case Material::Type::Specular:
			ray.direction = specularReflection( ray.direction, normal );
			break;
		case Material::Type::Glossy:
			ray.direction = thor::unitVector( (1 - obj->getMaterial().factor) * diffuseReflection( normal ) + obj->getMaterial().factor * specularReflection( ray.direction, normal ) );
	}


	return obj->getEmission() + thor::cwiseProduct( obj->getColor(), radiance( ray, depth - 1, obj ) );
}

int main()
{
	sf::RenderWindow window( sf::VideoMode(screen.x, screen.y), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	gen.seed( std::time( nullptr ) );
	std::uniform_real_distribution<float> dist( -1.f, 1.f );

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

		// the actual path tracer
		for( int i = 0; i < 5; ++i ){
			samples++;
			for ( int x = 0; x < screen.x; x++ ){
				for ( int y = 0 ; y < screen.y; y++ ){
					// camera
					float xpart = ( static_cast<float>( x - screen.x / 2 ) + dist( gen ) ) / screen.x * 0.9f;
					float ypart = ( static_cast<float>( y - screen.y / 2 ) + dist( gen ) ) / screen.x * 0.9f;

					Ray ray( sf::Vector3f( 50.f, 45.f, 295.f ), thor::unitVector( sf::Vector3f(xpart, ypart, -1.f) ) );

					pixels[x + screen.x * y] += radiance( ray, 5 );
				}
			}
		}

		// load data into the sf::image
		for ( int x = 0; x < screen.x; x++ ){
			for ( int y = 0 ; y < screen.y; y++ ){
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
