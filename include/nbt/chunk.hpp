#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include "nbt.hpp"

class Chunk
{
	public:
		Chunk();
		~Chunk();

		bool load( const NBT& nbt );

		std::pair<uint8_t, uint8_t> getBlock( glm::ivec3 pos ) const;

	private:
		std::vector<uint8_t> m_blocks;
		std::vector<uint8_t> m_data;
};


#endif // CHUNK_HPP_INCLUDED
