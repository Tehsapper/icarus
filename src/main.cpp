#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sentry.hpp"

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		std::cout << "usage: icarus (file filename/camera index)" << std::endl;
		return 1;
	}

	Sentry* sentry = NULL;

	if(strcmp(argv[1], "file") == 0) sentry = new Sentry(argv[2]);
	else if(strcmp(argv[1], "camera") == 0)
	{
		char* end = NULL;
		int cam_index = strtol(argv[2], &end, 10);
		if(*end == 0) sentry = new Sentry(cam_index);
		else
		{
			std::cerr << "icarus: bad camera index argument \"" << argv[2] << "\"!" << std::endl;
			return 2;
		}
	} else 
	{
		std::cerr << "icarus: unknown option \"" << argv[1] << "\"." << std::endl;
		return 3;
	}

	sentry->loop();
	delete sentry;

	return 0;
}