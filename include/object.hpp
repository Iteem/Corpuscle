#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>
#include <boost/optional.hpp>

#include "ray.hpp"

class Object
{
public:
	Object( sf::Vector3f color, sf::Vector3f emission );
	virtual ~Object();

	sf::Vector3f getColor() const;
	void setColor( const sf::Vector3f& color );

	sf::Vector3f getEmission() const;
	void setEmission( const sf::Vector3f& emission );

	virtual float intersect( const Ray& ray ) const = 0;
	virtual sf::Vector3f collisionNormal( const Ray& ray ) const = 0;

private:
	sf::Vector3f m_color;
	sf::Vector3f m_emission;
};

#endif // OBJECT_HPP_INCLUDED
