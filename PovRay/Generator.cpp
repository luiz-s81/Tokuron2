#include <stdio.h>
#include <iostream>

int main1 ()
{
	char command[255];
	float index = 0.1f;
	int limit = 100;

	// It calls the C++ program to generate many POV files
	for (int i = 0; i < limit; i++)
	{
		sprintf_s(command, "PovraySurface %.3f D:\\Luiz\\Projects\\cpp\\Tokuron2\\PovRay\\gauss%d.pov", index, i);
		index += 0.015f;
		system(command);
		std::cout << command << std::endl;
	}

	// It calls the POV-Ray program to generate the output (PNG files) from the code files
	for (int i = 0; i < limit; i++)
	{
		sprintf_s(command, "\"C:\\Program Files\\POV-Ray\\v3.7\\bin\\pvengine.exe\" D:\\Luiz\\Projects\\cpp\\Tokuron2\\PovRay\\gauss%d.pov /exit /nr", i);
		system(command);
		std::cout << command << std::endl;
	}

	return 0;
}