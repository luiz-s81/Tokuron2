#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat do_panorama(cv::Mat image1, cv::Mat image2);
cv::Mat do_panorama2(cv::Mat image1, cv::Mat image2);

int main(){


	// Load the images
	cv::Mat center= cv::imread("image/naist_center.jpg",1);
	cv::Mat left= cv::imread("image/naist_left.jpg",1);
	cv::Mat right = cv::imread("image/naist_right.jpg",1);

	cv::Mat result =  do_panorama2(left, center);
	cv::Mat result2 =  do_panorama2(result, right);

	imshow( "Final Result", result );
	imshow( "Result2", result2 );
	
	cv::waitKey(0);
	return 0;
}