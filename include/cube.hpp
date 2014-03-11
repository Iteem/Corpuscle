#ifndef CUBE_HPP_INCLUDED
#define CUBE_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>

#include "object.hpp"

class Cube : public Object
{
public:
	Cube( sf::Vector3f min, sf::Vector3f max, sf::Vector3f color, sf::Vector3f emission = sf::Vector3f(), Material material = Material( Material::Type::Diffuse ) );
	virtual ~Cube();

	virtual float intersect( const Ray& ray ) const;
	virtual sf::Vector3f collisionNormal( const Ray& ray ) const;

	sf::Vector3f min;
	sf::Vector3f max;
};

#endif // CUBE_HPP_INCLUDED
