#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glm/glm.hpp>

#include "ray.hpp"

class Camera
{
	public:
		Camera();
		~Camera();

		glm::uvec2 getResolution() const;
		void setResolution( const glm::uvec2& resolution );

		glm::vec3 getPosition() const;
		void setPosition( const glm::vec3& position );

		glm::vec3 getDirection() const;
		void setDirection( const glm::vec3& direction);

		float getFOV() const;
		void setFOV( float angle );

		Ray getRay( glm::vec2 pixel ) const;

	private:
		void compile();

		glm::uvec2 m_resolution;

		glm::vec3 m_position;
		glm::vec3 m_direction;
		float m_FOV;

		glm::vec3 m_horizontalVec;
		glm::vec3 m_verticalVec;
};


#endif // CAMERA_HPP_INCLUDED
