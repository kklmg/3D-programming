#pragma once

namespace Convert
{
	//------------------------------------
	//String to Figure
	//------------------------------------
	int SzToInt(LPCSTR str);
	WORD SzToWord(LPCSTR str);
	DWORD SzToDWord(LPCSTR str);

	float SzToFloat(LPCSTR str);
	double SzToDouble(LPCSTR str);

	//------------------------------------
	//Figure to String
	//------------------------------------
	std::string IntToStr(int data);
	std::string WordToStr(WORD data);
	std::string DwordToStr(DWORD data);

	std::string FloatToStr(DWORD data);
	std::string DoubleToStr(DWORD data);
}

