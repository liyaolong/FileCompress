#pragma once
#include<assert.h>
#include"Heap.h"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode* _left;
	HuffmanTreeNode* _right;
	HuffmanTreeNode* _parent;
	T weight;
	HuffmanTreeNode(const T& x)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, weight(x)
	{}
};

template<class T>
class HuffmanTree
{
public:
	typedef HuffmanTreeNode<T> Node;
	template<class T>
	struct NodeCompare
	{
		bool operator() (
			Node* l, Node* r)
		{
			return l->weight < r->weight;
		}
	};
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(const T& hf)
	{}
	~HuffmanTree()
	{}


	void CreatHuffmanTree(T* a, size_t size,const T& invalue)
	{
		assert(a);
		Heap<Node*, NodeCompare<T>> MinHeap;
		for (size_t i = 0;i < size;++i)
		{
			if (a[i] != invalue)
			{
				Node* node = new Node(a[i]);
				MinHeap.Push(node);

			}
		}
		while (MinHeap.Size()>1)
		{
			Node* left = MinHeap.Top();
			MinHeap.Pop();
			Node* right = MinHeap.Top();
			MinHeap.Pop();

			Node* parent = new Node(left->weight + right->weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			MinHeap.Push(parent);
		}
		_root = MinHeap.Top();
	}
	Node* GetRootNode()
	{
		return _root;
	}
private:
	Node* _root;
};


void Test()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	HuffmanTree<int> t;
	t.CreatHuffmanTree(a, 10,-1);
}