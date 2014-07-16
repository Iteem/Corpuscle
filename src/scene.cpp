#include "scene.hpp"

#include "cube.hpp"
#include "sphere.hpp"
#include "utility.hpp"


Scene::Scene()
{
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
	m_objects.emplace_back( make_unique<Cube>( sf::Vector3f(30,81,40), sf::Vector3f(70,82,80), sf::Vector3f(1,1,1), sf::Vector3f(1,1,1)*10.f ) );
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
