#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <limits>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <SFML/Graphics/Color.hpp>

// clamps value between 0 and 1
template <typename T>
T clamp( T param );

glm::vec3 clamp( glm::vec3 vec );

glm::vec3 gammmaCorrected( glm::vec3 col, float param );

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

const float inf = 1e20;
const float PI = 3.14159265358979f;

glm::vec3 parseVector( const std::string &str, const glm::vec3& defaultValue = glm::vec3() );

#include "utility.inl"

#endif // UTILITY_HPP_INCLUDED
