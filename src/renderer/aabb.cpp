#include "aabb.hpp"

#include "utility.hpp"

AABB::AABB()
{
}

AABB::AABB( glm::vec3 lower, glm::vec3 upper ) :
	lower( lower ),
	upper( upper )
{
}

glm::vec3 AABB::getSize() const
{
	return upper - lower;
}

glm::vec3 AABB::getCenter() const
{
	return 0.5f * ( upper + lower );
}

float AABB::getVolume() const
{
	glm::vec3 size = getSize();
	return size.x * size.y * size.z;
}

void AABB::extend( const AABB& other )
{
	lower = glm::min( lower, other.lower );
	upper = glm::max( upper, other.upper );
}

float AABB::intersect( const Ray& ray ) const
{
	glm::vec3 invdir( 1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z );

	auto t = std::make_pair<float, float>( ( lower.x - ray.origin.x) * invdir.x, ( upper.x - ray.origin.x) * invdir.x );
	if( invdir.x < 0.f )
		std::swap( t.first, t.second );

	auto ty = std::make_pair<float, float>( ( lower.y - ray.origin.y) * invdir.y, ( upper.y - ray.origin.y) * invdir.y );
	if( invdir.y < 0.f )
		std::swap( ty.first, ty.second );

	if ((t.first > ty.second) || (ty.first > t.second))
		return inf;

	if (ty.first > t.first)
		t.first = ty.first;
	if (ty.second < t.second)
		t.second = ty.second;

	auto tz = std::make_pair<float, float>( ( lower.z - ray.origin.z) * invdir.z, ( upper.z - ray.origin.z) * invdir.z );
	if( invdir.z < 0.f )
		std::swap( tz.first, tz.second );

	if ((t.first > tz.second) || (tz.first > t.second))
		return inf;

	if (tz.first > t.first)
		t.first = tz.first;
	if (tz.second < t.second)
		t.second = tz.second;

	return t.first;
}
