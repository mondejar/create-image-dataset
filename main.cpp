/*

Given an input and output directory this code generates X random transforms for each image of the input directory.
The patch size used is fixed to the same as the images at input directory.

Example of use: ./create_dataset path_input path_out X

----------------------------------------------------------------------------

Author: Victor Manuel Mondejar Guerra
Date: 12/2015
University of Cordoba
*/

#include "transformer_patch.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <algorithm>


int get_files_dir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	if(strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


int main(int argc, char *argv[])
{

	if(argc < 4 || argc > 4)
	{
		std::cout<<"Help usage:\n ./create_dataset input_dir output_dir N_transform"<< std::endl;
		return 0;
	}

	int N;
	std::string input_dir = argv[1];
	std::string output_dir = argv[2];
	N = atoi(argv[3]);

	std::cout<<"Input dir: "<< input_dir <<
			 "\nOutput dir: "<< output_dir <<
			 "\nN transform: "<< N << std::endl;

    std::vector<std::string> patch_names = std::vector<std::string>();
    get_files_dir(input_dir, patch_names);

	std::stringstream full_patch_name;
	std::stringstream output_patch_name;

	Transformer_patch transformer;
	std::vector<char> transforms_order;
	transforms_order.push_back('b');
	transforms_order.push_back('d');
	transforms_order.push_back('a');
	transforms_order.push_back('n');
	transforms_order.push_back('o');

    for (unsigned int i = 0; i < patch_names.size(); i++) 
    {//for each patch
    	
    	std::cout<< patch_names[i] << std::endl;
		full_patch_name.str("");
		full_patch_name.clear();
		full_patch_name << input_dir << patch_names[i];

    	cv::Mat patch;
    	patch = cv::imread(full_patch_name.str(), CV_LOAD_IMAGE_GRAYSCALE);

		output_patch_name << output_dir << patch_names[i];
	   	//Saves the original patch at output_dir
		cv::imwrite(output_patch_name.str(), patch);



    	for(int t = 0; t < N; t++)
    	{
    		std::cout<< " T "<< t << std::endl;
			output_patch_name.str("");
			output_patch_name.clear();

			transformer.setInputPatch(patch);
			
			//Select a random order for each transform
			std::random_shuffle(transforms_order.begin(), transforms_order.end());

			for(int o = 0; o < transforms_order.size(); o++)
			{
				switch( transforms_order[o] ) 
				{
					case 'b':			
						//1 Blur
						if( (double) rand() / (RAND_MAX) < 0.75)
							transformer.blur();
						break;

					case 'd':
						//2 Dynamic range compresion
						if( (double) rand() / (RAND_MAX) < 0.75)
							transformer.dynamic_range_compression();
						break;

					case 'a':
						//3 Affine transformation
						if( (double) rand() / (RAND_MAX) < 0.75)
							transformer.affine_transformation();
						break;

					case 'n':
						//4 Non uniform light
						if( (double) rand() / (RAND_MAX) < 0.25)
							transformer.non_uniform_light();
						break;

					case 'o':	
						//5 Dilate
						if( (double) rand() / (RAND_MAX) < 0.15)
							transformer.dilate();
						break;
				}
			}

			std::cout<<std::endl;

			output_patch_name << output_dir << patch_names[i].substr (0, patch_names[i].size()-4)<< "_T_"<< t << ".png";
			//Save the transform
			cv::imwrite(output_patch_name.str(), transformer.getPatch());


			//TODO save the transformed patch path and the corresponding label into a file
    	}
    }
}