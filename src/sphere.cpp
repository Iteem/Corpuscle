#include "sphere.hpp"

#include <algorithm>
#include <cmath>

#include <Thor/Vectors/VectorAlgebra3D.hpp>

#include "utility.hpp"

Sphere::Sphere( float radius, sf::Vector3f center, sf::Vector3f color, sf::Vector3f emission ) :
	Object( color, emission ),
	radius( radius ),
	center( center )
{
}

Sphere::~Sphere()
{
}


float Sphere::intersect( const Ray& ray ) const
{
	// This way we can ignore the origin of the ray.
	sf::Vector3f op = center - ray.origin;

	// Solve the equation length(t*ray.direction - op) = radius for t.
	// If there it is no solution, return an uninitialized optional.
	float b = thor::dotProduct( op, ray.direction );
	float D = b*b - thor::squaredLength( op ) + radius*radius;

	if( D < 0 ){
		return inf;
	}

	D = std::sqrt( D );
	return std::min( b - D, b + D );
}


sf::Vector3f Sphere::collisionNormal( const Ray& ray ) const
{
	float time = intersect( ray );
	if( time == inf )
		return sf::Vector3f();

	return thor::unitVector( ray.evaluate( time ) - center );
}
