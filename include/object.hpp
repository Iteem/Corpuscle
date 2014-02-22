#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <SFML/System/Vector3.hpp>
#include <boost/optional.hpp>

#include "ray.hpp"

class Material
{
public:
	enum class Type {
		Diffuse,
		Specular,
		Glossy
	};

	Material( Type type );
	Material( Type type, float factor );
	~Material();

	Type type;
	float factor;
};

class Object
{
public:
	Object( sf::Vector3f color, sf::Vector3f emission, Material material );
	virtual ~Object();

	sf::Vector3f getColor() const;
	void setColor( const sf::Vector3f& color );

	sf::Vector3f getEmission() const;
	void setEmission( const sf::Vector3f& emission );

	virtual float intersect( const Ray& ray ) const = 0;
	virtual sf::Vector3f collisionNormal( const Ray& ray ) const = 0;

	Material getMaterial() const;
	void setMaterial( Material material );

private:
	sf::Vector3f m_color;
	sf::Vector3f m_emission;

	Material m_material;
};

#endif // OBJECT_HPP_INCLUDED
