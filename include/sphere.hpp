#ifndef SPHERE_HPP_INCLUDED
#define SPHERE_HPP_INCLUDED

#include <boost/optional.hpp>
#include <SFML/System/Vector3.hpp>

#include "object.hpp"

class Sphere : public Object
{
public:
	Sphere( float radius, sf::Vector3f center, sf::Vector3f color = sf::Vector3f() );
	virtual ~Sphere();

	virtual boost::optional<float> intersect( const Ray& ray ) const;
	virtual boost::optional<float> collisionNormal( const Ray& ray ) const;

	float radius;
	sf::Vector3f center;
};

#endif // SPHERE_HPP_INCLUDED
