#pragma once
#include"HuffmanTree.h"
#include<algorithm>
struct Fileinfo
{
	unsigned char _ch;
	long _count;
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
		FILE* fOutfile = fopen(filename, "rb");
		assert(fOutfile);
		//生成对应的Huffman编码
		int num = 0;
		char ch = fgetc(fOutfile);
		while (ch != EOF)
		{
			//if (ch != '\r')
			{
				num++;
				_info[(unsigned char)ch]._count++;
			}
			ch = fgetc(fOutfile);
		}

		HuffmanTree<Fileinfo> t;
		Fileinfo invalue;
		t.CreatHuffmanTree(_info, 256, invalue);
		_GenerateHuffmanCode(t.GetRootNode());
		//压缩文件
		string compressFile = filename;
		compressFile += ".compress";
		FILE* fIncompress = fopen(compressFile.c_str(), "wb");
		assert(fIncompress);

		fseek(fOutfile, 0, SEEK_SET);
		ch = fgetc(fOutfile);

		char inch = 0;
		int index = 0;
		while (ch != EOF)
		{
			string& code = _info[(unsigned char)ch]._code;
			for (size_t i = 0;i < code.size();++i)
			{
				inch <<= 1;
				if (code[i] == '1')
					inch |= 1;

				if (++index==8)
				{
					fputc(inch, fIncompress);
					inch = 0;
					index = 0;
				}
			}
			ch = fgetc(fOutfile);
		}
		if (index != 0)
		{
			inch <<= (8 - index);
			fputc(inch, fIncompress);
		}
		//写配置文件
		char str[128];
		string configcompress = filename;
		configcompress += ".config";
		FILE* finconfig = fopen(configcompress.c_str(), "w");
		assert(finconfig);
		string numInfo;
		numInfo += itoa(num, str, 10);
		numInfo += "\n";
		fputs(numInfo.c_str(), finconfig);
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
		fclose(fOutfile);
		fclose(fIncompress);
		fclose(finconfig);
		return true;
	}

	//解压缩
	bool UnCompress(const char* Filename)
	{
		//读取配置文件
		string configFile = Filename;
		configFile += ".config";
		FILE* FOutconFigfile = fopen(configFile.c_str(), "r");
		assert(FOutconFigfile);

		string line;
		_Readline(FOutconFigfile, line);
		int num = atoi(line.c_str());
		line.clear();
		while (_Readline(FOutconFigfile, line))
		{
			

			if (!line.empty())
			{
				unsigned char ch = line[0];
				_info[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
			else
			{
				line = "\n";
			}
		}

		//重建Huffman树
		HuffmanTree<Fileinfo> t;
		Fileinfo invalue;
		t.CreatHuffmanTree(_info, 256, invalue);
		HuffmanTreeNode<Fileinfo>* root = t.GetRootNode();

		//解压缩
		string compressFile = Filename;
		compressFile += ".compress";
		FILE* fOutcompress = fopen(compressFile.c_str(), "r");
		assert(fOutcompress);

		string uncompressFile = Filename;
		uncompressFile += ".uncompress";
		FILE* fInuncompress = fopen(uncompressFile.c_str(), "w");
		assert(fInuncompress);

		char ch = fgetc(fOutcompress);
		HuffmanTreeNode<Fileinfo>* cur = root;

		int index = 8;
		while (1)
		{
			--index;
			if (ch&(1 << index))
			{
				cur = cur->_right;
			}
			else
				cur = cur->_left;
			if (cur->_left == nullptr&&cur->_right == nullptr)
			{
				fputc(cur->weight._ch, fInuncompress);
				if (cur->weight._ch == 'i')
				{
					int i = 0;
				}
				cur = root;
				if (--num == 0)
					break;
			}
			if (index == 0)
			{
				ch = fgetc(fOutcompress);
				index = 8;
			}	
		}
		fclose(FOutconFigfile);
		fclose(fOutcompress);
		fclose(fInuncompress);

		return true;
	}
	
protected:
	bool _Readline(FILE* filename, string& line)
	{
		char ch = fgetc(filename);
		if (ch == EOF)
			return false;
		while (ch != EOF&&ch != '\n')
		{
			line += ch;
			ch = fgetc(filename);
		}
		return true;
	}
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
	fc.UnCompress("Test.txt");
}