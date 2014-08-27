
#include <opencv2/opencv.hpp>

char search_window(cv::Mat, int, int, int);

// image -> input image(入力画像)
// mask -> mask iamge(マスク画像)
// win_size -> side length of a window is win_size*2+1(ウィンドウの1辺の長さはwin_size*2+1)
void generate_mask(cv::Mat image, cv::Mat mask, int win_size){

	int width = image.cols;// width of image 
	int height = image.rows;// height of image

	for(int y=0; y<height; y++){

		for(int x=0; x<width; x++){

			unsigned char topLeftRedValue	= image.data[(y-1)*width*3 + (x-1)*3 + 2]; 
			unsigned char topRedValue		= image.data[(y-1)*width*3 + (x+0)*3 + 2];
			unsigned char topRightRedValue	= image.data[(y-1)*width*3 + (x+1)*3 + 2];

			unsigned char leftRedValue		= image.data[(y+0)*width*3 + (x-1)*3 + 2]; 
			unsigned char currRedValue		= image.data[(y+0)*width*3 + (x+0)*3 + 2]; 
			unsigned char rightRedValue		= image.data[(y+0)*width*3 + (x+1)*3 + 2]; 

			unsigned char bottomLeftRedValue	= image.data[(y+1)*width*3 + (x-1)*3 + 2]; 
			unsigned char bottomRedValue		= image.data[(y+1)*width*3 + (x+0)*3 + 2];
			unsigned char bottomRightRedValue	= image.data[(y+1)*width*3 + (x+1)*3 + 2];

			// There is any "missing pixel" (in red) on neighborhood?
			bool missingOnNeighborhood = (topLeftRedValue == 255 || 
				topRedValue == 255 || 
				topRightRedValue == 255 || 
				leftRedValue == 255 || 
				rightRedValue == 255 || 
				bottomLeftRedValue == 255 || 
				bottomRedValue == 255 || 
				bottomRightRedValue == 255);

			// Missing region is painted in red
			if (currRedValue == 255) {
				mask.data[y*width + x] = 255;
			}
			else if(missingOnNeighborhood){
				mask.data[y*width + x] = 0;
			}
			else{ 
				char result = search_window(image, y, x, win_size);

				if(result == 'i')
					mask.data[y*width + x] = 5;	
				else if (result == 'o')
					mask.data[y*width + x] = 10;	
			}
			/*else {
				mask.data[y*width + x] = 1; 
			}*/
		}
	}
}

char search_window(cv:: Mat image, int y, int x, int window_size){

	int width = image.cols;
	int height = image.rows;
	int maxValue = width * height * 3;

	char result = 'a';

	for (int i = -window_size; i <= window_size; i++) 
	{
		for (int j = -window_size; j <= window_size; j++) 
		{
			int position = (y+i)*width*3 + (x+j)*3 + 2;

			if(position >= 0 && position <= maxValue)
			{
				unsigned char value = image.data[position]; 

				// if there is a missing pixel (in red) in the window's range
				if(value == 255)
				{
					//std::cout << position << std::endl;
					result = 'i'; //in
					
				}
			}
			else 
			{
				result = 'o'; //out
			}
		}
	}

	return result;
}