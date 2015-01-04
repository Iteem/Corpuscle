#include "sphere.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtx/norm.hpp>

#include "utility.hpp"

Sphere::Sphere( float radius, glm::vec3 center, glm::vec3 color, glm::vec3 emission, Material material ) :
	Object( color, emission, material ),
	radius( radius ),
	center( center )
{
}

Sphere::~Sphere()
{
}


float Sphere::intersect( const Ray& ray ) const
{
	// This way we can ignore the origin of the ray.
	glm::vec3 op = center - ray.origin;

	// Solve the equation length(t*ray.direction - op) = radius for t.
	// If there it is no solution, return an uninitialized optional.
	float b = glm::dot( op, ray.direction );
	float D = b*b - glm::length2( op ) + radius*radius;

	if( D < 0 ){
		return inf;
	}

	D = std::sqrt( D );
	return std::min( b - D, b + D );
}


glm::vec3 Sphere::collisionNormal( const Ray& ray ) const
{
	float time = intersect( ray );
	if( time == inf )
		return glm::vec3();

	return glm::normalize( ray.evaluate( time ) - center );
}


glm::vec3 Sphere::collisionColor( const Ray& ray ) const
{
	return getColor();
}

AABB Sphere::getAABB() const
{
	return AABB( center - glm::vec3( radius ), center + glm::vec3( radius ) );
}

std::pair<Ray, float> Sphere::createRayToObject( std::mt19937& gen, const glm::vec3& point ) const
{
	glm::vec3 sw = center - point;
	glm::vec3 su = glm::normalize( glm::cross( fabs(sw.x) > .1f ? glm::vec3( 0.f, 1.f, 0.f ) : glm::vec3( 1.f, 0.f, 0.f ), sw ) );
	glm::vec3 sv = glm::cross( sw, su );
	float cos_a_max = std::sqrt( 1 - radius*radius / glm::length2( sw ) );

	static std::uniform_real_distribution<float> dist( 0.f, 1.f );
	float eps1 = dist( gen );
	float cos_a = 1.f - eps1 + eps1 * cos_a_max;
	float sin_a = sqrt( 1.f - cos_a * cos_a );

	float eps2 = dist( gen );
	float phi = 2.f * PI * eps2;

	glm::vec3 l = glm::normalize( su * std::cos( phi ) * sin_a + sv * std::sin( phi ) * sin_a + sw * cos_a );

	float area = 2.f * PI * ( 1.f - cos_a_max );

	return std::make_pair( Ray( point, l ), area );
}
