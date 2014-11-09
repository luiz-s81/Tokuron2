// It calls the C++ program to generate many POV files
// It calls the POV-Ray program to generate the output (PNG files) from the code files
#include <stdio.h>
#include <iostream>

int main ()
{
	char command[255];
	float distance = -70.0f;
	int limit = 125;

	for (int i = 0; i < limit; i++)
	{
		sprintf_s(command, "Derezzed %.2f D:\\Luiz\\Projects\\cpp\\Tokuron2\\Tron\\derezzed%d.pov", distance, i);
		distance += 0.51f;
		system(command);
		std::cout << command << std::endl;
	}

	for (int i = 0; i < limit; i++)
	{
		sprintf_s(command, "\"C:\\Program Files\\POV-Ray\\v3.7\\bin\\pvengine.exe\" D:\\Luiz\\Projects\\cpp\\Tokuron2\\Tron\\derezzed%d.pov /exit /nr", i);
		system(command);
		std::cout << command << std::endl;
	}

	limit = 250;
	distance = 0.0f;
	for (int i = 125; i < limit; i++)
	{
		sprintf_s(command, "Derezzed2 %.2f D:\\Luiz\\Projects\\cpp\\Tokuron2\\Tron\\derezzed%d.pov", distance, i);
		distance += 0.4f;
		system(command);
		std::cout << command << std::endl;
	}

	for (int i = 125; i < limit; i++)
	{
		sprintf_s(command, "\"C:\\Program Files\\POV-Ray\\v3.7\\bin\\pvengine.exe\" D:\\Luiz\\Projects\\cpp\\Tokuron2\\Tron\\derezzed%d.pov /exit /nr", i);
		system(command);
		std::cout << command << std::endl;
	}


	return 0;
}