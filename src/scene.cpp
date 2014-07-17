#include "scene.hpp"

#include <exception>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "cube.hpp"
#include "sphere.hpp"
#include "utility.hpp"

Scene::Scene()
{/*
	// initialize scene
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( -1e5+1,40.8,81.6 ), sf::Vector3f(.75f,.25f,.25f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 1e5+99,40.8,81.6 ), sf::Vector3f(.25f,.25f,.75f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8, -1e5 ),    sf::Vector3f(.75f,.75f,.75f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,40.8,1e5+200 ),  sf::Vector3f(0.f,0.f,0.f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50, -1e5, 81.6 ),   sf::Vector3f(.75f,.75f,.75f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 1e5f,  sf::Vector3f( 50,1e5+81.6,81.6 ), sf::Vector3f(.75f,.75f,.75f) ) );
	m_objects.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 27,16.5,47 ),       sf::Vector3f(1,1,1)*.999f, sf::Vector3f(), Material( Material::Type::Specular ) ) );
	m_objects.emplace_back( make_unique<Sphere>(   5.f, sf::Vector3f( 40,   5,85 ),       sf::Vector3f(0.8f, 0.5f, 0.2f), sf::Vector3f(), Material( Material::Type::Glossy, 0.6f ) ) );
	//m_objects.emplace_back( make_unique<Sphere>( 16.5f, sf::Vector3f( 73,16.5,78 ),       sf::Vector3f(1,1,1)*.999f ) );
	m_objects.emplace_back( make_unique<Cube>( sf::Vector3f(60,0,60), sf::Vector3f(90,30,90),       sf::Vector3f(.25f,.75f,.25f) ) );
	//m_objects.emplace_back( make_unique<Sphere>( 600.f, sf::Vector3f(50,681.6-.27,81.6),  sf::Vector3f(1,1,1), sf::Vector3f(1,1,1)*10.f ) );
	m_objects.emplace_back( make_unique<Cube>( sf::Vector3f(30,81,40), sf::Vector3f(70,82,80), sf::Vector3f(1,1,1), sf::Vector3f(1,1,1)*10.f ) );*/
}

Scene::~Scene()
{
}

std::pair<float, const Object *> Scene::getCollision( const Ray &ray, const Object *prevObject )
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

		if( type == "cube" ){
			sf::Vector3f min = parseVector( obj.second.get( "min", "") );
			sf::Vector3f max = parseVector( obj.second.get( "max", "") );

			m_objects.emplace_back( make_unique<Cube>( min, max, color, emission, material ) );
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
