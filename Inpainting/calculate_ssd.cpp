
#include <opencv2/opencv.hpp>

int calculate_ssd(int width, int height, int win_size, cv::Mat image, cv::Mat mask, int tar_x, int tar_y, int cor_x, int cor_y )
{
	// width -> width of image(画像の幅)
	// height -> height of image(画像の高さ)
	// win_size -> side length of a window is win_size*2+1 (ウィンドウの1辺の長さはwin_size*2+1)
	// image
	// mas
	// tar_x, tar_y -> coordinate of target pixel (ターゲットとなる画素の座標)
	// cor_x, cor_y -> coordinate of corresponding pixel (対応画素の座標)

	int ssd=0;
	int tar_position = 0;
	int cor_position = 0;
	int total_positions = width * height * 3;

	//SSD(Sum of squared differences of RGB values between windows centered at target and corresponding pixels)
	for(int p=-win_size; p<=win_size;p++){
		for(int q=-win_size; q<=win_size; q++){

			tar_position = (tar_y+p)*width*3+(tar_x+q)*3;
			cor_position = (cor_y+p)*width*3+(cor_x+q)*3;

			if(tar_position >= 0 && tar_position < total_positions && cor_position >= 0 && cor_position < total_positions){

				unsigned char mask_target = mask.data[(tar_y+p)*width + (tar_x+q)];

				if(mask_target != 255) 
				{
					ssd+=(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])
						+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])
						+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2]);
				}
			}

		}
	}

	return ssd;
}