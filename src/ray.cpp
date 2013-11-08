#include "ray.hpp"

Ray::Ray( sf::Vector3f origin, sf::Vector3f direction ) :
	origin( origin ),
	direction( direction )
{
}

Ray::~Ray()
{
}
