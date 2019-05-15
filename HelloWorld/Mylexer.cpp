#include"stdafx.h"
#include"MyLexer.h"
#include"Convert.h"
#include<unordered_set>

std::string CMYLexer::s_CurToken_Str;
DWORD CMYLexer::s_CurToken_Dw;
//-------------------------------------
//Class My Lexer
//-------------------------------------
CMYLexer::CMYLexer() :m_pDef(NULL)
{
}
CMYLexer::CMYLexer(CTokenDefine* def) : m_pDef(def)
{
}

CMYLexer::~CMYLexer()
{
	Close();
}

void CMYLexer::SetTokenDef(CTokenDefine* def)
{
	m_pDef = def;
}

bool CMYLexer::GetNextChar()
{
	if (m_File.peek() == EOF) return FALSE;

	m_File.read(&m_CurChar, 1);
	return TRUE;
}

BOOL CMYLexer::Open(LPCSTR p_strFileName)
{
	//try open file
	m_File.open(p_strFileName);

	if (!m_File.is_open())return FALSE;
}

void CMYLexer::Close()
{
	if (m_File.is_open())
	{
		m_File.close();
	}
}

BOOL CMYLexer::Reset()
{
	m_File.seekg(std::ios::beg);
	return true;
}

DWORD CMYLexer::GetNextToken(std::string& GetString)
{
	//get next Char
	if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;

	while (m_CurChar == ' ' || m_CurChar == '\t' || m_CurChar == '\r' || m_CurChar == '\n')
	{
		if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;
	}

	// ------------------------------------------------------------
	// 2. BLOCK 확인.
	// ------------------------------------------------------------
	// 1) '{'인가를 확인한다.
	if (m_CurChar == '{')
	{
		GetString = m_CurChar;
		return	s_CurToken_Dw = TOKEND_BLOCK_START;
	}

	// 2) '}'인가를 확인한다.
	else if (m_CurChar == '}')
	{
		GetString = m_CurChar;
		return	s_CurToken_Dw = TOKEND_BLOCK_END;
	}

	// ------------------------------------------------------------
	// 3. Reserved Word
	// ------------------------------------------------------------
	// - '*'인가를 확인한다.
	//   만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if (m_CurChar == '*')
	{
		//no defination seted
		if (m_pDef == NULL) return s_CurToken_Dw = TOKEND_NOTDEFINED;

		int value;
		int	p_StringPoint = 0;

		while (m_CurChar != ' ' && m_CurChar != '\t' && m_CurChar != '\r'
			&& m_CurChar != '\n' && m_CurChar != '"' && m_CurChar != '{' && m_CurChar != '}')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;

		if (m_pDef->SearchData(m_strBuff, value))
			return s_CurToken_Dw = value;
		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		else return	s_CurToken_Dw = TOKEND_NOTDEFINED;
	}


	// ------------------------------------------------------------
	// 4. 숫자인지를 확인한다.
	// ------------------------------------------------------------
	// - 만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
	else if (m_CurChar >= '0' && m_CurChar <= '9' || m_CurChar == '-')
	{
		int		p_StringPoint = 0;

		while ((m_CurChar >= '0' && m_CurChar <= '9') || m_CurChar == '-' || m_CurChar == '.')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;
		return	s_CurToken_Dw = TOKEND_NUMBER;
	}


	// ------------------------------------------------------------
	// 5. String인지를 검사한다.
	// ------------------------------------------------------------
	// - '"' 따음표로 시작하면 이것은 String인다!
	else if (m_CurChar == '"')
	{
		int		p_StringPoint = 0;

		if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;


		while (m_CurChar != '"')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;
		return	s_CurToken_Dw = TOKEND_STRING;
	}
	//GetString = m_CurChar;
	// Error) 
	return	s_CurToken_Dw = TOKEND_NOTDEFINED;
}

void CMYLexer::MovetoBlockStart()
{
	do
	{
		if (s_CurToken_Dw == TOKEND_END)return;
		GetNextToken();
	} while (s_CurToken_Dw != TOKEND_BLOCK_START);
}
void CMYLexer::MovetoBlockEnd()
{
	do
	{
		if (s_CurToken_Dw == TOKEND_END)return;
		GetNextToken();
	} while (s_CurToken_Dw != TOKEND_BLOCK_END);
}
void CMYLexer::SkipCurBlock()
{
	int blk_S = 1;
	int blk_E = 0;

	while (FindToken(TOKEND_BLOCK_START, TOKEND_BLOCK_END))
	{
		switch (s_CurToken_Dw)
		{
		case TOKEND_BLOCK_START:blk_S += 1; break;
		case TOKEND_BLOCK_END:	blk_E += 1; break;
		default:
			break;
		}
		if (blk_S == blk_E) return;
	}
}


bool CMYLexer::FindToken(int FindToken)
{
	do
	{
		GetNextToken();
		if (s_CurToken_Dw == FindToken)return TRUE;
	} while (s_CurToken_Dw != TOKEND_END);
	return FALSE;
}
bool CMYLexer::FindToken(int FindToken1, int FindToken2)
{
	do
	{
		GetNextToken();
		if (s_CurToken_Dw == FindToken1 || s_CurToken_Dw == FindToken2)return TRUE;
	} while (s_CurToken_Dw != TOKEND_END);
	return FALSE;
}

bool CMYLexer::FindToken_Until(int FindToken, int stop)
{
	do
	{
		GetNextToken();
		if (s_CurToken_Dw == FindToken)return TRUE;
		if (s_CurToken_Dw == stop)return FALSE;
	} while (s_CurToken_Dw != TOKEND_END);
	return FALSE;
}
bool CMYLexer::FindToken_InBlock(int FindToken)
{
	return FindToken_Until(FindToken, TOKEND_BLOCK_END);
}

bool CMYLexer::FindToken_UseTable(std::unordered_set<DWORD>& table)
{
	do
	{
		//next
		GetNextToken();

		if (table.find(s_CurToken_Dw) != table.end())
			return true;

	} while (s_CurToken_Dw != TOKEND_END);

	return FALSE;
}
bool CMYLexer::FindToken_UseTable_Until(int stop, std::unordered_set<DWORD>& table) 
{
	do
	{
		//next
		GetNextToken();

		if (table.find(s_CurToken_Dw) != table.end())
			return true;

	} while (s_CurToken_Dw != TOKEND_END && s_CurToken_Dw!= stop);

	return FALSE;
}


bool CMYLexer::GetInt(int &Get)
{
	GetNextToken();
	if (s_CurToken_Dw == TOKEND_END)return FALSE;
	int Data = Convert::SzToInt(s_CurToken_Str.data());
	Get = Data;

	return TRUE;
}

bool CMYLexer::GetWORD(WORD &Get)
{
	GetNextToken();
	if (s_CurToken_Dw == TOKEND_END)return FALSE;
	WORD Data = Convert::SzToWord(s_CurToken_Str.data());
	Get = Data;
	return TRUE;
}

bool CMYLexer::GetDWORD(DWORD &Get)
{
	GetNextToken();
	if (s_CurToken_Dw == TOKEND_END)return FALSE;
	WORD Data = Convert::SzToDWord(s_CurToken_Str.data());
	Get = Data;
	return TRUE;
}

bool CMYLexer::GetFloat(float &Get)
{
	GetNextToken();
	if (s_CurToken_Dw == TOKEND_END)return FALSE;
	float Data = Convert::SzToFloat(s_CurToken_Str.data());
	Get = Data;

	return TRUE;
}

bool CMYLexer::GetString(std::string& Get)
{
	GetNextToken();
	if (s_CurToken_Dw == TOKEND_END)return FALSE;
	Get = s_CurToken_Str;
	return TRUE;
}