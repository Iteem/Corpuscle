#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

#include "scene.hpp"

class Renderer
{
	public:
		Renderer( const Scene *scene, sf::Vector2u dimension );
		~Renderer();

		sf::Vector2u getDimension() const;
		void setDimension( sf::Vector2u dimension );

		const Scene *getScene() const;
		void setScene( const Scene *scene );

		unsigned int getSPP() const;

		/// Renders one more sample.
		void render();

		sf::Image getImage() const;
	private:
		void reset();

		sf::Vector3f specularReflection( const sf::Vector3f &direction, const sf::Vector3f &normal ) const;
		sf::Vector3f diffuseReflection( const sf::Vector3f &normal ) const;
		sf::Vector3f radiance( Ray ray, int depth, const Object *prevObject = nullptr ) const;

		const Scene *m_scene;
		sf::Vector2u m_dimension;

		mutable std::mt19937 m_gen;

		std::vector< sf::Vector3f > m_pixels;
		unsigned int m_spp;
};

#endif // RENDERER_HPP_INCLUDED
