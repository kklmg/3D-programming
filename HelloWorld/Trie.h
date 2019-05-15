#pragma once

namespace STrie 
{
	//Default index Creator
	unsigned short InitIDTable(char *cr);

	//---------------------------- Trie Node -------------------------------//
	
	//TrieNode Basic
	//-------------------------------------------------------
	struct TrieNode
	{
		TrieNode() :term(0), children(nullptr) {}
		TrieNode(unsigned int slotct) :term(0), children(new TrieNode*[slotct]())
		{	
			memset(children, 0, sizeof(TrieNode*)*slotct);
		}

		bool term;
		TrieNode** children;
	};


	//TrieNode Counter
	//-------------------------------------------------------
	struct TrieNode_ct
	{
		TrieNode_ct():count(0), children(nullptr) {}

		unsigned int  count;
		TrieNode_ct* children;
	};


	//TrieNode Table
	//-------------------------------------------------------
	template<typename Value>
	struct TrieNode_t
	{
		TrieNode_t() :term(0), children(nullptr) {}
		TrieNode_t(unsigned int slotct) :term(0), children(new TrieNode_t*[slotct]())
		{
			memset(children, 0, sizeof(TrieNode_t*)*slotct);
		}

		bool term;
		TrieNode_t** children;
		Value data;
	};


	//----------------------------------- Trie --------------------------------------//
	

	//Basic Trie
	//----------------------------------------------------------
	class Trie
	{
	public:
		Trie(unsigned short(*fptr)(char*) = InitIDTable);
		~Trie();
	
		bool Insert(const char* str);
		bool Search(const char* str);
		bool Remove(const char* str);
		void Destroy();
		unsigned int size()const;

	private:

		void __DestroyHelper(TrieNode* Node);

		TrieNode* m_pRoot;				//root node
		unsigned int m_unSize;			//data count
		unsigned int m_unSlotCt;		//per branch slot count	
	
		char m_ARRTable[255];
	};

	



	//Table Trie
	//----------------------------------------------------------

	template<typename Value>
	class Trie_Table
	{
	public:
		Trie_Table(unsigned short(*fptr)(char*) = InitIDTable);
		~Trie_Table();

		bool Insert(const char* str, Value Data);
		bool Search(const char* str, Value &GetData);
		bool Remove(const char* str, Value &GetData);	
		void Destroy();
		unsigned int size()const;

	private:

		void __DestroyHelper(TrieNode_t<Value>* Node);

		TrieNode_t<Value>* m_pRoot;				//root node
		unsigned int m_unSize;					//data count
		unsigned int m_unSlotCt;				//per branch slot count	

		char m_ARRTable[255];
	};

	template<typename Value> 
	Trie_Table<Value>::Trie_Table<Value>(unsigned short(*fptr)(char*)) :
		m_unSlotCt(0),
		m_pRoot(nullptr),
		m_unSize(0)
	{
		memset(m_ARRTable, 255, sizeof(m_ARRTable));

		m_unSlotCt = fptr(m_ARRTable);

		m_pRoot = new TrieNode_t<Value>(m_unSlotCt);
	}

	template<typename Value>
	Trie_Table<Value>::~Trie_Table<Value>()
	{
		Destroy();
	}

	template<typename Value>
	bool Trie_Table<Value>::Insert(const char* str, Value Data)
	{
		TrieNode_t<Value>* pCurNode = m_pRoot;

		int index;
		for (int i = 0; str[i]; i++)
		{
			//compute branch index
			index = m_ARRTable[str[i]];

			//case: valid index
			if (index > -1 && index < m_unSlotCt)
			{
				//case: 
				if (!pCurNode->children[index])
					pCurNode->children[index] = new TrieNode_t<Value>(m_unSlotCt);

				//next
				pCurNode = pCurNode->children[index];
			}

			//case: InValid index
			else return false;
		}

		//case: str exist  /Insert Fail
		if (pCurNode->term) return false;

		//insert suceed
		else
		{
			++m_unSize;
			pCurNode->term = true;
			pCurNode->data = Data;
			return true;
		}
	}

	template<typename Value>
	bool Trie_Table<Value>::Search(const char* str, Value &GetData)
	{
		TrieNode_t<Value>* pCurNode = m_pRoot;

		int index;
		for (int i = 0; str[i]; i++)
		{
			//compute branch index
			index = m_ARRTable[str[i]];

			//case: out of max range
			if (index > -1 && index < m_unSlotCt)
			{
				if (pCurNode)
					pCurNode = pCurNode->children[index];
				//case: search failed
				else
					return false;
			}
		}

		//case: str exist 
		if (pCurNode && pCurNode->term)
		{
			GetData = pCurNode->data;
			return true;
		}
		else return false;
	}

	template<typename Value> 
	bool Trie_Table<Value>::Remove(const char* str, Value &GetData)
	{
		TrieNode_t<Value>* pCurNode = m_pRoot;

		int index;
		for (int i = 0; str[i]; i++)
		{
			//compute branch index
			index = m_ARRTable[str[i]];

			//case: out of max range
			if (index > -1 && index < m_unSlotCt)
			{
				if (pCurNode)
					pCurNode = pCurNode->children[index];
				else
					return false;
			}
			else return false;
		}

		//case: str exist  
		if (pCurNode->term)
		{
			--m_unSize;
			pCurNode->term = false;
			GetData = pCurNode->data;
			return true;
		}
		else return false;
	}

	template<typename Value>
	void Trie_Table<Value>::Destroy()
	{
		__DestroyHelper(m_pRoot);
		m_pRoot = nullptr;
	}

	template<typename Value>
	void Trie_Table<Value>::__DestroyHelper(TrieNode_t<Value>* Node)
	{
		if (!Node)return;

		int i;
		for (i = 0; i <m_unSlotCt; ++i)
		{
			__DestroyHelper(Node->children[i]);
		}
		delete[] Node->children;
		delete Node;
	}

	template<typename Value> 
	unsigned int Trie_Table<Value>::size()const
	{
		return m_unSize;
	}


}