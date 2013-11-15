#include "sphere.hpp"

#include <algorithm>
#include <cmath>

#include <Thor/Vectors/VectorAlgebra3D.hpp>

Sphere::Sphere( float radius, sf::Vector3f center, sf::Vector3f color ) :
	Object( color ),
	radius( radius ),
	center( center )
{
}

Sphere::~Sphere()
{
}


boost::optional<float> Sphere::intersect( const Ray& ray ) const
{
	// This way we can ignore the origin of the ray.
	sf::Vector3f op = center - ray.origin;

	// Solve the equation length(t*ray.direction - op) = radius for t.
	// If there it is no solution, return an uninitialized optional.
	float b = thor::dotProduct( op, ray.direction );
	float D = b*b - thor::squaredLength( op ) + radius*radius;

	if( D < 0 ){
		return boost::optional<float>();
	}

	D = std::sqrt( D );
	return std::min( b - D, b + D );
}


boost::optional<sf::Vector3f> Sphere::collisionNormal( const Ray& ray ) const
{
	boost::optional<float> time = intersect( ray );
	if( !time )
		return boost::optional<sf::Vector3f>();

	return thor::unitVector( ray.evaluate( *time ) - center );
}
