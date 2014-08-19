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

Object::Object( glm::vec3 color, glm::vec3 emission, Material material ) :
	m_color( color ),
	m_emission( emission ),
	m_material( material )
{
}

Object::~Object()
{
}

glm::vec3 Object::getColor() const
{
	return m_color;
}

void Object::setColor( const glm::vec3& color )
{
	m_color = color;
}

glm::vec3 Object::getEmission() const
{
	return m_emission;
}

void Object::setEmission( const glm::vec3& emission )
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
