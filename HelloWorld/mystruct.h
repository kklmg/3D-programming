#pragma once


//constant value
const float EPSINON = 0.0001f;

//-----------------------------------
//compare float
//-----------------------------------
bool fequal(const float A, const float B);




struct STIndex3
{
	STIndex3();

	WORD& operator[](WORD id);
	bool operator<(const STIndex3 other)const;
	bool operator==(const STIndex3 other)const;
	bool operator!=(const STIndex3 other)const;

	WORD wIndex[3];
};

struct STFloat3
{
	STFloat3();

	float& operator[](WORD id);
	bool operator<(const STFloat3 other)const;
	bool operator==(const STFloat3 other)const;
	bool operator!=(const STFloat3 other)const;
	D3DXVECTOR3 vt3()const;


	float f3[3];
};