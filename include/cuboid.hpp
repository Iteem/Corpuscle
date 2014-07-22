#ifndef CUBOID_HPP_INCLUDED
#define CUBOID_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>

#include "object.hpp"

class Cuboid : public Object
{
public:
	Cuboid( sf::Vector3f min, sf::Vector3f max, sf::Vector3f color, sf::Vector3f emission = sf::Vector3f(), Material material = Material( Material::Type::Diffuse ) );
	virtual ~Cuboid();

	virtual float intersect( const Ray& ray ) const;
	virtual sf::Vector3f collisionNormal( const Ray& ray ) const;

	sf::Vector3f min;
	sf::Vector3f max;
};

#endif // CUBOID_HPP_INCLUDED
