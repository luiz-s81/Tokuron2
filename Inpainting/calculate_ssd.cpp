
#include <opencv2/opencv.hpp>

int calculate_ssd(int width, int height, int win_size, cv::Mat image, int *mask, int tar_x, int tar_y, int cor_x, int cor_y ){


	// width -> width of image(画像の幅)
	// height -> height of image(画像の高さ)
	// win_size -> side length of a window is win_size*2+1 (ウィンドウの1辺の長さはwin_size*2+1)
	// image
	// mas
	// tar_x, tar_y -> coordinate of target pixel (ターゲットとなる画素の座標)
	// cor_x, cor_y -> coordinate of corresponding pixel (対応画素の座標)

	
	int ssd=0;

	//SSD(Sum of squared differences of RGB values between windows centered at target and corresponding pixels)
	//対象画素とそれに対応する画素を中心とする２つのウィンドウ内の対応する画素値RGBの差の二乗の総和
	for(int p=-win_size; p<=win_size;p++){
		for(int q=-win_size; q<=win_size; q++){
			ssd+=(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])
				+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])
				+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2]);

		}
	}

	return ssd;
	
	

}