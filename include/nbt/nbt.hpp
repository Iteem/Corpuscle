#ifndef NBT_HPP_INCLUDED
#define NBT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "node.hpp"

class NBT
{
	public:
		NBT();
		~NBT();

		enum class Tag : std::uint8_t
		{
			End = 0,
			Byte,
			Short,
			Int,
			Long,
			Float,
			Double,
			ByteArray,
			String,
			List,
			Compound,
			IntArray
		};

		bool loadFromFile( const std::string &path, bool compressed = true );
		bool loadFromMemory( std::vector<char> data, bool compressed = true );

		void printNBT();

		Node::Ptr node;

	private:
		Node::Ptr parseTag( std::vector<char>::const_iterator &it, Tag tag );
		Node::Ptr parseNode( std::vector<char>::const_iterator &it );
};

#endif // NBT_HPP_INCLUDED
