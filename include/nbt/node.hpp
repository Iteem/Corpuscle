#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <cstdint>
#include <map>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class Node
{
	public:
		virtual ~Node() = 0;
		virtual void print( std::string indentation = "" ) = 0;

		typedef std::shared_ptr<Node> Ptr;
};

template <typename T>
class ConcreteNode : public Node
{
	public:
		ConcreteNode( const T& value );
		virtual void print( std::string indentation = "" ) override;

		T data;
};

typedef ConcreteNode<int8_t> ByteNode;
typedef ConcreteNode<int16_t> ShortNode;
typedef ConcreteNode<int32_t> IntNode;
typedef ConcreteNode<int64_t> LongNode;
typedef ConcreteNode<float> FloatNode;
typedef ConcreteNode<double> DoubleNode;
typedef ConcreteNode<std::vector<int8_t>> ByteArrayNode;
typedef ConcreteNode<std::string> StringNode;
typedef ConcreteNode<std::list<Node::Ptr>> ListNode;
typedef ConcreteNode<std::map<std::string, Node::Ptr>> CompoundNode;
typedef ConcreteNode<std::vector<int32_t>> IntArrayNode;

template <typename T>
ConcreteNode<T>::ConcreteNode( const T& value ) :
	data( value )
{
}

template <typename T>
void ConcreteNode<T>::print( std::string indentation )
{
	std::cout << data;
}

template <>
void ByteNode::print( std::string indentation );

template <>
void ByteArrayNode::print( std::string indentation );

template <>
void IntArrayNode::print( std::string indentation );

template <>
void ListNode::print( std::string indentation );

template <>
void CompoundNode::print( std::string indentation );

#endif // NODE_HPP_INCLUDED
