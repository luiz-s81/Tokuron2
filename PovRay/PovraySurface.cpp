/*
	This file generate a .POV file according to the received parameters.
*/
#include <iostream>
#include <fstream>
#include "vector3f.h"

float g_sigma = 1.0;

float f(float x, float y){
	// Gaus fuction
	float s2 = g_sigma * g_sigma;
	float result = 1.0f / sqrt(2.0f * 3.1416 * s2) * exp(-(x * x + y* y) / (2.0f *s2));

	return result;
}

int main(int argc, char** argv){

	if(argc < 3) {
		std::cout << "Invalid number of arguments" << std::endl;
		return 1; // error check
	}
	
	g_sigma = (float)atof(argv[1]);
	
	std::string outpath = argv[2];
	std::ofstream fout;
	fout.open(outpath, std::ios::out);
	
	if(!fout.is_open()) {
		std::cout << "Error when trying to create the outuput file" << std::endl;
		return 1;
	}

	int div = 200;
	Vector3f* pos = new Vector3f[div*div];

	// Setup mesh
	for (int i = 0; i < div; i++) // loop for x
	{
		for (int j = 0; j < div; j++)
		{
			// x, y = [-5, -5]
			float x = 10.0f * (float)i / (float)(div - 1) - 5.0f;
			float y = 10.0f * (float)j / (float)(div - 1) - 5.0f;

			// Compute z value according to gauss function
			float z = f(x, y);
			pos[div * i + j] = Vector3f(x, y, z);
		}
	}

	fout <<
		"camera{" << std::endl <<
		"\tlocation <0, -5, 5>" << std::endl <<
		"\tright <-1.33, 0, 0>" << std::endl <<
		"\tup <0,0,1>" << std::endl <<
		"\tangle 30" << std::endl <<
		"\tlook_at <0, 0,0>" << std::endl <<
		"}" << std::endl <<
		"\nlight_source {<5, -5, 5> color rgb <1, 1, 1>}" << std::endl;

	// Write object as mesh
	fout << "\nobject {" << std::endl;
	fout << "\tmesh {" << std::endl;

	// Write triangles
	for (int i = 0; i < div - 1; i++) // loop for x
	{
		for (int j = 0; j < div - 1; j++) // loop for y
		{
			Vector3f p0 = pos[div * (i + 0) + (j + 0)];
			Vector3f p1 = pos[div * (i + 1) + (j + 0)];
			Vector3f p2 = pos[div * (i + 0) + (j + 1)];
			Vector3f p3 = pos[div * (i + 1) + (j + 1)];

			// Write 2 triangles (0, 1, 3) and (0, 3, 2)
			fout << "\t\ttriangle {";
			fout << "<" << p0.x << ", " << p0.y << ", " << p0.z << "> ";
			fout << "<" << p1.x << ", " << p1.y << ", " << p1.z << "> ";
			fout << "<" << p3.x << ", " << p3.y << ", " << p3.z << ">}" << std::endl;

			fout << "\t\ttriangle {";
			fout << "<" << p0.x << ", " << p0.y << ", " << p0.z << "> ";
			fout << "<" << p3.x << ", " << p3.y << ", " << p3.z << "> ";
			fout << "<" << p2.x << ", " << p2.y << ", " << p2.z << ">}" << std::endl;
		}
	}

	fout << "\t}" << std::endl;
	fout << "\n\tpigment { color <0, 1, 0>}" << std::endl;
	fout << "\tfinish { diffuse 1.0 ambient 0.3 }"<< std::endl;
	fout << "}"<< std::endl;
	fout.close();

	delete[] pos;
	return 0;
}

