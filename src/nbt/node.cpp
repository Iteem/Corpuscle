#include "node.hpp"

Node::~Node()
{
}

template <>
void ByteNode::print( std::string indentation )
{
	std::cout << static_cast<int>( data );
}

template <>
void ByteArrayNode::print( std::string indentation )
{
	std::cout << std::endl << indentation << "[" << std::endl;
	//for( auto i : data )
		//std::cout << indentation << "  " << static_cast<int>( i ) << std::endl;
	std::cout << indentation << "  " << data.size() << " entries." << std::endl;
	std::cout << indentation << "]" << std::endl;
}

template <>
void IntArrayNode::print( std::string indentation )
{
	std::cout << std::endl << indentation << "[" << std::endl;
	//for( auto i : data )
		//std::cout << indentation << "  " << i << std::endl;
	std::cout << indentation << "  " << data.size() << " entries." << std::endl;
	std::cout << indentation << "]" << std::endl;
}

template <>
void ListNode::print( std::string indentation )
{
	std::cout << std::endl << indentation << "{" << std::endl;
	for( auto node : data ){
		std::cout << indentation + "  ";
		node->print( indentation + "  " );
		std::cout << std::endl;
	}
	std::cout << indentation << "}" << std::endl;
}

template <>
void CompoundNode::print( std::string indentation )
{
	std::cout << std::endl << indentation << "{" << std::endl;
	for( auto pair : data ){
		std::cout << indentation + "  " << pair.first << ":";
		pair.second->print( indentation + "  " );
		std::cout << std::endl;
	}
	std::cout << indentation << "}" << std::endl;
}
