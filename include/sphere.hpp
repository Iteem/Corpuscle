#ifndef SPHERE_HPP_INCLUDED
#define SPHERE_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>

#include "object.hpp"

class Sphere : public Object
{
public:
	Sphere( float radius, sf::Vector3f center, sf::Vector3f color, sf::Vector3f emission = sf::Vector3f(), Material material = Material( Material::Type::Diffuse ) );
	virtual ~Sphere();

	virtual float intersect( const Ray& ray ) const;
	virtual sf::Vector3f collisionNormal( const Ray& ray ) const;

	float radius;
	sf::Vector3f center;
};

#endif // SPHERE_HPP_INCLUDED
