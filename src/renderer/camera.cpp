#include "camera.hpp"

#include <cmath>

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

glm::uvec2 Camera::getResolution() const
{
	return m_resolution;
}

void Camera::setResolution( const glm::uvec2& resolution )
{
	// Make sure the resolution is always at least 1x1.
	m_resolution = glm::uvec2( std::max( resolution.x, 1u ),
							   std::max( resolution.y, 1u ) );

	compile();
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}

void Camera::setPosition( const glm::vec3& position )
{
	m_position = position;
}

glm::vec3 Camera::getDirection() const
{
	return m_direction;
}

void Camera::setDirection( const glm::vec3& direction)
{
	if( direction == glm::vec3( 0.f, 0.f, 0.f ) ){
		return;
	}

	m_direction = glm::normalize( direction );

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

Ray Camera::getRay( glm::vec2 pixel ) const
{
	glm::vec3 direction = glm::normalize( m_direction +
										  m_horizontalVec * ( pixel.x - static_cast<float>( m_resolution.x ) / 2.f ) +
										  m_verticalVec   * ( pixel.y - static_cast<float>( m_resolution.y ) / 2.f ) );

	return Ray( m_position, direction );
}

void Camera::compile()
{
	// Calculate the unit vectors first.
	float theta = std::acos( m_direction.y ) - glm::radians( 90.f );
	float phi = std::atan2( m_direction.z, m_direction.x );
	m_verticalVec = glm::vec3( std::sin( theta ) * std::cos( phi ),
							   std::cos( theta ),
							   std::sin( theta ) * std::sin( phi ) );

	m_horizontalVec = glm::cross( m_direction, m_verticalVec );

	// Scale considering the FOV and resolution
	float delta = 2.f * std::tan( glm::radians( m_FOV / 2.f ) ) / static_cast<float>( m_resolution.y );
	m_verticalVec *= delta;
	m_horizontalVec *= delta;
}
