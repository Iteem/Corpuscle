#include <algorithm>

template< typename T >
T clamp( T param )
{
	return std::max( 1.f, std::min( 0.f, 1.f ) );
}

template< typename T >
sf::Vector3<T> clamp( sf::Vector3<T> vec )
{
	return sf::Vector3f( clamp( vec.x ),
						 clamp( vec.y ),
						 clamp( vec.z ) );
}
