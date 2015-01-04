#include "cuboid.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtx/norm.hpp>
#include <SFML/Graphics.hpp>

#include "utility.hpp"

Cuboid::Cuboid( glm::vec3 lower, glm::vec3 upper, glm::vec3 color, glm::vec3 emission, Material material ) :
	Object( color, emission, material ),
	aabb( lower, upper ),
	m_textureSize( 0, 0 )
{
}

Cuboid::~Cuboid()
{
}


float Cuboid::intersect( const Ray& ray ) const
{
	glm::vec3 invdir( 1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z );

	auto t = std::make_pair<float, float>( ( aabb.lower.x - ray.origin.x) * invdir.x, ( aabb.upper.x - ray.origin.x) * invdir.x );
	if( invdir.x < 0.f )
		std::swap( t.first, t.second );

	auto ty = std::make_pair<float, float>( ( aabb.lower.y - ray.origin.y) * invdir.y, ( aabb.upper.y - ray.origin.y) * invdir.y );
	if( invdir.y < 0.f )
		std::swap( ty.first, ty.second );

	if ((t.first > ty.second) || (ty.first > t.second))
		return inf;

	if (ty.first > t.first)
		t.first = ty.first;
	if (ty.second < t.second)
		t.second = ty.second;

	auto tz = std::make_pair<float, float>( ( aabb.lower.z - ray.origin.z) * invdir.z, ( aabb.upper.z - ray.origin.z) * invdir.z );
	if( invdir.z < 0.f )
		std::swap( tz.first, tz.second );

	if ((t.first > tz.second) || (tz.first > t.second))
		return inf;

	if (tz.first > t.first)
		t.first = tz.first;
	if (tz.second < t.second)
		t.second = tz.second;

	return t.first;
}


glm::vec3 Cuboid::collisionNormal( const Ray& ray ) const
{
	glm::vec3 p( ray.evaluate( intersect( ray ) ) );

	glm::vec3 normal( -1.f, 0.f, 0.f );
	float d = std::abs( aabb.lower.x - p.x );

	float tmp = std::abs( aabb.lower.y - p.y );
	if( tmp < d ){
		d = tmp;
		normal = glm::vec3( 0.f, -1.f, 0.f );
	}
	tmp = std::abs( aabb.lower.z - p.z );
	if( tmp < d ){
		d = tmp;
		normal = glm::vec3( 0.f, 0.f, -1.f );
	}
	tmp = std::abs( aabb.upper.x - p.x );
	if( tmp < d ){
		d = tmp;
		normal = glm::vec3( 1.f, 0.f, 0.f );
	}
	tmp = std::abs( aabb.upper.y - p.y );
	if( tmp < d ){
		d = tmp;
		normal = glm::vec3( 0.f, 1.f, 0.f );
	}
	tmp = std::abs( aabb.upper.z - p.z );
	if( tmp < d ){
		d = tmp;
		normal = glm::vec3( 0.f, 0.f, 1.f );
	}

	return normal;
}

glm::vec3 Cuboid::collisionColor( const Ray& ray ) const
{
	// In case no texture is set return the color of the object.
	if( m_texture.empty() ){
		return getColor();
	}

	glm::vec3 p( ray.evaluate( intersect( ray ) ) );

	int side = 0;
	float d = std::abs( aabb.lower.x - p.x );

	float tmp = std::abs( aabb.lower.y - p.y );
	if( tmp < d ){
		d = tmp;
		side = 1;
	}
	tmp = std::abs( aabb.lower.z - p.z );
	if( tmp < d ){
		d = tmp;
		side = 2;
	}
	tmp = std::abs( aabb.upper.x - p.x );
	if( tmp < d ){
		d = tmp;
		side = 3;
	}
	tmp = std::abs( aabb.upper.y - p.y );
	if( tmp < d ){
		d = tmp;
		side = 4;
	}
	tmp = std::abs( aabb.upper.z - p.z );
	if( tmp < d ){
		d = tmp;
		side = 5;
	}

	glm::vec3 pos = aabb.lower;
	glm::vec3 dim = aabb.getSize();

	sf::Vector2f normCoord;

	if( side % 3 == 0 ){
		// Ignore x-coordinate.
		normCoord = sf::Vector2f( ( p.z - pos.z ) / dim.z, ( p.y - pos.y ) / dim.y );
	}
	else if( side % 3 == 1 ){
		// Ignore y-coordinate.
		normCoord = sf::Vector2f( ( p.x - pos.x ) / dim.x, ( p.z - pos.z ) / dim.z );
	}
	else if( side % 3 == 2 ){
		// Ignore z-coordinate.
		normCoord = sf::Vector2f( ( p.x - pos.x ) / dim.x, ( p.y - pos.y ) / dim.y );
	}

	glm::vec3 color = m_texture[ std::max( 0u, std::min( m_textureSize.x - 1, static_cast<unsigned int>( std::floor( normCoord.x * m_textureSize.x ) ) ) ) +
								 std::max( 0u, std::min( m_textureSize.y - 1, static_cast<unsigned int>( std::floor( normCoord.y * m_textureSize.y ) ) ) ) * m_textureSize.x ];

	return getColor() * color;
}

std::pair<Ray, float>  Cuboid::createRayToObject( std::mt19937& gen, const glm::vec3& point ) const
{
	// This function only works correctly for far away points.
	glm::vec3 dim = aabb.getSize();
	glm::vec3 dim_2 = dim / 2.f;

	float xyArea = dim.x * dim.y;
	float xzArea = dim.x * dim.z;
	float yzArea = dim.y * dim.z;

	// Calculate visible area from point.
	float visibleArea[6];
	visibleArea[0] = std::max( 0.f, glm::normalize( aabb.lower + glm::vec3( 0.f, dim_2.y, dim_2.z ) - point ).x ) * yzArea;
	visibleArea[1] = std::max( 0.f, glm::normalize( aabb.lower + glm::vec3( dim_2.x, 0.f, dim_2.z ) - point ).y ) * xzArea;
	visibleArea[2] = std::max( 0.f, glm::normalize( aabb.lower + glm::vec3( dim_2.x, dim_2.y, 0.f ) - point ).z ) * xyArea;
	visibleArea[3] = std::max( 0.f, glm::normalize( point - aabb.upper - glm::vec3( 0.f, dim_2.y, dim_2.z ) ).x ) * yzArea;
	visibleArea[4] = std::max( 0.f, glm::normalize( point - aabb.upper - glm::vec3( dim_2.x, 0.f, dim_2.z ) ).y ) * xzArea;
	visibleArea[5] = std::max( 0.f, glm::normalize( point - aabb.upper - glm::vec3( dim_2.x, dim_2.y, 0.f ) ).z ) * xyArea;

	std::uniform_real_distribution<float> uDist( 0.f, 1.f );
	float eps1( uDist( gen ) );
	float eps2( uDist( gen ) );

	// Choose random side weighted by the visible area.
	glm::vec3 pointOnSurface;
	std::discrete_distribution<> dist( std::begin( visibleArea ), std::end( visibleArea ) );
	switch( dist( gen ) ){
		case 0:
			pointOnSurface = aabb.lower + glm::vec3( 0.f, eps1 * dim.y, eps2 * dim.z );
			break;
		case 1:
			pointOnSurface = aabb.lower + glm::vec3( eps1 * dim.x, 0.f, eps2 * dim.z );
			break;
		case 2:
			pointOnSurface = aabb.lower + glm::vec3( eps1 * dim.x, eps2 * dim.y, 0.f );
			break;
		case 3:
			pointOnSurface = aabb.upper - glm::vec3( 0.f, eps1 * dim.y, eps2 * dim.z );
			break;
		case 4:
			pointOnSurface = aabb.upper - glm::vec3( eps1 * dim.x, 0.f, eps2 * dim.z );
			break;
		case 5:
			pointOnSurface = aabb.upper - glm::vec3( eps1 * dim.x, eps2 * dim.y, 0.f );
			break;
	}

	float totalArea = 0.f;
	for( auto dArea : visibleArea ){
		totalArea += dArea;
	}

	glm::vec3 l = pointOnSurface - point;

	return std::make_pair( Ray( point, glm::normalize( l ) ), totalArea / glm::length2( l ) );
}

AABB Cuboid::getAABB() const
{
	return aabb;
}

void Cuboid::setTexture( const sf::Image& image )
{
	m_textureSize = glm::uvec2( image.getSize().x, image.getSize().y );

	m_texture.resize( m_textureSize.x * m_textureSize.y );
	m_texture.shrink_to_fit();

	for( unsigned int x = 0; x < m_textureSize.x; ++x ){
		for( unsigned int y = 0; y < m_textureSize.y; ++y ){
			sf::Color col = image.getPixel( x, y );
			// Textures are gamma-corrected, so don't forget to reverse that.
			m_texture[ x + y * m_textureSize.x ] = gammmaCorrected( glm::vec3( col.r / 255.f, col.g / 255.f, col.b / 255.f ), 2.2f );
		}
	}
}

void Cuboid::clearTexture()
{
	m_textureSize = glm::uvec2();

	m_texture.clear();
	m_texture.shrink_to_fit();
}
