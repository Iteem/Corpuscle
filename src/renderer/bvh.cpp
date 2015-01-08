#include "bvh.hpp"

#include <iostream>
#include <forward_list>

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
	std::pair<float, const Object *> collisionPair( inf, nullptr );

	std::forward_list<const Node*> nodeStack; // Stack of nodes which are still to check.
	nodeStack.emplace_front( &m_root );

	while( !nodeStack.empty() ){
		const Node *node = *(nodeStack.begin());
		nodeStack.pop_front();

		// Skip if an earlier intersection was already found (or there is no intersection at all).
		float t = node->aabb.intersect( ray );
		if( t >= collisionPair.first ){
			continue;
		}

		// Check actual objects for collision in leafs.
		if( node->isLeaf() ){
			for( unsigned int i = node->ptr; i < node->ptr + node->numChilds; ++i ){
				float tmp = m_objects[i]->intersect( ray );
				if( tmp < collisionPair.first && tmp > 0.f && m_objects[i] != prevObject ) {
					collisionPair.first = tmp;
					collisionPair.second = m_objects[i];
				}
			}
		}
		else {
			nodeStack.push_front( node->left.get() );
			nodeStack.push_front( node->right.get() );
		}

	}

	return collisionPair;
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
