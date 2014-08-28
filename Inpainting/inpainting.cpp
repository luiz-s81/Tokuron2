#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <limits>

extern bool inpainting_flag;

void generate_mask(cv::Mat, cv::Mat, int);
int calculate_ssd(int, int, int, cv::Mat, int *mask, int tar_x, int tar_y, int cor_x, int cor_y);
void paint(cv::Mat image, cv::Mat mask, int targetX, int targetY, int correspondingX, int correspondingY, int win_size);

void inpainting(cv::Mat image){

	int width = image.cols; // image width
	int height = image.rows; // image height
	//Used for window size for calculating pattern similarity. Length of window's side is 2*wsize+1
	int win_size= 8; 
	//int size = 2 * win_size +1;

	// Mask image for classifying image into missing regions and so on
	// Initialized with "1s"
	cv::Mat mask = cv::Mat::ones(height, width, CV_8UC1);

	//In tasks 1-1 to 1-4, you need to update function "generate_mask()" in generate_mask.cpp 
	generate_mask(image, mask, win_size);

	// Used if we want to make just one replacement
	bool found = false;

	// Task 1-5
	// This is not really necessary. Just a trial for the real inpating
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char mask_value = mask.data[y*width + x];

			// Found a pixel on the border of missing area	
			if(mask_value == 0 && !found)
			{
				// Looking in the window area
				for (int i=-win_size; i<win_size; i++) 
				{
					for (int j=-win_size; j<win_size; j++) 
					{
						int mask_value = mask.data[(y+i)*width + (x+j)];

						// If we found a missing area, paint it
						if(mask_value == 255)
						{
							image.data[(y+i)*width*3+(x+j)*3+0] = 0; //blue
							image.data[(y+i)*width*3+(x+j)*3+1] = 255; //green
							image.data[(y+i)*width*3+(x+j)*3+2] = 0; //red
						}
					}
				}

				found = true;
			}
		}
	}

	int smallest_ssd =  std::numeric_limits<int>::max();
	int correspondingX = 0;
	int correspondingY = 0;

	//Task 1-7
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char target_mask = mask.data[y*width + x];

			// Find pixel with 0
			if(target_mask == 0)
			{
				for(int j=0; j<height; j++)
				{
					for(int i=0; i<width; i++)
					{
						unsigned char corresponding_mask = mask.data[j*width + i];

						// Find pixel with 1
						if(corresponding_mask == 1)
						{
							// Is the pointer for the mask useless?
							int ssd = calculate_ssd(width, height, win_size, image, 0, x, y, i, j);

							if(ssd < smallest_ssd  && ssd > 0)
							{
								smallest_ssd = ssd;
								correspondingX = i;
								correspondingY = j;

							} // if(ssd < smallestSSD)
						} // if(maskValue2 == 1)
					} // for(int t=0; t<width; t++)
				} // for(int w=0; w<height; w++)

				for (int w = -win_size; w <= win_size; w++)
				{
					for (int t = -win_size; t <= win_size; t++)
					{
						if (mask.data[(y+w)*width+(x+t)] == 255)
						{
							image.data[(y+w)*width*3+(x+t)*3+0] = image.data[(correspondingY+w)*width*3+(correspondingX+t)*3+0];
							image.data[(y+w)*width*3+(x+t)*3+1] = image.data[(correspondingY+w)*width*3+(correspondingX+t)*3+1];
							image.data[(y+w)*width*3+(x+t)*3+2] = image.data[(correspondingY+w)*width*3+(correspondingX+t)*3+2];
						}
					}
			    }

				cv::waitKey(1);
				cv::imshow("image inpainting",image);
				smallest_ssd =  std::numeric_limits<int>::max();

			} // if(maskValue == 0)
		}
	}

	cv::imshow("image inpainting",image);
	cv::waitKey(1);

	printf("Fin.\n");
	inpainting_flag=false;
}