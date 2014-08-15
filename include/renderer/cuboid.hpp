#ifndef CUBOID_HPP_INCLUDED
#define CUBOID_HPP_INCLUDED

#include <memory>

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector3.hpp>

#include "object.hpp"

class Cuboid : public Object
{
	public:
		Cuboid( sf::Vector3f min, sf::Vector3f max, sf::Vector3f color, sf::Vector3f emission = sf::Vector3f(), Material material = Material( Material::Type::Diffuse ) );
		virtual ~Cuboid();

		virtual float intersect( const Ray& ray ) const;
		virtual sf::Vector3f collisionNormal( const Ray& ray ) const;
		virtual sf::Vector3f collisionColor( const Ray& ray ) const;

		virtual std::pair<Ray, float> createRayToObject( std::mt19937& gen, const sf::Vector3f& point ) const;

		void setTexture( const sf::Image& image );
		void clearTexture();

		sf::Vector3f min;
		sf::Vector3f max;

	private:
		std::vector<sf::Vector3f> m_texture;
		sf::Vector2u m_textureSize;
};

#endif // CUBOID_HPP_INCLUDED
