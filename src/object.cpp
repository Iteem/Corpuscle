#include "object.hpp"

Object::Object( sf::Vector3f color ) :
	m_color( color )
{
}

Object::~Object()
{
}

sf::Vector3f Object::getColor() const
{
	return m_color;
}

void Object::setColor( const sf::Vector3f& color )
{
	m_color = color;
}
