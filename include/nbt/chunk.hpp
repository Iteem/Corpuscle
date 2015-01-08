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

		bool load( const std::string &path, int x, int z );

		std::pair<uint8_t, uint8_t> getBlock( glm::vec3 pos );
		bool isBlockVisible( glm::vec3 pos );

	private:
		std::vector<uint8_t> m_blocks;
		std::vector<uint8_t> m_data;
};


#endif // CHUNK_HPP_INCLUDED
