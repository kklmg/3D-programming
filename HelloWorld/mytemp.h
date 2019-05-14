#pragma once

//Duplicate checker
//--------------------------------------------------------
template<typename T>
class Checker
{
public:
	Checker() {}
	~Checker() {}

	bool Check(T &Data)
	{
		auto iter = map.find(Data);
		if (iter == map.end())
			return false;
		else return true;
	}
	bool Check_insert(T &Data)
	{
		auto iter = map.find(Data);
		if (iter == map.end())
		{
			map.insert(std::make_pair(T, map.size()));
			return false;
		}
		else return true;
	}
	bool Check_insert(T &Data, WORD &Getid)
	{
		auto iter = map.find(Data);
		if (iter == map.end())
		{
			Getid = map.size();
			map.insert(std::make_pair(Data, map.size()));
			return false;
		}
		else
		{
			Getid = iter->second;
			return true;
		}
	}
	void ExportData(std::vector<T>&vec)
	{
		vec.resize(map.size());


		auto Iter = map.begin();
		for (; Iter != map.end(); ++Iter)
		{
			vec[Iter->second] = Iter->first;
		}
	}
private:
	std::map<T, WORD> map;
};


