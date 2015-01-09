#include "map.hpp"

#include <algorithm>
#include <fstream>
#include <vector>

#include "nbt.hpp"

template<typename T>
T parse_value( std::vector<char>::const_iterator &it )
{
    std::vector<char> buf( it, it + sizeof( T ) );
    it += sizeof( T );
    std::reverse( buf.begin(), buf.end() );
    return *( reinterpret_cast<T *>( buf.data() ) );
}

bool Map::load( const std::string &path, glm::ivec2 min, glm::ivec2 max )
{
	for( int x = min.x; x <= max.x ; ++x ){
		for( int z = min.y; z <= max.y ; ++z ){
			// Construct region-file name.
			int regionX = std::floor( static_cast<float>( x ) / 32.f );
			int regionZ = std::floor( static_cast<float>( z ) / 32.f );

			int chunkX = x - regionX * 32;
			int chunkZ = z - regionZ * 32;

			std::string filePath( path + "/region/r." + std::to_string(regionX ) + "." + std::to_string( regionZ ) + ".mca" );

			std::ifstream file( filePath, std::ios::binary );

			if( !file.is_open() ){
				std::cout << "Failed to open file: " << filePath << std::endl;
				return false;
			}

			// Look up the chunk in the location table.
			unsigned int pos = ( chunkX % 32  + ( chunkZ % 32 ) * 32 ) * 4;
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

			if( !m_chunks[x][z].load( nbt ) ){
				std::cout << "Failed to load chunk." << std::endl;
				return false;
			}
		}
	}

	return true;
}

std::pair<uint8_t, uint8_t> Map::getBlock( glm::ivec3 pos ) const
{
	auto chunk = getChunk( pos );
	if( chunk == nullptr ){
		return { 0, 0 };
	}

	return chunk->getBlock( getCoordinatesInChunk( pos ) );
}

bool Map::isBlockTransparent( glm::ivec3 pos ) const
{
	// If a block is not loaded handle it as non transparent block.
	// This way we don't need to load the blocks at the border.
	if( getChunk( pos ) == nullptr || pos.y < 0 || pos.y > 255 )
		return false;

	return getBlock( pos ).first == 0;
}

bool Map::isBlockVisible( glm::ivec3 pos ) const
{

	if( getBlock( pos ).first == 0 ){
		return false;
	}

	for( int x = -1; x <= 1; ++x ){
		for( int y = -1; y <= 1; ++y ){
			for( int z = -1; z <= 1; ++z ){
				if( isBlockTransparent( pos + glm::ivec3( x, y, z ) ) ){
					return true;
				}
			}
		}
	}

	return false;
}


const Chunk *Map::getChunk( glm::ivec3 pos ) const
{
	glm::ivec2 chunkPos( std::floor( static_cast<float>( pos.x ) / 16.f ), std::floor( static_cast<float>( pos.z ) / 16.f ) );

	auto it = m_chunks.find( chunkPos.x );
	if( it == m_chunks.end() ){
		return nullptr;
	}

	auto chunk = it->second.find( chunkPos.y );
	if( chunk == it->second.end() ){
		return nullptr;
	}

	return &( chunk->second );
}

glm::ivec3 Map::getCoordinatesInChunk( glm::ivec3 pos ) const
{
	glm::ivec3 vec( pos.x % 16, pos.y, pos.z % 16 );
	return { vec.x < 0 ? vec.x + 16 : vec.x, pos.y, vec.z < 0 ? vec.z + 16 : vec.z };
}
