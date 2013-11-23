#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <memory>

// clamps value between 0 and 1
template< typename T >
T clamp( T param );

template< typename T >
sf::Vector3<T> gammmaCorrected( sf::Vector3<T> col, T param );

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

#include "utility.inl"

#endif // UTILITY_HPP_INCLUDED
