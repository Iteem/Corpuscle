#include "scene.hpp"

#include <exception>
#include <iostream>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <SFML/Graphics/Image.hpp>

#include "cuboid.hpp"
#include "sphere.hpp"
#include "utility.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
}

std::pair<float, const Object *> Scene::getCollision( const Ray &ray, const Object *prevObject ) const
{
	float t = inf;
	Object * obj = nullptr;
	for( unsigned int i = 0; i < m_objects.size(); ++i ){
		float tmp = m_objects[i]->intersect( ray );

		if( tmp < t && tmp > 0.f && m_objects[i].get() != prevObject ) {
			t = tmp;
			obj = m_objects[i].get();
		}
	}

	return std::make_pair( t, obj );
}

bool Scene::loadFromJSON( const std::string &path )
{
	using namespace boost::property_tree;
	ptree pt;

	// Try to load the file into a property_tree.
	try {
		json_parser::read_json( path, pt );
	}
	catch( std::exception &e ){
		std::cerr << "Failed to load " << path << ": " << std::endl;
		std::cerr << e.what();
		return false;
	}

	// Iterate over all objects.
	for( auto obj : pt.get_child( "objects", ptree() ) ){

		// Load common properties.
        sf::Vector3f color    = parseVector( obj.second.get( "color", ""), sf::Vector3f( 1.f, 1.f, 1.f ) );
        sf::Vector3f emission = parseVector( obj.second.get( "emission", ""), sf::Vector3f( 0.f, 0.f, 0.f ) );

        Material material( Material::Type::Diffuse );

        std::string materialType = obj.second.get( "material", "" );
        if( materialType == "specular" ){
			material.type = Material::Type::Specular;
        }
        else if( materialType == "glossy" ){
			material.type = Material::Type::Glossy;
			material.factor = obj.second.get( "glossiness", 0.5f );
        }


		// Load type dependent properties and add the object to the scene.
		std::string type( obj.second.get( "type", "" ) );

		if( type == "cuboid" ){
			sf::Vector3f min = parseVector( obj.second.get( "min", "") );
			sf::Vector3f max = parseVector( obj.second.get( "max", "") );

			auto cuboid = make_unique<Cuboid>( min, max, color, emission, material );

			std::string imagePath = obj.second.get( "texture", "" );
			if( imagePath != "" ){
				sf::Image image;

				if( image.loadFromFile( imagePath ) ){
					cuboid->setTexture( image );
				}
			}

			m_objects.emplace_back( std::move( cuboid ) );
		} else if ( type == "sphere" ){
			sf::Vector3f pos = parseVector( obj.second.get( "pos", "") );
			float radius = obj.second.get( "radius", 0.f );

			m_objects.emplace_back( make_unique<Sphere>( radius, pos, color, emission, material ) );
		} else {
			std::cout << "WARNING: Unknown object type " << type << "." << std::endl;
			continue;
		}
	}

    return true;
}

const Camera& Scene::getCamera() const
{
	return m_camera;
}

void Scene::setCamera( const Camera& camera )
{
	m_camera = camera;
}
