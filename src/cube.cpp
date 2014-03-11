#include "cube.hpp"

#include <algorithm>
#include <cmath>

#include <Thor/Vectors/VectorAlgebra3D.hpp>

#include "utility.hpp"

Cube::Cube( sf::Vector3f min, sf::Vector3f max, sf::Vector3f color, sf::Vector3f emission, Material material ) :
	Object( color, emission, material ),
	min( min ),
	max( max )
{
}

Cube::~Cube()
{
}


float Cube::intersect( const Ray& ray ) const
{
	sf::Vector3f invdir( thor::cwiseQuotient( sf::Vector3f( 1.f, 1.f, 1.f ), ray.direction ) );
	std::pair<float, float> t = std::minmax( ( min.x - ray.origin.x) * invdir.x, ( max.x - ray.origin.x) * invdir.x );
	std::pair<float, float> ty = std::minmax( ( min.y - ray.origin.y) * invdir.y, ( max.y - ray.origin.y) * invdir.y );

	if ((t.first > ty.second) || (ty.first > t.second))
		return inf;

	if (ty.first > t.first)
		t.first = ty.first;
	if (ty.second < t.second)
		t.second = ty.second;

	std::pair<float, float> tz = std::minmax( ( min.z - ray.origin.z) * invdir.z, ( max.z - ray.origin.z) * invdir.z );

	if ((t.first > tz.second) || (tz.first > t.second))
		return inf;

	if (tz.first > t.first)
		t.first = tz.first;
	if (tz.second < t.second)
		t.second = tz.second;

	return t.first;
}


sf::Vector3f Cube::collisionNormal( const Ray& ray ) const
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
