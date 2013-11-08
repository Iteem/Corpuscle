#ifndef SPHERE_HPP_INCLUDED
#define SPHERE_HPP_INCLUDED

#include <boost/optional.hpp>
#include <SFML/System/Vector3.hpp>

#include "ray.hpp"

class Sphere
{
public:
	Sphere( float radius, sf::Vector3f center );
	~Sphere();

	boost::optional<float> intersect( const Ray& ray );

	float radius;
	sf::Vector3f center;
};

#endif // SPHERE_HPP_INCLUDED
