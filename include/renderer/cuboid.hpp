#ifndef CUBOID_HPP_INCLUDED
#define CUBOID_HPP_INCLUDED

#include <memory>

#include <glm/glm.hpp>
#include <SFML/Graphics/Image.hpp>

#include "object.hpp"

class Cuboid : public Object
{
	public:
		Cuboid( glm::vec3 min, glm::vec3 max, glm::vec3 color, glm::vec3 emission = glm::vec3(), Material material = Material( Material::Type::Diffuse ) );
		virtual ~Cuboid();

		virtual float intersect( const Ray& ray ) const;
		virtual glm::vec3 collisionNormal( const Ray& ray ) const;
		virtual glm::vec3 collisionColor( const Ray& ray ) const;

		virtual std::pair<Ray, float> createRayToObject( std::mt19937& gen, const glm::vec3& point ) const;

		void setTexture( const sf::Image& image );
		void clearTexture();

		glm::vec3 min;
		glm::vec3 max;

	private:
		std::vector<glm::vec3> m_texture;
		glm::uvec2 m_textureSize;
};

#endif // CUBOID_HPP_INCLUDED
