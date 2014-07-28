#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <limits>
#include <memory>
#include <string>

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

// clamps value between 0 and 1
template <typename T>
T clamp( T param );

template <typename T>
sf::Vector3<T> gammmaCorrected( sf::Vector3<T> col, T param );

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

const float inf = 1e20;
const float PI = 3.14159265358979f;

constexpr float radToDeg( float angle );
constexpr float degToRad( float angle );

sf::Vector3f parseVector( const std::string &str, const sf::Vector3f& defaultValue = sf::Vector3f() );

#include "utility.inl"

#endif // UTILITY_HPP_INCLUDED
