
#include <opencv2/opencv.hpp>

int calculate_ssd(int width, int height, int win_size, cv::Mat image, int *mask, int tar_x, int tar_y, int cor_x, int cor_y ){


	// width -> width of image(�摜�̕�)
	// height -> height of image(�摜�̍���)
	// win_size -> side length of a window is win_size*2+1 (�E�B���h�E��1�ӂ̒�����win_size*2+1)
	// image
	// mas
	// tar_x, tar_y -> coordinate of target pixel (�^�[�Q�b�g�ƂȂ��f�̍��W)
	// cor_x, cor_y -> coordinate of corresponding pixel (�Ή���f�̍��W)

	
	int ssd=0;

	//SSD(Sum of squared differences of RGB values between windows centered at target and corresponding pixels)
	//�Ώۉ�f�Ƃ���ɑΉ������f�𒆐S�Ƃ���Q�̃E�B���h�E���̑Ή������f�lRGB�̍��̓��̑��a
	for(int p=-win_size; p<=win_size;p++){
		for(int q=-win_size; q<=win_size; q++){
			ssd+=(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+0]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+0])
				+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+1]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+1])
				+(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2])*(image.data[(tar_y+p)*width*3+(tar_x+q)*3+2]-image.data[(cor_y+p)*width*3+(cor_x+q)*3+2]);

		}
	}

	return ssd;
	
	

}