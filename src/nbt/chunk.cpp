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


template<typename T>
T parse_value( std::vector<char>::const_iterator &it )
{
    std::vector<char> buf( it, it + sizeof( T ) );
    it += sizeof( T );
    std::reverse( buf.begin(), buf.end() );
    return *( reinterpret_cast<T *>( buf.data() ) );
}

bool Chunk::load( const std::string &path, int x, int z )
{
	// Construct region-file name.
	int regionX = std::floor( static_cast<float>( x ) / 32.f );
	int regionZ = std::floor( static_cast<float>( z ) / 32.f );

	x -= regionX * 32;
	z -= regionZ * 32;

	std::string filePath( path + "/region/r." + std::to_string(regionX ) + "." + std::to_string( regionZ ) + ".mca" );

	std::ifstream file( filePath, std::ios::binary );

	if( !file.is_open() ){
		std::cout << "Failed to open file: " << filePath << std::endl;
		return false;
	}

	// Look up the chunk in the location table.
	unsigned int pos = ( x % 32  + ( z % 32 ) * 32 ) * 4;
	file.seekg( pos );

	std::vector<char> buf(4);
	file.read( buf.data(), 4 );
	auto it = buf.cbegin();
	auto meta = parse_value<int32_t>( it );

	if( meta == 0 ){
		return false;
	}

	int offset = meta >> 8;
	int size = meta & 0xFF;

	// Load the chunk.
	file.seekg( offset * 4096 );
	file.read( buf.data(), 4 );
	it = buf.cbegin();
	size = parse_value<int32_t>( it );
	file.read( buf.data(), 1 );
	it = buf.cbegin();
	parse_value<int8_t>( it ); // The compression is always zlib...

	buf.resize( size );
	file.read( buf.data(), size );

	NBT nbt;
	nbt.loadFromMemory( buf );
	//nbt.printNBT();

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

std::pair<uint8_t, uint8_t> Chunk::getBlock( glm::uvec3 pos )
{
	size_t index = pos.y * 16 * 16 + pos.z * 16 + pos.x;

	if( index >= m_blocks.size() ){
		return std::make_pair( 0, 0 );
	}

	return std::make_pair( m_blocks[index], m_data[index] );
}
