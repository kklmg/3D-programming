#include"stdafx.h"
#include"MyLexer.h"
#include"Convert.h"


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

	m_File.seekg(0, std::ios::cur);
	m_File.read(&m_CurChar, 1);
	return TRUE;
}

BOOL CMYLexer::Open(LPCSTR p_strFileName)
{
	//try open file
	m_File.open(p_strFileName);

	if (!m_File.is_open())return FALSE;

	//Init seek position
	//m_File.seekg(std::ios::beg);
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
	GetString.clear();
	//get next Char
	if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;

	while (m_CurChar == ' ' || m_CurChar == '\t' || m_CurChar == '\r' || m_CurChar == '\n')
	{
		if (!GetNextChar())	return s_CurToken_Dw = TOKEND_END;
	}

	// ------------------------------------------------------------
	// 2. BLOCK Ȯ��.
	// ------------------------------------------------------------
	// 1) '{'�ΰ��� Ȯ���Ѵ�.
	if (m_CurChar == '{')
	{
		GetString = m_CurChar;
		return	s_CurToken_Dw = TOKEND_BLOCK_START;
	}

	// 2) '}'�ΰ��� Ȯ���Ѵ�.
	else if (m_CurChar == '}')
	{
		GetString = m_CurChar;
		return	s_CurToken_Dw = TOKEND_BLOCK_END;
	}

	// ------------------------------------------------------------
	// 3. Reserved Word
	// ------------------------------------------------------------
	// - '*'�ΰ��� Ȯ���Ѵ�.
	//   ���� '*'�� �����ϴ� ���� Reserved word�̹Ƿ� �˻��Ѵ�!
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
		// �´� ���� ���� ã�� ���ߴ�. ���ǵ��� ���� TOKEN�̶�� �����ش�.
		else return	s_CurToken_Dw = TOKEND_NOTDEFINED;
	}


	// ------------------------------------------------------------
	// 4. ���������� Ȯ���Ѵ�.
	// ------------------------------------------------------------
	// - ���࿡ '0'���� '9'�����̰ų� '-'���ϰ� �پ� ������ ���ڶ�� �Ǵ��� �Ѵ�.
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
	// 5. String������ �˻��Ѵ�.
	// ------------------------------------------------------------
	// - '"' ����ǥ�� �����ϸ� �̰��� String�δ�!
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

	while (FindToken_Multi(2,TOKEND_BLOCK_START, TOKEND_BLOCK_END))
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
bool CMYLexer::FindToken_Multi(int num,...)
{
	bool finded = FALSE;

	int *seekType = new int[num];

	va_list ap;
	va_start(ap, num);
	for (int i = 0; i < num; i++)
	{
		seekType[i] = va_arg(ap, int);
	}
	va_end(ap);

	do
	{
		GetNextToken();

		if (s_CurToken_Dw == TOKEND_END)
		{
			SAFE_DELETE_ARRAY(seekType);
			return FALSE;
		}

		for (int i = 0; i < num; i++)
		{
			if (s_CurToken_Dw == seekType[i]) 
			{ 
				finded = TRUE;
				break;
			}
		}

	} while (!finded);

	SAFE_DELETE_ARRAY(seekType);
	
	return TRUE;


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
bool CMYLexer::FindToken_Multi_Until(int num, int stop, ...)
{
	bool finded = FALSE;
	

	int *seekType = new int[num];

	va_list ap;
	va_start(ap, stop);
	for (int i = 0; i < num; i++)
	{
		seekType[i] = va_arg(ap, int);
	}
	va_end(ap);     

	do
	{
		GetNextToken();
		if (s_CurToken_Dw == stop|| s_CurToken_Dw == TOKEND_END)
		{
			if (seekType != NULL) { delete[] seekType; seekType = NULL; }
			return FALSE;
		}

		for (int i = 0; i < num; i++)
		{
			if (s_CurToken_Dw == seekType[i]) { finded = TRUE; break; }
		}

	} while (finded == FALSE);

	if (seekType != NULL) { delete[] seekType; seekType = NULL; }
	return TRUE;


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