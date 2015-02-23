#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <map>
#include <string>
#include <utility>

#include <glm/glm.hpp>

#include "chunk.hpp"
#include "nbt.hpp"

class Map
{
	public:
		bool load( const std::string &path, glm::ivec2 min, glm::ivec2 max );

		std::pair<uint8_t, uint8_t> getBlock( glm::ivec3 pos ) const;

		bool isBlockTransparent( glm::ivec3 pos ) const;
		bool isBlockVisible( glm::ivec3 pos ) const;

	private:
		const Chunk *getChunk( glm::ivec3 pos ) const;
		glm::ivec3 getCoordinatesInChunk( glm::ivec3 pos ) const;

		typedef std::map<int, std::map<int, Chunk>> ChunkMap;
		ChunkMap m_chunks;
};


#endif // MAP_HPP_INCLUDED
