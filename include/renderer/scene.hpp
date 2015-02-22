#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <memory>
#include <utility>

#include "bvh.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "ray.hpp"

class Scene
{
	 public:
		Scene();
		~Scene();

		std::vector<const Object*> getObjects() const;

		std::pair<float, const Object *> getCollision( const Ray &ray, const Object *prevObject ) const;
		const std::vector<const Object *>& getLights() const;

		const Camera& getCamera() const;
		void setCamera( const Camera& camera );

		bool loadFromJSON( const std::string &path );

		void clear();

	 private:
	 	Camera m_camera;
	 	BVH m_bvh;

		std::vector< std::unique_ptr<Object> > m_objects;
		std::vector<const Object *> m_lights;
};


#endif // SCENE_HPP_INCLUDED
