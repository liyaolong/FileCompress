#pragma 
#include <vector>
using namespace std;

// Ð¡¶Ñ
template<class T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r; // operator<
	}
};

template<class T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l > r; // operator<
	}
};

template<class T, class Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* a, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			_arrays.push_back(a[i]);
		}

		// ½¨¶Ñ
		for(int i = (_arrays.size()-2)/2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		_arrays.push_back(x);
		AdjustUp(_arrays.size()-1);
	}

	void Pop()
	{
		assert(_arrays.size() > 0);
		swap(_arrays[0], _arrays[_arrays.size() - 1]);
		_arrays.pop_back();

		AdjustDown(0);
	}

	T& Top()
	{
		assert(_arrays.size() > 0);
		return _arrays[0];
	}

	bool Empty()
	{
		return _arrays.empty();
	}

	size_t Size()
	{
		return _arrays.size();
	}

	void AdjustDown(int root)
	{
		int child = root*2 + 1;
		// 	
		Compare com;
		while (child < _arrays.size())
		{

			if(child+1<_arrays.size() &&
				com(_arrays[child+1],_arrays[child]))
			{
				++child;
			}

			if(com(_arrays[child],_arrays[root]))
			{
				swap(_arrays[child], _arrays[root]);
				root = child;
				child = 2*root+1;
			}
			else
			{
				break;
			}
		}
	}

	void AdjustUp(int child)
	{
		int parent = (child-1)/2;

		while (child > 0)
		{
			if (Compare()(_arrays[child],_arrays[parent]))
			{
				swap(_arrays[parent], _arrays[child]);
				child = parent;
				parent = (child-1)/2;
			}
			else
			{
				break;
			}
		}
	}

public:
	vector<T> _arrays;
};

