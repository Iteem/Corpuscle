#ifndef RAY_HPP_INCLUDED
#define RAY_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>

struct Ray
{
	Ray( sf::Vector3f origin, sf::Vector3f direction );
	~Ray();

	sf::Vector3f origin;
	sf::Vector3f direction;
};

#endif // RAY_HPP_INCLUDED
