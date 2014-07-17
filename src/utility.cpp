#include "utility.hpp"

#include <boost/algorithm/string.hpp>

sf::Vector3f parseVector( const std::string &str, const sf::Vector3f& defaultValue  )
{
	std::vector<std::string> strings;
	boost::split( strings, str, boost::is_any_of("\t ") );

	if( strings.size() != 3 ){
		return defaultValue;
	}

	return sf::Vector3f( std::stof( strings[0] ),
						 std::stof( strings[1] ),
						 std::stof( strings[2] ) );
}
