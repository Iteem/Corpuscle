#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>
#include <boost/optional.hpp>

#include "ray.hpp"

class Object
{
public:
	Object( sf::Vector3f color );
	virtual ~Object();

	sf::Vector3f getColor() const;
	void setColor( const sf::Vector3f& color );

	virtual boost::optional<float> intersect( const Ray& ray ) const = 0;
	virtual boost::optional<float> collisionNormal( const Ray& ray ) const = 0;

private:
	sf::Vector3f m_color;
};

#endif // OBJECT_HPP_INCLUDED
