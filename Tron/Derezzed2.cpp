/*
	This file generate a .POV file according to the received parameters.
*/

#include <iostream>
#include <fstream>

float distance = 0.0f;

int main(int argc, char **argv){

	if(argc < 3) {
		std::cout << "Invalid number of parameters. " << std::endl;
		return 1;
	}

	distance = (float)atof(argv[1]);

	std::string outpath = argv[2];
	std::ofstream fout;
	fout.open(outpath, std::ios::out);

	if(!fout.is_open()){
		std::cout << "Error when trying to create the output file" << std::endl;
		return 1;
	}

	fout <<
		"#include \"myraster2.inc\""  << std::endl <<
		"\ncamera{" << std::endl <<
		"\tangle 80" << std::endl <<
		"\tlocation <" << distance << ", 0.8, -6.76>" << std::endl <<
		"\tright x*image_width/image_height" << std::endl <<
		"\tlook_at <200.0, 0.9, 0.0>" << std::endl <<
		"}" << std::endl <<
		"\nlight_source{ <" << distance <<", 0.8, -6.76> color <1, 1, 1> }" << std::endl <<
		"\nbox{" << std::endl <<
		"\t<" << (distance + 3.85) << ", 0, -5.01>" << std::endl <<
		"\t<0.35 , .3 , -4.91>"<< std::endl <<    
		"\ttexture { pigment { color <1, 0, 0> }}" << std::endl <<
		"}" << std::endl;

	fout.close();
	
	return 0;
}

		