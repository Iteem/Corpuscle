#include "bvh.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

BVH::BVH( std::vector<const Object *> objects )
{
	construct( objects );
}

void BVH::construct( std::vector<const Object *> objects )
{
	m_objects = objects;
	if( m_objects.size() > 0 ){
		m_root.construct( m_objects, 0, m_objects.size() - 1 );
	}
}

std::pair<float, const Object *> BVH::getCollision( const Ray &ray, const Object *prevObject ) const
{
	return m_root.getCollision( m_objects, ray, prevObject );
}

void BVH::print()
{
	m_root.print("");
}

BVH::Node::Node( std::vector<const Object *> &objects, size_t left, size_t right )
{
	construct( objects, left, right );
}

void BVH::Node::construct( std::vector<const Object *> &objects, size_t left, size_t right )
{
	// Set AABB for node.
	aabb = objects[left]->getAABB();
	for( size_t i = left + 1; i <= right; ++i ){
		aabb.extend( objects[i]->getAABB() );
	}

	if( right - left + 1 > ObjectsPerLeaf ){
		// Node is a branch.
		// Sort along the longest axis and split at median.
		glm::vec3 size = aabb.getSize();
		size_t axis = size.x > size.y && size.x > size.z ? 0 : size.y > size.z ? 1 : 0;
		std::sort( objects.begin() + left, objects.begin() + right,
		           [=](const Object *lhs, const Object *rhs){ return lhs->getAABB().getCenter()[axis] < rhs->getAABB().getCenter()[axis]; } );

		size_t middle = ( left + right ) / 2;
		this->left.reset( new Node( objects, left, middle ) );
		this->right.reset( new Node( objects, middle + 1, right ) );
	} else {
		// Node is a leaf;
		ptr = left;
		numChilds = right - left + 1;
	}
}

bool BVH::Node::isLeaf() const
{
	return numChilds != 0;
}

std::pair<float, const Object *> BVH::Node::getCollision( const std::vector<const Object *> &objects, const Ray &ray, const Object *prevObject ) const
{
	if( isLeaf() ){
		float t = inf;
		const Object *obj = nullptr;
		for( unsigned int i = 0; i < objects.size(); ++i ){
			float tmp = objects[i]->intersect( ray );

			if( tmp < t && tmp > 0.f && objects[i] != prevObject ) {
				t = tmp;
				obj = objects[i];
			}
		}
		return std::make_pair( t, obj );
	}
	else {
		auto leftPair = left->getCollision( objects, ray, prevObject );
		auto rightPair = left->getCollision( objects, ray, prevObject );

		return leftPair.first < rightPair.first ? leftPair : rightPair;
	}
}

void BVH::Node::print(std::string indent)
{
	if( isLeaf() ){
		std::cout << indent << glm::to_string( aabb.getSize() ) << " " << " Leaf: " << numChilds << std::endl;
	}
	else {
		std::cout << indent << "Branch:" << std::endl;
		left->print( indent + " " );
		right->print( indent + " " );
	}
}
