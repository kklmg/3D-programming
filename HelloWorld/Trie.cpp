#include"stdafx.h"
#include"Trie.h"


unsigned short STrie::InitIDTable(char *cr)
{
	int MaxId = 0;
	for (int i = 'A'; i <= 'Z'; ++i, ++MaxId) 
	{	
		cr[i] = MaxId;
	}
	return MaxId;
}

STrie::Trie::Trie(unsigned short(*fptr)(char*)) :
	m_unSlotCt(0),
	m_pRoot(nullptr),
	m_unSize(0)
{
	memset(m_ARRTable, 255, sizeof(m_ARRTable));
	
	m_unSlotCt = fptr(m_ARRTable);

	m_pRoot = new TrieNode(m_unSlotCt);
}
STrie::Trie::~Trie()
{
	Destroy();
}

bool STrie::Trie::Insert(const char* str)
{
	TrieNode* pCurNode = m_pRoot;

	int index;
	for (int i = 0; str[i]; i++)
	{
		//compute branch index
		index = m_ARRTable[str[i]];

		//case: valid index
		if (index > -1 && index < m_unSlotCt)
		{
			//case: 
			if (!(pCurNode->children[index]))
				pCurNode->children[index] = new TrieNode(m_unSlotCt);

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
		return true;
	}
}
bool STrie::Trie::Search(const char* str)
{
	TrieNode* pCurNode = m_pRoot;

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
	return pCurNode->term;
}
bool STrie::Trie::Remove(const char* str)
{
	TrieNode* pCurNode = m_pRoot;

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
		return true;
	}
	else return false;
}


unsigned int STrie::Trie::size()const
{
	return m_unSize;
}

void STrie::Trie::Destroy()
{
	__DestroyHelper(m_pRoot);
	m_pRoot = nullptr;
}
void STrie::Trie::__DestroyHelper(TrieNode* Node)
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