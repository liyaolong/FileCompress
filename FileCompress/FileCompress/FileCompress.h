#pragma once
#include"HuffmanTree.h"
#include<algorithm>
struct Fileinfo
{
	unsigned char _ch;
	int _count;
	string _code;
	Fileinfo(unsigned char ch=0)
		:_ch(ch)
		,_count(0)
		{}
	bool operator<(const Fileinfo& info)
	{
		return this->_count < info._count;
	}
	Fileinfo operator+(const Fileinfo& info) 
	{
		Fileinfo tmp;
		tmp._count = this->_count + info._count;
		return tmp;
	}
	bool operator!=(const Fileinfo& info) const
	{
		return this->_count != info._count;
	}
};


class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0;i < 256;++i)
		{
			_info[i]._ch = i;
		}
	}
	bool Compress(const char* filename)
	{
		assert(filename);
		//打开待压缩文件
		FILE* fOut = fopen(filename, "rb");
		assert(fOut);
		//生成对应的Huffman编码
		char ch = fgetc(fOut);
		while (ch != EOF)
		{
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fOut);
		}

		HuffmanTree<Fileinfo> t;
		Fileinfo invalue;
		t.CreatHuffmanTree(_info, 256, invalue);
		_GenerateHuffmanCode(t.GetRootNode());
		//压缩文件
		string compressFile = filename;
		compressFile += ".compress";
		FILE* fpt = fopen(compressFile.c_str(), "w");
		assert(fpt);

		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);

		char inch = 0;
		int num = 0;
		while (ch != EOF)
		{
			for (size_t i = 0;i < _info[(unsigned char)ch]._code.size();++i)
			{

				inch <<= 1;
				if (_info[ch]._code[i] == '1')
					inch |= 1;

				if (++num % 8 == 0)
				{
					fputc(inch, fpt);
					inch = 0;
					num = 0;
				}
			}
			ch = fgetc(fOut);
		}
		if (num != 0)
		{
			inch <<= (8 - num);
			fputc(inch, fpt);
		}
		//写配置文件
		char str[128];
		string configcompress = filename;
		configcompress += ".config";
		FILE* finconfig = fopen(configcompress.c_str(), "w");
		assert(finconfig);

		for (size_t i = 0;i < 256;++i)
		{
			string chInfo;
			if (_info[i]._count > 0)
			{
				chInfo = _info[i]._ch;
				chInfo += ":";
				chInfo += itoa(_info[i]._count, str, 10);
				chInfo += "\n";
				fputs(chInfo.c_str(), finconfig);
			}
		}
		fclose(fOut);
		fclose(fpt);
		fclose(finconfig);
		return true;
	}

	
protected:
	//生成Huffman编码
	void _GenerateHuffmanCode(HuffmanTreeNode<Fileinfo>* root)
	{
		if (NULL == root)
			return;
		_GenerateHuffmanCode(root->_left);
		_GenerateHuffmanCode(root->_right);

		if (root->_left == NULL&&root->_right == NULL)
		{
			HuffmanTreeNode<Fileinfo>* cur = root;
			HuffmanTreeNode<Fileinfo>* parent = root->_parent;
			string& code = _info[cur->weight._ch]._code;
			while (parent)
			{
				if (parent->_left==cur)
					code += '0';
				if (parent->_right==cur)
					code += '1';

				cur = parent;
				parent = parent->_parent;
			}
			reverse(code.begin(), code.end());
		}
	}
private:
	Fileinfo _info[256];
};


void Test1()
{
	FileCompress fc;
	fc.Compress("Test.txt");
}