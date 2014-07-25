#include "cuboid.hpp"

#include <algorithm>
#include <cmath>

#include <Thor/Vectors/VectorAlgebra3D.hpp>

#include "utility.hpp"

Cuboid::Cuboid( sf::Vector3f min, sf::Vector3f max, sf::Vector3f color, sf::Vector3f emission, Material material ) :
	Object( color, emission, material ),
	min( min ),
	max( max )
{
}

Cuboid::~Cuboid()
{
}


float Cuboid::intersect( const Ray& ray ) const
{
	sf::Vector3f invdir( 1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z );

	auto t = std::make_pair<float, float>( ( min.x - ray.origin.x) * invdir.x, ( max.x - ray.origin.x) * invdir.x );
	if( invdir.x < 0.f )
		std::swap( t.first, t.second );

	auto ty = std::make_pair<float, float>( ( min.y - ray.origin.y) * invdir.y, ( max.y - ray.origin.y) * invdir.y );
	if( invdir.y < 0.f )
		std::swap( ty.first, ty.second );

	if ((t.first > ty.second) || (ty.first > t.second))
		return inf;

	if (ty.first > t.first)
		t.first = ty.first;
	if (ty.second < t.second)
		t.second = ty.second;

	auto tz = std::make_pair<float, float>( ( min.z - ray.origin.z) * invdir.z, ( max.z - ray.origin.z) * invdir.z );
	if( invdir.z < 0.f )
		std::swap( tz.first, tz.second );

	if ((t.first > tz.second) || (tz.first > t.second))
		return inf;

	if (tz.first > t.first)
		t.first = tz.first;
	if (tz.second < t.second)
		t.second = tz.second;

	return t.first;
}


sf::Vector3f Cuboid::collisionNormal( const Ray& ray ) const
{
	sf::Vector3f p( ray.evaluate( intersect( ray ) ) );

	sf::Vector3f normal( -1.f, 0.f, 0.f );
	float d = std::abs( min.x - p.x );

	float tmp = std::abs( min.y - p.y );
	if( tmp < d ){
		d = tmp;
		normal = sf::Vector3f( 0.f, -1.f, 0.f );
	}
	tmp = std::abs( min.z - p.z );
	if( tmp < d ){
		d = tmp;
		normal = sf::Vector3f( 0.f, 0.f, -1.f );
	}
	tmp = std::abs( max.x - p.x );
	if( tmp < d ){
		d = tmp;
		normal = sf::Vector3f( 1.f, 0.f, 0.f );
	}
	tmp = std::abs( max.y - p.y );
	if( tmp < d ){
		d = tmp;
		normal = sf::Vector3f( 0.f, 1.f, 0.f );
	}
	tmp = std::abs( max.z - p.z );
	if( tmp < d ){
		d = tmp;
		normal = sf::Vector3f( 0.f, 0.f, 1.f );
	}

	return normal;
}

sf::Vector3f Cuboid::collisionColor( const Ray& ray ) const
{
	// In case no texture is set return the color of the object.
	if( !texture ){
		return getColor();
	}

	sf::Vector3f p( ray.evaluate( intersect( ray ) ) );

	int side = 0;
	float d = std::abs( min.x - p.x );

	float tmp = std::abs( min.y - p.y );
	if( tmp < d ){
		d = tmp;
		side = 1;
	}
	tmp = std::abs( min.z - p.z );
	if( tmp < d ){
		d = tmp;
		side = 2;
	}
	tmp = std::abs( max.x - p.x );
	if( tmp < d ){
		d = tmp;
		side = 3;
	}
	tmp = std::abs( max.y - p.y );
	if( tmp < d ){
		d = tmp;
		side = 4;
	}
	tmp = std::abs( max.z - p.z );
	if( tmp < d ){
		d = tmp;
		side = 5;
	}

	sf::Vector3f pos = min;
	sf::Vector3f dim = max - min;

	sf::Vector2f normCoord;

	if( side % 3 == 0 ){
		// Ignore x-coordinate.
		normCoord = sf::Vector2f( ( p.z - pos.z ) / dim.z, ( p.y - pos.y ) / dim.y );
	}
	else if( side % 3 == 1 ){
		// Ignore y-coordinate.
		normCoord = sf::Vector2f( ( p.x - pos.x ) / dim.x, ( p.z - pos.z ) / dim.z );
	}
	else if( side % 3 == 2 ){
		// Ignore z-coordinate.
		normCoord = sf::Vector2f( ( p.x - pos.x ) / dim.x, ( p.y - pos.y ) / dim.y );
	}

	sf::Color col = texture->getPixel( std::round( normCoord.x * ( texture->getSize().x - 1 ) ),
									   std::round( normCoord.y * ( texture->getSize().y - 1 ) ) );

	return thor::cwiseProduct( getColor(), sf::Vector3f( col.r / 255.f, col.g / 255.f, col.b / 255.f ) );
}
