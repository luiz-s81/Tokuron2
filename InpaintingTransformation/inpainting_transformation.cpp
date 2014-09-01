#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <limits>

void generate_mask(cv::Mat, cv::Mat, int);
int calculate_ssd(int, int, int, cv::Mat, int *mask, int tar_x, int tar_y, int cor_x, int cor_y);

const cv::Point2f matrix1[] =
{
	/*cv::Point2f(17, 61),
	cv::Point2f(189, 52),
	cv::Point2f(29, 158),
	cv::Point2f(185, 113)*/
	cv::Point2f(17, 61),
	cv::Point2f(187, 52),
	cv::Point2f(29, 158),
	cv::Point2f(185, 113)
};

const cv::Point2f matrix2[] =
{
	cv::Point2f(0, 40),
	cv::Point2f(140, 40),
	cv::Point2f(0, 99),
	cv::Point2f(140, 99)
	/*cv::Point2f(0, 39),
	cv::Point2f(131, 39),
	cv::Point2f(0, 99),
	cv::Point2f(139, 99)*/
};

void inpainting(cv::Mat image){

	int width = image.cols; // image width
	int height = image.rows; // image height

	//Used for window size for calculating pattern similarity. Length of window's side is 2*wsize+1
	int win_size = 8; 
	int win_lenght = 2 * win_size + 1;
	//int size = 2 * win_size +1;

	// Mask image for classifying image into missing regions and so on
	// Initialized with "1s"
	cv::Mat mask = cv::Mat::ones(height, width, CV_8UC1);

	//In tasks 1-1 to 1-4, you need to update function "generate_mask()" in generate_mask.cpp 
	generate_mask(image, mask, win_size);
	//return;
	// Used if we want to make just one replacement
	bool found = false;

	/* Task 1-5
	This is not really necessary. Just a trial for the real inpating*/
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

	found = false;

	//Task 1-7
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char target_mask = mask.data[y*width + x];

			// Find pixel with 0
			if(target_mask == 0 && !found)
			{
				//found = true;

				for(int j=0; j<height; j++)
				{
					for(int i=0; i<width; i++)
					{
						unsigned char corresponding_mask = mask.data[j*width + i];

						// Find pixel with 1
						if(corresponding_mask == 1)
						{
							// Is the pointer for the mask useless?
							int ssd = calculate_ssd(width, height, win_lenght, image, 0, x, y, i, j);

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

				cv::imshow("image inpainting",image);
				cv::waitKey(1);
				smallest_ssd =  std::numeric_limits<int>::max();

				generate_mask(image, mask, win_size);

			} // if(maskValue == 0)
		}
	}

	/*cv::imshow("image inpainting",image);
	cv::waitKey(1);*/

	printf("Fin.\n");

}

cv::Mat transform_image(cv::Mat source, bool reverse = false)
{
	cv::Mat result(source.rows, source.cols, source.type());
	cv::Mat homography_matrix;

	if(reverse)
	{
		homography_matrix = cv::getPerspectiveTransform(matrix2, matrix1);
	}
	else{	
		homography_matrix = cv::getPerspectiveTransform(matrix1, matrix2);
	}

	cv::warpPerspective(source, result, homography_matrix, cv::Size(source.rows, source.cols));

	return result;
}


int main()
{
	int key;
	cv::Mat original = cv::imread("image/a079.bmp");

	// We will apply inpaiting over this Mat:
	cv::Mat transformed = transform_image(original);
	//cv::Mat transformed_mask = cv::Mat::ones(transformed.rows, transformed.cols, CV_8UC1);

	cv::Mat untransformed = transform_image(transformed, true);
	cv::Mat mask1 = cv::Mat::ones(transformed.rows, transformed.cols, CV_8UC1);
	generate_mask(untransformed, mask1, 8);
	
	//cv::imshow("abc", mask1);
	bool x = false;
	while(1)
	{
		key=cv::waitKey(10);

		if(key == 'i'){
			inpainting(transformed);
			x = true;
		}

		cv::imshow("image inpainting",transformed);

		if(x){

			int width = mask1.cols; // image width
			int height = mask1.rows; // image height

			cv::Mat untransformed = transform_image(transformed, true);
			//cv::imshow("Untransformed", untransformed);

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					unsigned char mask_value = mask1.data[y*width + x];

					if(mask_value == 255 || mask_value == 0)
					{
						original.at<cv::Vec3b>(y, x)[0] = untransformed.at<cv::Vec3b>(y, x)[0];
						original.at<cv::Vec3b>(y, x)[1] = untransformed.at<cv::Vec3b>(y, x)[1];
						original.at<cv::Vec3b>(y, x)[2] = untransformed.at<cv::Vec3b>(y, x)[2];
					}
				}
			}

			cv::imshow("Final Result", original);
		}
	}

	cv::waitKey(0);
	return 0;
}




