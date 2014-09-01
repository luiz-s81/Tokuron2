#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

// Used to print the mask to file
void writeMatToFile(cv::Mat& m, const char* filename)
{
	std::ofstream fout(filename);
	int width = m.cols;// width of image 
	int height = m.rows;// height of image

	if(!fout)
	{
		std::cout<<"File Not Opened"<<std::endl;  return;
	}
	
	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			if(m.data[y*width+x] == 255)
				fout << 2;
			else if(m.data[y*width+x] == 1)
				fout << 1;
			else if(m.data[y*width+x] == 0)
				fout << 0;
			else if(m.data[y*width+x] == 5)
				fout << 5;
			else if(m.data[y*width+x] == 10)
				fout << 9;
			if (x==width-1)
			{
				fout<<std::endl;
			}
		}	
	}
	
	//for(int i=0; i<m.rows; i++)
	//{
	//	for(int j=0; j<m.cols; j++)
	//	{
	//		fout<<m.at<float>(i,j)<<"\t";
	//	}
	//	fout<<std::endl;
	//}

	fout.close();
	std::cout << "EOF" << std::endl;
}

// image -> input image
// mask -> mask iamge
// win_size -> side length of a window is win_size*2+1
void generate_mask(cv::Mat image, cv::Mat mask, int win_size)
{
	int width = image.cols;// width of image 
	int height = image.rows;// height of image

	// Reseting the mask. Otherwise it will not be "refresh"
	// on each interation
	mask = cv::Mat::ones(height, width, CV_8UC1);
	// Mat initialization
	cv::Mat tmp_image = cv::Mat::zeros(height, width, CV_8UC3);

	// Task 1-1
	//Set the mask's pixel to 255 (white) in the missing areas
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char b = image.data[y*width*3 + x*3 + 0];
			unsigned char g = image.data[y*width*3 + x*3 + 1];
			unsigned char r = image.data[y*width*3 + x*3 + 2];
			
			if(r > 183)
			{
				mask.data[y*width + x] = 255;
			}
		}
	}

	// Task 1-2
	// Set the mask's pixel to 0 (black) on the borders of missing areas
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char mask_value = mask.data[y*width + x];

			if(mask_value == 1)
			{
				/*unsigned char top_leftleft	= mask.data[(y-1)*width + (x-1-1)]; 
				unsigned char toptop_left	= mask.data[(y-1-1)*width + (x-1-1)]; */

				unsigned char top_left	= mask.data[(y-1)*width + (x-1)]; 
				unsigned char top		= mask.data[(y-1)*width + (x+0)];
				unsigned char top_right	= mask.data[(y-1)*width + (x+1)];

				unsigned char left		= mask.data[(y+0)*width + (x-1)]; 
				unsigned char right		= mask.data[(y+0)*width + (x+1)]; 

				unsigned char bottom_left	= mask.data[(y+1)*width + (x-1)]; 
				unsigned char bottom		= mask.data[(y+1)*width + (x+0)];
				unsigned char bottom_right	= mask.data[(y+1)*width + (x+1)];

				bool missingAreaInNeighborhood = 
					/*top_leftleft == 255||
					toptop_left == 255 ||*/

					top_left == 255 || 
					top == 255 || 
					top_right == 255 || 
					left == 255 || 
					right == 255 || 
					bottom_left == 255 || 
					bottom == 255 || 
					bottom_right == 255;

				if(missingAreaInNeighborhood)
				{
					mask.data[y*width + x] = 0;
				}
			} 
		}
	}

	// Task 1-3
	// Instead of look at the image and try to find missing areas
	// It's better to look at the mask and try to find "1s" near
	// the missing areas
	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){

			unsigned char mask_value = mask.data[y*width + x];

			if (mask_value == 255)
			{
				for(int i=-win_size; i<win_size; i++)
				{
					for(int j=-win_size; j<win_size; j++)
					{
						if (mask.data[(y+i)*width + (x+j)] == 1)
						{
							mask.data[(y+i)*width + (x+j)] = 5;
						}

					}
				}
			}
		}
	}

	// Task 1-4
	// Set the mask's pixel to 10 if a window centered in a pixel contains areas outside the image
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char mask_value = mask.data[y*width + x];

			if(mask_value == 1)
			{
				if (x - win_size < 0 || y - win_size < 0 || x + win_size >= width || y + win_size >= height)
				{
					mask.data[y*width + x] = 10;
				}
			}
		}
	}

	// Painting the temporary image
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char  mask_value = mask.data[y*width + x];

			if(mask_value == 255)
			{
				// Panting the missing area with blue
				tmp_image.data[y*width*3+x*3+0] = 255; 
			}
			else if (mask_value == 1)
			{
				// Setting the background with yellow
				tmp_image.data[y*width*3+x*3+1] = 255; // green
				tmp_image.data[y*width*3+x*3+2] = 255; // red
			}
			else if (mask_value == 0)
			{
				// Setting the border of missing area with green
				tmp_image.data[y*width*3+x*3+1] = 255; // green
			}
			else if (mask_value == 5)
			{
				// Making a thick purple border around the green border
				// of the missing area
				tmp_image.data[y*width*3+x*3+0] = 255; // blue
				tmp_image.data[y*width*3+x*3+1] = 0; // blue
				tmp_image.data[y*width*3+x*3+2] = 255; // red
			}
			else if (mask_value == 10)
			{
				// Making a cyan border around the picture
				tmp_image.data[y*width*3+x*3+0] = 255; //blue
				tmp_image.data[y*width*3+x*3+1] = 255; //green
				tmp_image.data[y*width*3+x*3+2] = 0; //blue
			}
		}
	}

	// used to debug the mask "making"
	//writeMatToFile(mask, "luiz.txt");

	//cv::imshow("image inpainting",image);
	/*cv::imshow("temp image",tmp_image);
	cv::imshow("mask",mask);*/
	cv::waitKey(1);
}



