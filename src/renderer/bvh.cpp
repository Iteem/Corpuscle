#include "bvh.hpp"

#include <iostream>
#include <forward_list>

#include <glm/gtx/string_cast.hpp>

AABB calculateAABB( std::vector<const Object *> &objects, size_t left, size_t right )
{
	AABB aabb( objects[left]->getAABB() );
	for( size_t i = left + 1; i < right; ++i ){
		aabb.extend( objects[i]->getAABB() );
	}
	return aabb;
}

BVH::BVH( std::vector<const Object *> objects )
{
	construct( objects );
}

void BVH::construct( std::vector<const Object *> objects )
{
	m_objects = objects;
	if( m_objects.size() > 0 ){
		m_root.construct( m_objects, 0, m_objects.size() );
	}
}

std::pair<float, const Object *> BVH::getCollision( const Ray &ray, const Object *prevObject ) const
{
	std::pair<float, const Object *> collisionPair( inf, nullptr );

	std::forward_list<const Node*> nodeStack; // Stack of nodes which are still to check.
	nodeStack.emplace_front( &m_root );

	glm::vec3 invdir = 1.f / ray.direction;

	while( !nodeStack.empty() ){
		const Node *node = *(nodeStack.begin());
		nodeStack.pop_front();

		// Skip if an earlier intersection was already found (or there is no intersection at all).
		float t = node->aabb.intersect( ray.origin, invdir );
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
	for( size_t i = left + 1; i < right; ++i ){
		aabb.extend( objects[i]->getAABB() );
	}

	// Calculate best possible split, based on a SAH.
	float inverseParentSurface = 1.f / aabb.getSurface();
	size_t numObjects = right - left;

	size_t bestAxis = 3;
	size_t splitIndex = 0;
	float cost = numObjects;

	// Every axis.
	for( size_t axis = 0; axis < 3; ++axis ){
		std::sort( objects.begin() + left, objects.begin() + right,
		           [axis](const Object *lhs, const Object *rhs){ return lhs->getAABB().getCenter()[axis] < rhs->getAABB().getCenter()[axis]; } );

		// Calculate and cache the AABB areas.
		std::vector<AABB> aabbLeft( numObjects ), aabbRight( numObjects );
		aabbLeft[0] = objects[left]->getAABB();
		aabbRight[0] = objects[right - 1]->getAABB();
		for( size_t i = 1; i < numObjects; ++i ){
			aabbLeft[i] = aabbLeft[i - 1];
			aabbLeft[i].extend( objects[left + i]->getAABB() );

			aabbRight[i] = aabbRight[i - 1];
			aabbRight[i].extend( objects[right - 1 - i]->getAABB() );
		}

		// Iterate over all possible splits and save if better than any found yet.
		for( size_t i = 1; i < numObjects; ++i ){
			float newCost = 2.f + ( aabbLeft[i - 1].getSurface() * i + aabbRight[numObjects - i].getSurface() * ( numObjects - i ) ) * inverseParentSurface;
			if( newCost < cost ){
				cost = newCost;
				bestAxis = axis;
				splitIndex = left + i;
			}
		}
	}

	if( bestAxis < 3 ){
		// Node is a branch.
		std::sort( objects.begin() + left, objects.begin() + right,
		           [bestAxis](const Object *lhs, const Object *rhs){ return lhs->getAABB().getCenter()[bestAxis] < rhs->getAABB().getCenter()[bestAxis]; } );

		this->left.reset( new Node( objects, left, splitIndex ) );
		this->right.reset( new Node( objects, splitIndex, right ) );
	} else {
		// Node is a leaf.
		ptr = left;
		numChilds = numObjects;
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
