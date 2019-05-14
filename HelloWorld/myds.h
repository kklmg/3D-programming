#pragma once
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<algorithm>
#include<iostream>
#include<unordered_map>

namespace MYDS 
{
	const unsigned int pn[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43 };
	const int Chdsize = 13;

	template<typename _Value, typename _Key=unsigned int>
	class HashNode
	{
	public:
		HashNode(_Key Key, _Value data)
			:boccupied(false)
		{}
		~HashNode() 
		{
		
		
		}

	private:
		
		_Key key;
		_Value value;
		bool boccupied;
	};

	template<typename Value,int Maxsize=26>
	class TrieNode 
	{
	public: 
		TrieNode() :term(0)
		{
			memset(children, 0, sizeof(children));
		}
		~TrieNode()
		{
			for (int i = 0; i < Maxsize; ++i)
			{
				if (children[i] != nullptr)
				{
					delete children[i];
					children[i] = nullptr;
				}

			}
		}

		bool term;
		Value Data;
		TrieNode* children[Maxsize];
	};



	template<typename Value, int MaxSize = 26>
	class Trie
	{
	public:
		Trie() 
		{
			pRoot = new TrieNode<Value>;
		}
		~Trie() 
		{
			delete pRoot;
		}

		bool Insert(const char* str, Value Data)
		{
			int index;
			TrieNode<Value>* pCurNode = pRoot;

			for (int i = 0; str[i]; i++)
			{
				index = str[i] - 'a';

				//case: out of max range
				if (index < 0 || index > MaxSize)
					return false;

				//case: char not exist
				if (!pCurNode->children[index])
					pCurNode->children[index] = new TrieNode();

				//next
				pCurNode = pCurNode->children[index];
			}

			//case: char exist  
			if (pCurNode->term)
				return false;

			//Mark insert
			else
			{
				pCurNode->term = true;
				pCurNode->Data = Data;
				return true;
			}
		}

		TrieNode<Value>* Search(const char* str, Value& GetValue)
		{
			int index;
			TrieNode<Value>* pCurNode = pRoot;

			for (int i = 0; str[i]; i++)
			{
				index = str[i] - 'a';

				//case: out of max range
				if (index < 0 || index > MaxSize)
					return nullptr;

				//case: char not exist
				if (!pCurNode->children[index])
					return nullptr;

				//next
				pCurNode = pCurNode->children[index];
			}

			//case: char exist  
			if (pCurNode->term)
			{
				GetValue = pCurNode->Data;
				return pCurNode;
			}
			else
				return nullptr;
		}

		bool Remove(const char* str, Value& GetValue)
		{
			TrieNode<Value>* pCurNode = Search(str,GetValue)

			if (pCurNode) 			
				pCurNode->term = false;
			else return false;
		}

	private:
		TrieNode<Value>* pRoot;
	private:
		void Destory(TrieNode* pRoot);
		void Print(TrieNode* pRoot);
	};
}
