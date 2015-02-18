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

float AABB::getSurface() const
{
	glm::vec3 size = getSize();
	return 2.f * ( size.x * size.y + size.x * size.z + size.y * size.z );
}

void AABB::extend( const AABB& other )
{
	lower = glm::min( lower, other.lower );
	upper = glm::max( upper, other.upper );
}

std::pair<float, float> AABB::intersect( const Ray& ray ) const
{
	auto t = std::make_pair<float, float>( ( lower.x - ray.getOrigin().x) * ray.getInverseDirection().x, ( upper.x - ray.getOrigin().x) * ray.getInverseDirection().x );
	if( ray.getInverseDirection().x < 0.f )
		std::swap( t.first, t.second );

	auto ty = std::make_pair<float, float>( ( lower.y - ray.getOrigin().y) * ray.getInverseDirection().y, ( upper.y - ray.getOrigin().y) * ray.getInverseDirection().y );
	if( ray.getInverseDirection().y < 0.f )
		std::swap( ty.first, ty.second );

	if ((t.first > ty.second) || (ty.first > t.second))
		return {inf, inf};

	if (ty.first > t.first)
		t.first = ty.first;
	if (ty.second < t.second)
		t.second = ty.second;

	auto tz = std::make_pair<float, float>( ( lower.z - ray.getOrigin().z) * ray.getInverseDirection().z, ( upper.z - ray.getOrigin().z) * ray.getInverseDirection().z );
	if( ray.getInverseDirection().z < 0.f )
		std::swap( tz.first, tz.second );

	if ((t.first > tz.second) || (tz.first > t.second))
		return {inf, inf};

	if (tz.first > t.first)
		t.first = tz.first;
	if (tz.second < t.second)
		t.second = tz.second;

	return t;
}
