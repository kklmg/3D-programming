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
}
