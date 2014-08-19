#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <random>
#include <vector>

#include <glm/glm.hpp>
#include <SFML/Graphics/Image.hpp>

#include "scene.hpp"

class Renderer
{
	public:
		Renderer( const Scene *scene, glm::uvec2 dimension );
		~Renderer();

		glm::uvec2 getDimension() const;
		void setDimension( glm::uvec2 dimension );

		const Scene *getScene() const;
		void setScene( const Scene *scene );

		/// Renders one more sample.
		void render( std::vector< glm::vec3 >& pixels, glm::ivec2 position, glm::ivec2 dimension ) const;

		sf::Image getImage() const;

	private:
		glm::vec3 specularReflection( const glm::vec3 &direction, const glm::vec3 &normal ) const;
		glm::vec3 diffuseReflection( const glm::vec3 &normal ) const;
		glm::vec3 radiance( Ray ray, int depth, const Object *prevObject = nullptr, float emission = 1.f ) const;

		const Scene *m_scene;
		glm::uvec2 m_dimension;

		mutable std::mt19937 m_gen;
};

#endif // RENDERER_HPP_INCLUDED
