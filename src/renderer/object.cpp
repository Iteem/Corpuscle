#include "object.hpp"

Material::Material( Type type ) :
	type( type )
{
}

Material::Material( Type type, float factor ) :
	type( type ),
	factor( factor )
{
}

Material::~Material()
{
}

Object::Object( sf::Vector3f color, sf::Vector3f emission, Material material ) :
	m_color( color ),
	m_emission( emission ),
	m_material( material )
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

sf::Vector3f Object::getEmission() const
{
	return m_emission;
}

void Object::setEmission( const sf::Vector3f& emission )
{
	m_emission = emission;
}

Material Object::getMaterial() const
{
	return m_material;
}

void Object::setMaterial( Material material )
{
	m_material = material;
}
