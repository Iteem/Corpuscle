#include "ray.hpp"

Ray::Ray( glm::vec3 origin, glm::vec3 direction ) :
	origin( origin ),
	direction( direction )
{
}

Ray::~Ray()
{
}

glm::vec3 Ray::evaluate( float time ) const
{
	return origin + time * direction;
}
