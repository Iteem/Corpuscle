#ifndef RAY_HPP_INCLUDED
#define RAY_HPP_INCLUDED

#include <glm/glm.hpp>

class Ray
{
	public:
		Ray( glm::vec3 origin, glm::vec3 direction );

		glm::vec3 evaluate( float time ) const;

		void setDirection( const glm::vec3& direction );
		glm::vec3 getDirection() const;
		glm::vec3 getInverseDirection() const;

		void setOrigin( const glm::vec3& origin );
		glm::vec3 getOrigin() const;

	private:
		glm::vec3 m_origin;
		glm::vec3 m_direction;
		glm::vec3 m_inverseDirection;
};

#include "ray.inl"

#endif // RAY_HPP_INCLUDED
