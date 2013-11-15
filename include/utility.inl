#include <algorithm>

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

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
