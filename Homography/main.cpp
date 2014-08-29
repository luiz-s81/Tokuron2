#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"


cv::Mat do_panorama(cv::Mat image1, cv::Mat image2);

int main(){


	// Load the images
	cv::Mat image1= cv::imread("image/naist_center.jpg",1);
	cv::Mat image2= cv::imread("image/naist_left.jpg",1);
	cv::Mat image3= cv::imread("image/naist_right.jpg",1);

	cv::Mat result =  do_panorama(image1, image2);
	//cv::Mat result2 =  do_panorama(image3, result);

	imshow( "Result", result );
	//imshow( "Result2", result2 );
	cv::waitKey(0);
	return 0;
}