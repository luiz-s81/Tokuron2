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
		"\tlocation <0.0, 0.8, " << distance << ">" << std::endl <<
		"\tright x*image_width/image_height" << std::endl <<
		"\tlook_at <0.0, 0.9, 0.0>" << std::endl <<
		"}" << std::endl <<
		"\nlight_source{ <0.0, 0.8, " << distance << "> color <1, 1, 1> }" << std::endl <<
		"\nbox{" << std::endl <<
		"\t<-0.35 , 0 , " << distance + 1.5 << ">" << std::endl <<
		"\t<-0.25 , .3 , " << distance << ">" << std::endl <<    
		"\ttexture { pigment { color <1, 1, 0> }}" << std::endl <<
		"}" << std::endl <<
		"\nbox{" << std::endl <<
		"\t<0.25 , 0 , " << (distance + 1.85) << ">" << std::endl <<
		"\t<0.35 , .3 , " << distance << ">"<< std::endl <<    
		"\ttexture { pigment { color <1, 0, 0> }}" << std::endl <<
		"}" << std::endl;

	fout.close();
	
	return 0;
}

		