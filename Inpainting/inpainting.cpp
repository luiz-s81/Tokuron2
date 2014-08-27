#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>

extern bool inpainting_flag;

void generate_mask(cv::Mat, cv::Mat, int);
//void generate_temp_image(cv::Mat, cv::Mat);


int calculate_ssd(int,int,int,unsigned char *,int *,int,int,int,int);

void inpainting(cv::Mat image){

	int width = image.cols;// width of image 
	int height = image.rows;// height of image

	//mask image for classifying image into missing regions and so on
	cv::Mat mask(height,width,CV_8UC1); 

	//Temporary image
	cv::Mat tmp_image(height,width,CV_8UC3);

	//Used for window size for calculating pattern similarity. Length of window's side is 2*wsize+1
	int win_size=4; 

	//Initialize mask image
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			mask.data[y*width+x]=1;
		} 
	}
	//Initialize temporary image
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			for(int k=0;k<3;k++){
				tmp_image.data[y*width*3+x*3+k]=0;
			}
		}
	}

	//Sample program
	//Generate gray image by averaging values of R, G and B of input image
	//(This is just an example and not used for the following exercises.)
	/*for(int y=0;y<height;y++){
	for(int x=0;x<width;x++){
	unsigned char b = image.data[y*width*3+x*3+0];
	unsigned char g = image.data[y*width*3+x*3+1];
	unsigned char r = image.data[y*width*3+x*3+2];
	image.data[y*width*3+x*3+0] = (r+g+b)/3;
	image.data[y*width*3+x*3+1] = (r+g+b)/3;
	image.data[y*width*3+x*3+2] = (r+g+b)/3;
	}
	}*/


	//Write codes for Task 1-x for Image inpainting from here
	//The program generated in each task is helpful for subsequent tasks

	//In tasks 1-1 to 1-4, you need to update function "generate_mask()" in generate_mask.cpp 
	generate_mask(image,mask,win_size);


	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){

			unsigned char  mask_value = mask.data[y*width + x];

			if(mask_value == 255){
				// Setting max value to blue channel
				tmp_image.data[y*width*3+x*3+0] = 255; 
			}
			else if (mask_value == 1){
				// Setting yellow color (RGB 255,255,0)
				tmp_image.data[y*width*3+x*3+1] = 255; //green
				tmp_image.data[y*width*3+x*3+2] = 255; // red
			}
			else if (mask_value == 5){
				// Setting yellow color (RGB 255,255,0)
				tmp_image.data[y*width*3+x*3+0] = 255; //blue
				tmp_image.data[y*width*3+x*3+2] = 255; // red
			}
			else if (mask_value == 10){
				// Setting yellow color (RGB 255,255,0)
				tmp_image.data[y*width*3+x*3+0] = 255; //blue
				tmp_image.data[y*width*3+x*3+1] = 255; //green
			}
			else if (mask_value == 0){
				// Setting max value to green channel
				tmp_image.data[y*width*3+x*3+1] = 255;
			
			}

		}
	}

	//The following sentences are for displaying updated "image" matrix
	cv::imshow("image inpainting",image);
	cv::imshow("mask",mask);
	cv::imshow("temp image",tmp_image);
	
	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char value = mask.at<unsigned char>(i,j);
			if(value == 255)
				std::cout << 9 << " ";
			else if (value == 0)
				std::cout << 0 << " ";
			else if (value == 5)
				std::cout << 5 << " ";
			else if (value == 1)
				std::cout << 1 << " ";
			else
				std::cout << "x" << " ";
		}
		std::cout << std::endl;
	}
*/

	cv::waitKey(1);

	

	printf("Fin.\n");
	inpainting_flag=false;

}