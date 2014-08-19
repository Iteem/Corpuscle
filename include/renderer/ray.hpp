#ifndef RAY_HPP_INCLUDED
#define RAY_HPP_INCLUDED

#include <glm/glm.hpp>

struct Ray
{
	Ray( glm::vec3 origin, glm::vec3 direction );
	~Ray();

	glm::vec3 evaluate( float time ) const;

	glm::vec3 origin;
	glm::vec3 direction;
};

#endif // RAY_HPP_INCLUDED
