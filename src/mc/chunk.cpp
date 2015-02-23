#include "chunk.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "nbt.hpp"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

bool Chunk::load( const NBT& nbt )
{
	m_blocks.resize( 256 * 16 * 16, 0 );
	m_data.resize( 256 * 16 * 16, 0 );

	std::shared_ptr<ListNode> sections;
	std::shared_ptr<CompoundNode> compound;
	if( !( compound = std::dynamic_pointer_cast<CompoundNode>( nbt.node ) ) )
		return false;
	if( !( compound = std::dynamic_pointer_cast<CompoundNode>( compound->data[""] ) ) )
		return false;
	if( !( compound = std::dynamic_pointer_cast<CompoundNode>( compound->data["Level"] ) ) )
		return false;
	if( !( sections = std::dynamic_pointer_cast<ListNode>( compound->data["Sections"] ) ) )
		return false;

	for( auto node : sections->data ){
		if( !( compound = std::dynamic_pointer_cast<CompoundNode>( node ) ) ){
			return false;
		}
		int yOffset = std::dynamic_pointer_cast<ByteNode>( compound->data["Y"] )->data ;
		yOffset *= 16 * 16 * 16;
		auto blockIDs = std::dynamic_pointer_cast<ByteArrayNode>( compound->data["Blocks"] )->data;

		for( unsigned int y = 0; y < 16; ++y ){
			for( unsigned int z = 0; z < 16; ++z ){
				for( unsigned int x = 0; x < 16; ++x ){
					unsigned int blockPos = y * 16 * 16 + z * 16 + x;
					m_blocks[blockPos + yOffset] = blockIDs[blockPos];
				}
			}
		}
	}

	return true;
}

std::pair<uint8_t, uint8_t> Chunk::getBlock( glm::ivec3 pos ) const
{
	if( pos.x < 0 || pos.x >= 16 ||  pos.x < 0 || pos.y >= 256 || pos.z < 0 || pos.z >= 16){
		return std::make_pair( 0, 0 );
	}

	size_t index = pos.y * 16 * 16 + pos.z * 16 + pos.x;
	return std::make_pair( m_blocks[index], m_data[index] );
}
