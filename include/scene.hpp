#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <memory>
#include <utility>

#include "ray.hpp"
#include "object.hpp"

class Scene
{
	 public:
		Scene();
		~Scene();

		std::pair<float, const Object *> getCollision( const Ray &ray, const Object *prevObject );

		bool loadFromJSON( const std::string &path );

	 private:
		std::vector< std::unique_ptr<Object> > m_objects;
};


#endif // SCENE_HPP_INCLUDED
