#include "nbt.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <stdexcept>

#include <zlib.h>

std::vector<char> decompress(const std::vector<char> &src, int expectedSize = 1024){
	std::vector<char> dest;

	dest.resize(expectedSize);
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *) src.data();
    strm.avail_in = src.size();
    strm.next_out = (Bytef *) dest.data();
    strm.avail_out = dest.size();

	//use inflateInit2 with 15 + 32 as second parameter to decompress both zlib and gzip formats
    int ret = inflateInit2(&strm, 15 + 32);

    if (ret != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib.");
    }

    do {
        ret = inflate(&strm, Z_SYNC_FLUSH);

        if(ret != Z_OK && ret != Z_STREAM_END){
			throw std::runtime_error("Failed to decompress data.");
        }

        if (ret != Z_STREAM_END) {
            int oldSize = dest.size();
            dest.resize(dest.size() * 2);

            strm.next_out = (Bytef *)(dest.data() + oldSize);
            strm.avail_out = oldSize;
        }
    }
    while (ret != Z_STREAM_END);

    inflateEnd(&strm);

    //resize the vector to fit the data length exactly
    dest.shrink_to_fit();

    return dest;
}

template<typename T>
T parse_value( std::vector<char>::const_iterator &it )
{
    std::vector<char> buf( it, it + sizeof( T ) );
    it += sizeof( T );
    std::reverse( buf.begin(), buf.end() );
    return *( reinterpret_cast<T *>( buf.data() ) );
}

template<typename T>
std::vector<T> parse_array( int length, std::vector<char>::const_iterator &it )
{
    std::vector<T> vec;
    vec.reserve( length );

    for( int i = 0; i < length; ++i ){
        vec.push_back( parse_value<T>( it ) );
    }

    return vec;
}

std::string parse_name( std::vector<char>::const_iterator &it )
{
	auto length = parse_value<uint16_t>( it );
	std::string str( it, it + length );
	it += length;
	return str;
}

NBT::NBT()
{
}

NBT::~NBT()
{
}

bool NBT::loadFromFile( const std::string &path, bool compressed )
{
	std::vector<char> data;

	std::ifstream file( path, std::ios::binary );
	data = std::vector<char>( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );

	return loadFromMemory( data, compressed );
}

bool NBT::loadFromMemory( std::vector<char> data, bool compressed )
{
	if( compressed ){
		data = decompress( data );
	}

	std::map<std::string, Node::Ptr> map;

	auto it = data.cbegin();
	parse_value<uint8_t>( it ); // The first byte should always be Tag::Compound
	auto name = parse_name( it );
	auto child = parseNode( it );

	map.emplace( std::make_pair( name, child ) );
	node = std::make_shared<CompoundNode>( map );

	return true;
}

Node::Ptr NBT::parseTag( std::vector<char>::const_iterator &it, Tag tag )
{
	switch( tag ){
	case Tag::Byte:
		return std::make_shared<ByteNode>( parse_value<int8_t>( it ) );

	case Tag::Short:
		return std::make_shared<ShortNode>( parse_value<int16_t>( it ) );

	case Tag::Int:
		return std::make_shared<IntNode>( parse_value<int32_t>( it ) );

	case Tag::Long:
		return std::make_shared<LongNode>( parse_value<int64_t>( it ) );

	case Tag::Float:
		return std::make_shared<FloatNode>( parse_value<float>( it ) );

	case Tag::Double:
		return std::make_shared<DoubleNode>( parse_value<double>( it ) );

	case Tag::ByteArray:
		{
            auto length = parse_value<int32_t>( it );
            return std::make_shared<ByteArrayNode>( parse_array<int8_t>( length, it ) );

        }

	case Tag::String:
		{
            auto length = parse_value<uint16_t>( it );
            auto vec = parse_array<char>( length, it );
            return std::make_shared<StringNode>( std::string( vec.begin(), vec.end() ) );
        }

	case Tag::List:
		{
			auto newTag = static_cast<Tag>( parse_value<uint8_t>( it ) );
			auto length = parse_value<int32_t>( it );

			std::list<Node::Ptr> list;
			for( int i = 0; i < length; i++ ){
				list.push_back( parseTag( it, newTag ) );
			}
			return std::make_shared<ListNode>( list );
		}

	case Tag::Compound:
		return parseNode( it );

	case Tag::IntArray:
		{
			auto length = parse_value<int32_t>( it );
            return std::make_shared<IntArrayNode>( parse_array<int32_t>( length, it ) );
		}

	default:
		return Node::Ptr();
	}
}

Node::Ptr NBT::parseNode( std::vector<char>::const_iterator &it )
{
	std::map<std::string, Node::Ptr> map;
	while( true ){

		// Parse next tag.
		Tag tag = static_cast<Tag>( parse_value<uint8_t>( it ) );

		// Check for compound tag end.
		if( tag == Tag::End ){
			return std::make_shared<CompoundNode>( map );
		}

		// Parse name.
		auto name = parse_name( it );
		auto value = parseTag( it, tag );

		// Add the parsed value to the map.
		map.emplace( std::make_pair( name, value ) );
	}

}

void NBT::printNBT()
{
	node->print();
}
