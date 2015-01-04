#ifndef SPHERE_HPP_INCLUDED
#define SPHERE_HPP_INCLUDED

#include <glm/glm.hpp>

#include "object.hpp"

class Sphere : public Object
{
	public:
		Sphere( float radius, glm::vec3 center, glm::vec3 color, glm::vec3 emission = glm::vec3(), Material material = Material( Material::Type::Diffuse ) );
		virtual ~Sphere();

		virtual float intersect( const Ray& ray ) const;
		virtual glm::vec3 collisionNormal( const Ray& ray ) const;
		virtual glm::vec3 collisionColor( const Ray& ray ) const;

		virtual std::pair<Ray, float> createRayToObject( std::mt19937& gen, const glm::vec3& point ) const;

		virtual AABB getAABB() const;

		float radius;
		glm::vec3 center;
};

#endif // SPHERE_HPP_INCLUDED
