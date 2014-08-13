#include "camera.hpp"

#include <cmath>
#include <iostream>
#include <Thor/Graphics.hpp>

#include <Thor/Vectors.hpp>

#include "utility.hpp"

Camera::Camera() :
	m_resolution( 1, 1 ),
	m_direction( 0.f, 1.f, 0.f ),
	m_FOV( 70 )
{
	compile();
}

Camera::~Camera()
{
}

sf::Vector2u Camera::getResolution() const
{
	return m_resolution;
}

void Camera::setResolution( const sf::Vector2u& resolution )
{
	// Make sure the resolution is always at least 1x1.
	m_resolution = sf::Vector2u( std::max( resolution.x, 1u ),
								 std::max( resolution.y, 1u ) );

	compile();
}

sf::Vector3f Camera::getPosition() const
{
	return m_position;
}

void Camera::setPosition( const sf::Vector3f& position )
{
	m_position = position;
}

sf::Vector3f Camera::getDirection() const
{
	return m_direction;
}

void Camera::setDirection( const sf::Vector3f& direction)
{
	if( direction == sf::Vector3f( 0.f, 0.f, 0.f ) ){
		return;
	}

	m_direction = thor::unitVector( direction );

	compile();
}

float Camera::getFOV() const
{
	return m_FOV;
}

void Camera::setFOV( float angle )
{
	m_FOV = std::max( angle, 1.f );

	compile();
}

Ray Camera::getRay( sf::Vector2f pixel ) const
{
	sf::Vector3f direction = thor::unitVector( m_direction +
											   m_horizontalVec * ( pixel.x - static_cast<float>( m_resolution.x ) / 2.f ) +
											   m_verticalVec   * ( pixel.y - static_cast<float>( m_resolution.y ) / 2.f ) );

	return Ray( m_position, direction );
}

void Camera::compile()
{
	// Calculate the unit vectors first.
	float theta = std::acos( m_direction.y ) - degToRad( 90.f );
	float phi = std::atan2( m_direction.z, m_direction.x );
	m_verticalVec = sf::Vector3f( std::sin( theta ) * std::cos( phi ),
								  std::cos( theta ),
								  std::sin( theta ) * std::sin( phi ) );

	m_horizontalVec = thor::crossProduct( m_direction, m_verticalVec );

	// Scale considering the FOV and resolution
	float delta = 2.f * std::tan( degToRad( m_FOV / 2.f ) ) / static_cast<float>( m_resolution.y );
	m_verticalVec *= delta;
	m_horizontalVec *= delta;
}
