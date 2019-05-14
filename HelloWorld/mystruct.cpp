#include"stdafx.h"
#include"mystruct.h"


inline bool fequal(const float A, const float B)
{
	return fabs(A - B) < EPSINON;
}

STIndex3::STIndex3() 
{
	wIndex[0] = 0;
	wIndex[1] = 0;
	wIndex[2] = 0;
}
WORD& STIndex3::operator[](WORD id)
{
	if (id > 2)return wIndex[0];
	return wIndex[id];
}
bool STIndex3::operator<(const STIndex3 other)const
{
	if (wIndex[0] != other.wIndex[0]) return wIndex[0] < other.wIndex[0];
	else if (wIndex[1] != other.wIndex[1])  return wIndex[1] < other.wIndex[1];
	else if (wIndex[2] != other.wIndex[2])  return wIndex[2] < other.wIndex[2];
	else return false;
}
bool STIndex3::operator==(const STIndex3 other)const
{
	if (wIndex[0] == other.wIndex[0]
		&& wIndex[1] == other.wIndex[1]
		&& wIndex[2] == other.wIndex[2]) return true;
	else return false;
}
bool STIndex3::operator!=(const STIndex3 other)const
{
	return !(*this == other);
}


STFloat3::STFloat3()
{
	f3[0] = 0;
	f3[1] = 0;
	f3[2] = 0;
}
float&  STFloat3::operator[](WORD id)
{
	if (id > 2)return f3[0];
	return f3[id];
}
bool STFloat3::operator<(const STFloat3 other)const
{
	if (!fequal(f3[0],other.f3[0])) return f3[0] < other.f3[0];
	else if (!fequal(f3[1], other.f3[1]))  return f3[1] < other.f3[1];
	else if (!fequal(f3[2], other.f3[2]))  return f3[2] < other.f3[2];
	else return false;
}
bool STFloat3::operator==(const STFloat3 other)const
{
	if (fequal(f3[0], other.f3[0])
		&& fequal(f3[1], other.f3[1])
		&& fequal(f3[2], other.f3[2])) return true;
	else return false;
}
bool STFloat3::operator!=(const STFloat3 other)const
{
	return !(*this == other);
}
D3DXVECTOR3 STFloat3::vt3()const 
{
	return{f3[0],f3[1],f3[2]};
}
