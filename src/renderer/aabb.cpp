#include "aabb.hpp"

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
