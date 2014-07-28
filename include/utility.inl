#include <algorithm>
#include <cmath>

template< typename T >
T clamp( T param )
{
	return std::max( 0.f, std::min( param, 1.f ) );
}

template< typename T >
sf::Vector3<T> clamp( sf::Vector3<T> vec )
{
	return sf::Vector3f( clamp( vec.x ),
						 clamp( vec.y ),
						 clamp( vec.z ) );
}


template< typename T >
sf::Vector3<T> gammmaCorrected( sf::Vector3<T> col, T param )
{
	return sf::Vector3f( std::pow( col.x, param ),
						 std::pow( col.y, param ),
						 std::pow( col.z, param ) );
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

constexpr float radToDeg( float angle )
{
	return angle * 180.f / PI;
}

constexpr float degToRad( float angle )
{
	return angle * PI / 180.f;
}

