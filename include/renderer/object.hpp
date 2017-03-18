#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <random>
#include <utility>

#include <glm/glm.hpp>

#include "aabb.hpp"
#include "ray.hpp"

class Material
{
public:
	enum class Type {
		Diffuse,
		Specular,
		Glossy,
		Glass
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
	Object( glm::vec3 color, glm::vec3 emission, Material material );
	virtual ~Object();

	glm::vec3 getColor() const;
	void setColor( const glm::vec3& color );

	glm::vec3 getEmission() const;
	void setEmission( const glm::vec3& emission );

	virtual float intersect( const Ray& ray ) const = 0;
	virtual glm::vec3 collisionNormal( const Ray& ray ) const = 0;
	virtual glm::vec3 collisionColor( const Ray& ray ) const = 0;

	virtual std::pair<Ray, float> createRayToObject( std::mt19937& gen, const glm::vec3& point ) const = 0;

	virtual AABB getAABB() const = 0;

	Material getMaterial() const;
	void setMaterial( Material material );

private:
	glm::vec3 m_color;
	glm::vec3 m_emission;

	Material m_material;
};

#endif // OBJECT_HPP_INCLUDED
