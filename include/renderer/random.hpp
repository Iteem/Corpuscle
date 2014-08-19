#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#include <random>

#include <glm/glm.hpp>

// generates points uniformly distributed over S2
template< typename Generator >
glm::vec3 uniformOnSphere( Generator& gen );

#include "random.inl"

#endif // RANDOM_HPP_INCLUDED
