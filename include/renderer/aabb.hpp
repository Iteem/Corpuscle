#ifndef AABB_HPP_INCLUDED
#define AABB_HPP_INCLUDED

#include <utility>

#include <glm/glm.hpp>

#include "ray.hpp"

class AABB
{
	public:
		AABB();
		AABB( glm::vec3 vec );
		AABB( glm::vec3 lower, glm::vec3 upper );

		glm::vec3 getSize() const;
		glm::vec3 getCenter() const;

		float getVolume() const;
		float getSurface() const;

		void extend( const AABB& other );
		void extend( const glm::vec3 &vec );

		AABB transform( const glm::mat4 &mat );

		std::pair<float, float> intersect( const Ray& ray ) const;

		glm::vec3 lower;
		glm::vec3 upper;
};

#endif // AABB_HPP_INCLUDED

