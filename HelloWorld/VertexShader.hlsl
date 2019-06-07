
//
// Globals
//

matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

matrix MatrixPalette[255];


//
// Structures
//

// Input structure 
struct VS_INPUT
{
	vector position   : POSITION;
	float  Weight[3]  : BLENDWEIGHT;
	uint   Indicies   : BLENDINDICES;
};

// Output structure 
struct VS_OUTPUT
{
	vector position : POSITION;
};



VS_OUTPUT main(VS_INPUT input) : SV_Position
{
	// zero out members of output
	VS_OUTPUT output = (VS_OUTPUT)0;


	uint BoneID[4] = { 0,0,0,0 };

	//Decompose Indicies
	//BoneID[0] = (input.Indicies >> 24) & 0xff;
	//BoneID[1] = (input.Indicies >> 16) & 0xff;
	//BoneID[2] = (input.Indicies >> 8) & 0xff;
	//BoneID[3] = input.Indicies & 0xff;


	float weight4 = 1 - input.Weight[0] - input.Weight[1] - input.Weight[2];

	//blend
	output.position =
		mul(input.position,	MatrixPalette[BoneID[0]]) * weight4 +
		mul(input.position,	MatrixPalette[BoneID[1]]) * input.Weight[2] +
		mul(input.position,	MatrixPalette[BoneID[2]]) * input.Weight[1] +
		mul(input.position,	MatrixPalette[BoneID[3]]) * input.Weight[0];


	// transform to view space and project
	output.position = mul(output.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjMatrix);


	return output;
}












