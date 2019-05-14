#include"stdafx.h"
#include"Convert.h"


//------------------------------------
//String to Figure
//------------------------------------
int Convert::SzToInt(LPCSTR str)
{
	return atoi(str);
	/*int result;
	sscanf_s(str, "%d", &result);
	return result;*/
}
WORD Convert::SzToWord(LPCSTR str)
{
	return atoi(str);

	/*WORD result;
	sscanf_s(str, "%hu", &result);
	return result;*/
}
DWORD Convert::SzToDWord(LPCSTR str)
{
	return atoi(str);
	//DWORD result;
	//sscanf_s(str, "%u", &result);
	//return result;
}


float Convert::SzToFloat(LPCSTR str)
{
	return atof(str);

	/*float result;
	sscanf_s(str, "%f", &result);
	return result;*/
}
double Convert::SzToDouble(LPCSTR str)
{
	double result;
	sscanf_s(str, "%lf", &result);
	return result;
}

//------------------------------------
//Figure to String
//------------------------------------
std::string Convert::IntToStr(int data)
{
	char str[255];
	sprintf_s(str, "%d", data);
	return str;
}
std::string Convert::WordToStr(WORD data)
{
	char str[255];
	sprintf_s(str, "%hu\0", data);

	return str;
}
std::string Convert::DwordToStr(DWORD data)
{
	char str[255];
	sprintf_s(str, "%u", data);
	return str;
}

std::string Convert::FloatToStr(DWORD data)
{
	char str[255];
	sprintf_s(str, "%f", data);
	return str;
}
std::string Convert::DoubleToStr(DWORD data)
{
	char str[255];
	sprintf_s(str, "%lf", data);
	return str;
}


