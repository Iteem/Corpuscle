#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#include <random>

#include <SFML/System/Vector3.hpp>

// generates points uniformly distributed over S2
template< typename Generator >
sf::Vector3f uniformOnSphere( Generator& gen );

#include "random.inl"

#endif // RANDOM_HPP_INCLUDED
