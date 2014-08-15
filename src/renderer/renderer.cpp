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
}

const Scene *Renderer::getScene() const
{
	return m_scene;
}

void Renderer::setScene( const Scene *scene )
{
	m_scene = scene;
}

void Renderer::render( std::vector< sf::Vector3f >& pixels, sf::IntRect rect ) const
{
	// Distribution for anti-aliasing.
	std::uniform_real_distribution<float> dist( -1.f, 1.f );

	// Convert the dimension to a signed integer for convenience.
	sf::Vector2i dim( m_dimension );

	for ( int x = rect.left; x < rect.left + rect.width; x++ ){
		for ( int y = rect.top; y < rect.top + rect.height; y++ ){
			Ray ray( m_scene->getCamera().getRay( sf::Vector2f( x + dist( m_gen ), y + dist( m_gen ) ) ) );

			pixels[x + dim.x * y] += radiance( ray, 5 );
		}
	}
}

sf::Vector3f Renderer::specularReflection( const sf::Vector3f &direction, const sf::Vector3f &normal ) const
{
	return direction - 2.f * thor::dotProduct( direction, normal ) * normal;
}

sf::Vector3f Renderer::diffuseReflection( const sf::Vector3f &normal ) const
{
	//sf::Vector3f randomVec( uniformOnSphere( m_gen ) );
	//float d = thor::dotProduct( randomVec, normal );
	//return d > 0.f ? randomVec : randomVec - 2.f * d * normal;

	std::uniform_real_distribution<float> dist( 0.f, 1.f );

	float r1 = 2.f * PI * dist( m_gen );
	float r2 = dist( m_gen );
	float r2s = std::sqrt( r2 );
	sf::Vector3f u( thor::unitVector( thor::crossProduct( std::fabs( normal.x ) > 0.1f ? sf::Vector3f( 0.f, 1.f, 0.f ) : sf::Vector3f( 1.f, 0.f, 0.f ), normal ) ) );
	sf::Vector3f v = thor::crossProduct( normal, u );
	return thor::unitVector( u * std::cos( r1 ) * r2s + v * std::sin( r1 ) * r2s + normal * std::sqrt( 1-r2 ) );
}

sf::Vector3f Renderer::radiance( Ray ray, int depth, const Object *prevObject, float emission ) const
{
	auto collisionPair = m_scene->getCollision( ray, prevObject );
	float t = collisionPair.first;
	const Object *obj = collisionPair.second;

	// no collision, return black
	if( t == inf ){
		return sf::Vector3f();
	}

	// maximal depth, return emission only
	if( depth <= 0 ){
		return obj->getEmission();
	}

	sf::Vector3f directLight;
	float newEmission = 1.f;

	// calculate new ray
	sf::Vector3f normal( obj->collisionNormal( ray ) );
	sf::Vector3f color( obj->collisionColor( ray ) );

	ray.origin = ray.evaluate( t );

	switch( obj->getMaterial().type ){
		case Material::Type::Diffuse:
			ray.direction = diffuseReflection( normal );

			// Direct lighting.
			for( auto light : m_scene->getLights() ){
				auto rayAreaPair( light->createRayToObject( m_gen, ray.origin ) );
				if( m_scene->getCollision( rayAreaPair.first, nullptr ).second == light ){
					directLight += rayAreaPair.second / PI * std::max( 0.f, thor::dotProduct( rayAreaPair.first.direction, normal ) ) *
					               thor::cwiseProduct( color, light->getEmission() );
				}
			}

			newEmission = 0.f;
			break;
		case Material::Type::Specular:
			ray.direction = specularReflection( ray.direction, normal );
			break;
		case Material::Type::Glossy:
			ray.direction = thor::unitVector( (1 - obj->getMaterial().factor) * diffuseReflection( normal ) + obj->getMaterial().factor * specularReflection( ray.direction, normal ) );
	}


	return obj->getEmission() * emission  + directLight + thor::cwiseProduct( color, radiance( ray, depth - 1, obj, newEmission ) );
}
