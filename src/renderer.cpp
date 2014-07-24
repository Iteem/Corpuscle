#include "renderer.hpp"

#include <ctime>

#include <Thor/Vectors.hpp>

#include "object.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "utility.hpp"

Renderer::Renderer( const Scene *scene, sf::Vector2u dimension ) :
	m_scene( scene ),
	m_dimension( dimension )
{
	m_gen.seed( std::time( nullptr ) );
	std::uniform_real_distribution<float> dist( -1.f, 1.f );

	reset();
}

Renderer::~Renderer()
{
}

sf::Vector2u Renderer::getDimension() const
{
	return m_dimension;
}

void Renderer::setDimension( sf::Vector2u dimension )
{
	m_dimension = dimension;
	reset();
}

const Scene *Renderer::getScene() const
{
	return m_scene;
}

void Renderer::setScene( const Scene *scene )
{
	m_scene = scene;
	reset();
}

unsigned int Renderer::getSPP() const
{
	return m_spp;
}

void Renderer::render()
{
	// Distribution for anti-aliasing.
	std::uniform_real_distribution<float> dist( -1.f, 1.f );

	// Convert the dimension to a signed integer for convenience.
	sf::Vector2i dim( m_dimension );

	m_spp++;
	for ( int x = 0; x < dim.x; x++ ){
		for ( int y = 0 ; y < dim.y; y++ ){
			// Camera.
			float xpart = ( static_cast<float>( x - dim.x / 2 ) + dist( m_gen ) ) / dim.x * 0.9f;
			float ypart = ( static_cast<float>( y - dim.y / 2 ) + dist( m_gen ) ) / dim.x * 0.9f;

			Ray ray( sf::Vector3f( 50.f, 45.f, 295.f ), thor::unitVector( sf::Vector3f(xpart, ypart, -1.f) ) );

			m_pixels[x + dim.x * y] += radiance( ray, 5 );
		}
	}
}

sf::Image Renderer::getImage() const
{
	sf::Image image;
	image.create( m_dimension.x, m_dimension.y );

	for ( unsigned int x = 0; x < m_dimension.x; x++ ){
		for ( unsigned int y = 0 ; y < m_dimension.y; y++ ){
			sf::Vector3i col( gammmaCorrected( clamp( m_pixels[x + m_dimension.x * y] / static_cast<float>( m_spp ) ), 1.f/2.2f ) * 255.f );
			image.setPixel( x, m_dimension.y - y - 1, sf::Color( col.x, col.y, col.z) );
		}
	}

	return image;
}

void Renderer::reset()
{
	// Reset pixel data and make sure not to waste any memory.
	m_pixels.resize( m_dimension.x * m_dimension.y, sf::Vector3f() );
	m_pixels.shrink_to_fit();

	m_spp = 0;
}

sf::Vector3f Renderer::specularReflection( const sf::Vector3f &direction, const sf::Vector3f &normal ) const
{
	return direction - 2.f * thor::dotProduct( direction, normal ) * normal;
}

sf::Vector3f Renderer::diffuseReflection( const sf::Vector3f &normal ) const
{
	sf::Vector3f randomVec( uniformOnSphere( m_gen ) );
	float d = thor::dotProduct( randomVec, normal );
	return d > 0.f ? randomVec : randomVec - 2.f * d * normal;
}

sf::Vector3f Renderer::radiance( Ray ray, int depth, const Object *prevObject ) const
{
	auto collisionPair = m_scene->getCollision( ray, prevObject );
	float t = collisionPair.first;
	const Object *obj = collisionPair.second;

	// no collision, return black
	if( t == inf ){
		return sf::Vector3f();
	}

	// maximal depth, return emission only
	if( --depth < 0 ){
		return obj->getEmission();
	}

	// calculate new ray
	sf::Vector3f normal( obj->collisionNormal( ray ) );
	sf::Vector3f color( obj->collisionColor( ray ) );

	ray.origin = ray.evaluate( t );

	switch( obj->getMaterial().type ){
		case Material::Type::Diffuse:
			ray.direction = diffuseReflection( normal );
			break;
		case Material::Type::Specular:
			ray.direction = specularReflection( ray.direction, normal );
			break;
		case Material::Type::Glossy:
			ray.direction = thor::unitVector( (1 - obj->getMaterial().factor) * diffuseReflection( normal ) + obj->getMaterial().factor * specularReflection( ray.direction, normal ) );
	}


	return obj->getEmission() + thor::cwiseProduct( color, radiance( ray, depth - 1, obj ) );
}
