#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include "ray.hpp"

class Camera
{
	public:
		Camera();
		~Camera();

		sf::Vector2u getResolution() const;
		void setResolution( const sf::Vector2u& resolution );

		sf::Vector3f getPosition() const;
		void setPosition( const sf::Vector3f& position );

		sf::Vector3f getDirection() const;
		void setDirection( const sf::Vector3f& direction);

		float getFOV() const;
		void setFOV( float angle );

		Ray getRay( sf::Vector2f pixel ) const;

	private:
		void compile();

		sf::Vector2u m_resolution;

		sf::Vector3f m_position;
		sf::Vector3f m_direction;
		float m_FOV;

		sf::Vector3f m_horizontalVec;
		sf::Vector3f m_verticalVec;
};


#endif // CAMERA_HPP_INCLUDED
