#include "renderer.hpp"

#include <ctime>
#include <iostream>

#include "object.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "utility.hpp"

Renderer::Renderer( const Scene *scene, glm::uvec2 dimension ) :
	m_scene( scene ),
	m_dimension( dimension )
{
	m_gen.seed( std::time( nullptr ) );
	std::uniform_real_distribution<float> dist( -1.f, 1.f );
}

Renderer::~Renderer()
{
}

glm::uvec2 Renderer::getDimension() const
{
	return m_dimension;
}

void Renderer::setDimension( glm::uvec2 dimension )
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

void Renderer::render( std::vector< glm::vec3 >& pixels, glm::ivec2 position, glm::ivec2 dimension ) const
{
	// Distribution for anti-aliasing.
	std::uniform_real_distribution<float> dist( -1.f, 1.f );

	// Convert the dimension to a signed integer for convenience.
	glm::ivec2 dim( m_dimension );

	for ( int x = position.x; x < position.x + dimension.x; x++ ){
		for ( int y = position.y; y < position.y + dimension.y; y++ ){
			Ray ray( m_scene->getCamera().getRay( glm::vec2( x + dist( m_gen ), y + dist( m_gen ) ) ) );

			pixels[x + dim.x * y] += radiance( ray, 5 );
		}
	}
}

glm::vec3 Renderer::specularReflection( const glm::vec3 &direction, const glm::vec3 &normal ) const
{
	return direction - 2.f * glm::dot( direction, normal ) * normal;
}

glm::vec3 Renderer::diffuseReflection( const glm::vec3 &normal ) const
{
	std::uniform_real_distribution<float> dist( 0.f, 1.f );

	float r1 = 2.f * PI * dist( m_gen );
	float r2 = dist( m_gen );
	float r2s = std::sqrt( r2 );
	glm::vec3 u( glm::normalize( glm::cross( std::fabs( normal.x ) > 0.1f ? glm::vec3( 0.f, 1.f, 0.f ) : glm::vec3( 1.f, 0.f, 0.f ), normal ) ) );
	glm::vec3 v = glm::cross( normal, u );
	return glm::normalize( u * std::cos( r1 ) * r2s + v * std::sin( r1 ) * r2s + normal * std::sqrt( 1-r2 ) );
}

glm::vec3 Renderer::radiance( Ray ray, int depth, const Object *prevObject, float emission ) const
{
	auto collisionPair = m_scene->getCollision( ray, prevObject );
	float t = collisionPair.first;
	const Object *obj = collisionPair.second;

	// no collision, return black
	if( t == inf ){
		if( depth == 5 )
			return glm::vec3(0.3736, 0.6524, 0.8751);
		return glm::vec3();
	}

	// maximal depth, return emission only
	if( depth <= 0 ){
		return obj->getEmission() * emission;
	}

	glm::vec3 directLight;
	float newEmission = 1.f;

	// calculate new ray
	glm::vec3 normal( obj->collisionNormal( ray ) );
	glm::vec3 color( obj->collisionColor( ray ) );

	ray.setOrigin( ray.evaluate( t ) );

	std::uniform_real_distribution<float> dist( 0.f, 1.f );

	switch( obj->getMaterial().type ){
		case Material::Type::Diffuse:
			ray.setDirection( diffuseReflection( normal ) );

			// Direct lighting.
			for( auto light : m_scene->getLights() ){
				auto rayAreaPair( light->createRayToObject( m_gen, ray.getOrigin() ) );
				if( m_scene->getCollision( rayAreaPair.first, obj ).second == light ){
					directLight += rayAreaPair.second / PI * std::max( 0.f, glm::dot( rayAreaPair.first.getDirection(), normal ) ) * light->getEmission();
				}
			}

			newEmission = 0.f;
			break;
		case Material::Type::Specular:
			ray.setDirection( specularReflection( ray.getDirection(), normal ) );
			break;
		case Material::Type::Glossy:
			ray.setDirection( glm::normalize( (1 - obj->getMaterial().factor) * diffuseReflection( normal ) + obj->getMaterial().factor * specularReflection( ray.getDirection(), normal ) ) );
			break;
		case Material::Type::Glass:
			auto directionReflected = specularReflection( ray.getDirection(), normal );
			bool entering = glm::dot( normal, ray.getDirection() ) < 0.f;

			// The index of refraction (IOR) is given by Material.factor, the IOR for air is approximately 1
			float IORchange = entering ? 1.f / obj->getMaterial().factor : obj->getMaterial().factor;
			auto trueNormal = entering ? normal : -normal;
			float ddn = glm::dot( trueNormal, ray.getDirection() );
			float cos2t = 1.f - IORchange*IORchange * ( 1.f - ddn*ddn );

			// Check for total internal reflection
			if( cos2t < 0.f ){
				ray.setDirection( directionReflected );
				break;
			}

			// Fresnel using Schlick's approximation
			auto directionTransmitted = glm::normalize( ( ray.getDirection() * IORchange - trueNormal ) * ( ddn * IORchange + std::sqrt( cos2t ) ) );
			float a = obj->getMaterial().factor - 1.f;
			float b = obj->getMaterial().factor + 1.f;
			float R0 = a*a / (b*b);
			float c = 1 - ( entering ? -ddn : glm::dot( directionTransmitted, normal ) );

			float Re = R0 + ( 1.f - R0 ) * c*c*c*c*c;
			float Tr = 1.f - Re;

			if( dist( m_gen ) < Re ) {
				ray.setDirection( directionReflected );
			}
			else {
				ray.setDirection( directionTransmitted );
			}
			break;
	}


	return obj->getEmission() * emission + color * ( directLight + radiance( ray, depth - 1, obj, newEmission ) );
}
