#pragma once
#include"stdafx.h"
#include<fstream>
#include<map>
#include"TokenDefine.h"
#include<unordered_set>

//-------------------------------
//Class My Lexer
//-------------------------------
class CMYLexer
{
public:
	CMYLexer();
	CMYLexer(CTokenDefine*);
	virtual ~CMYLexer();

	static std::string s_CurToken_Str;
	static DWORD s_CurToken_Dw;

public:
	BOOL Open(LPCSTR p_strFileName);
	void Close();
	BOOL Reset();

	void SetTokenDef(CTokenDefine* def);

	DWORD GetNextToken(std::string& GetString = s_CurToken_Str);

	void MovetoBlockStart();
	void MovetoBlockEnd();
	void SkipCurBlock();


	bool FindToken(int FindToken);
	bool FindToken(int FindToken1, int FindToken2);

	bool FindToken_Until(int FindToken, int stop);
	bool FindToken_InBlock(int FindToken);
	
	bool FindToken_UseTable(std::unordered_set<DWORD>& table);
	bool FindToken_UseTable_Until(int stop, std::unordered_set<DWORD>& table);


	bool GetInt(int &Get);
	bool GetWORD(WORD &Get);
	bool GetDWORD(DWORD &Get);
	bool GetFloat(float &Get);
	bool GetString(std::string& Get);

public:
	std::string m_CurStr;
	WORD m_wCurToken;

private:
	bool GetNextChar();

	std::ifstream m_File;

	char m_strBuff[255];

	char m_CurChar;


	CTokenDefine* m_pDef;
};


