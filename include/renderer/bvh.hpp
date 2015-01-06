#ifndef BVH_HPP_INCLUDED
#define BVH_HPP_INCLUDED

#include <vector>

#include "aabb.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "utility.hpp"

class BVH
{
	public:
		BVH() = default;
		BVH( std::vector<const Object *> objects );

		void construct( std::vector<const Object *> objects );

		std::pair<float, const Object *> getCollision( const Ray &ray, const Object *prevObject ) const;

		void print();

	private:
		struct Node
		{
			Node() = default;
			Node( std::vector<const Object *> &objects, size_t left, size_t right );

			void construct( std::vector<const Object *> &objects, size_t left, size_t right );
			bool isLeaf() const;

			std::pair<float, const Object *> getCollision( const std::vector<const Object *> &objects, const Ray &ray, const Object *prevObject ) const;

			void print(std::string indent);

			static const unsigned int ObjectsPerLeaf = 5;

			AABB aabb;
			unsigned int numChilds = 0;

			std::unique_ptr<Node> left, right; // In case it's a branch, i.e. numChilds == 0.
			size_t ptr = 0; // In case of a leaf store the index of the first child.
		};

		std::vector<const Object *> m_objects;
		Node m_root;
};

#endif // BVH_HPP_INCLUDED
