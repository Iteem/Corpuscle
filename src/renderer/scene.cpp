#include "scene.hpp"

#include <exception>
#include <iostream>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <glm/glm.hpp>
#include <SFML/Graphics/Image.hpp>

#include "nbt/chunk.hpp"
#include "cuboid.hpp"
#include "sphere.hpp"
#include "utility.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
}

std::vector<const Object*> Scene::getObjects() const
{
	std::vector<const Object*> vec;
	vec.reserve( m_objects.size() );
	for( const auto& obj : m_objects ){
		vec.push_back( obj.get() );
	}
	return vec;
}

std::pair<float, const Object *> Scene::getCollision( const Ray &ray, const Object *prevObject ) const
{
	return m_bvh.getCollision( ray, prevObject );

	/*float t = inf;
	Object * obj = nullptr;
	for( unsigned int i = 0; i < m_objects.size(); ++i ){
		float tmp = m_objects[i]->intersect( ray );

		if( tmp < t && tmp > 0.f && m_objects[i].get() != prevObject ) {
			t = tmp;
			obj = m_objects[i].get();
		}
	}

	return std::make_pair( t, obj );*/
}

const std::vector<const Object *>& Scene::getLights() const
{
	return m_lights;
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
        glm::vec3 color    = parseVector( obj.second.get( "color", ""), glm::vec3( 1.f, 1.f, 1.f ) );
        glm::vec3 emission = parseVector( obj.second.get( "emission", ""), glm::vec3( 0.f, 0.f, 0.f ) );

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
			glm::vec3 min = parseVector( obj.second.get( "min", "") );
			glm::vec3 max = parseVector( obj.second.get( "max", "") );

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
			glm::vec3 pos = parseVector( obj.second.get( "pos", "") );
			float radius = obj.second.get( "radius", 0.f );

			m_objects.emplace_back( make_unique<Sphere>( radius, pos, color, emission, material ) );
		} else {
			std::cout << "WARNING: Unknown object type " << type << "." << std::endl;
			continue;
		}
	}

	// Load NBT.
	std::string nbtPath = pt.get( "nbt.path", "" );
	if( nbtPath != "" ){
		Chunk chunk;
		if( chunk.load( nbtPath, 0, 0 ) ){
			for( int x = 0; x < 16; ++x ){
				for( int y = 0; y < 256; ++y ){
					for( int z = 0; z < 16; ++z ){
						if( chunk.isBlockVisible( glm::vec3( x, y, z ) ) ){
							m_objects.emplace_back( make_unique<Cuboid>( glm::vec3(x,y,z), glm::vec3( x + 1 , y + 1, z + 1), glm::vec3( 0.5f, 0.5f, 0.5f ) ) );
						}
					}
				}
			}
		}
	}

	// Load camera.
	m_camera.setPosition( parseVector( pt.get( "camera.position", "" ), glm::vec3( 0.f, 0.f, 0.f ) ) );
	m_camera.setDirection( glm::normalize( parseVector( pt.get( "camera.direction", "" ), glm::vec3( 0.f, 0.f, -1.f ) ) ) );
	m_camera.setFOV( pt.get( "camera.FOV", 70.f ) );

	// Cache lights.
	m_lights.clear();
	for( const auto& ptr : m_objects ){
		glm::vec3 emission = ptr->getEmission();
		if( emission.x != 0.f || emission.y != 0.f || emission.z != 0.f ){
			m_lights.push_back( ptr.get() );
		}
	}

	// Construct BVH.
	m_bvh.construct( getObjects() );
	m_bvh.print();

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
