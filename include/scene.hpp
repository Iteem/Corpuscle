#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <memory>
#include <utility>

#include "camera.hpp"
#include "object.hpp"
#include "ray.hpp"

class Scene
{
	 public:
		Scene();
		~Scene();

		std::pair<float, const Object *> getCollision( const Ray &ray, const Object *prevObject ) const;

		const Camera& getCamera() const;
		void setCamera( const Camera& camera );

		bool loadFromJSON( const std::string &path );

	 private:
	 	Camera m_camera;
		std::vector< std::unique_ptr<Object> > m_objects;
};


#endif // SCENE_HPP_INCLUDED
