#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat do_panorama(cv::Mat image1, cv::Mat image2);
cv::Mat do_panorama2(cv::Mat image1, cv::Mat image2, cv::Mat mask);
cv::Mat crop_image(cv::Mat source, cv::Mat mask);

int main(){

	// Load the images
	cv::Mat center= cv::imread("image/naist_center.jpg",1);
	cv::Mat left= cv::imread("image/naist_left.jpg",1);
	cv::Mat right = cv::imread("image/naist_right.jpg",1);

	// First stitching 
	cv::Mat partial_result =  do_panorama2(left, center, cv::Mat());
	imshow( "Partial Result", partial_result);

	// Create a mask
	cv::Mat mask;
	cv::cvtColor(partial_result, mask, cv::COLOR_RGB2GRAY);
	cv::medianBlur(mask, mask, 5);
	cv::threshold(mask, mask, 1.0, 255.0, cv::THRESH_BINARY);

	// Stitch the 3rd image
	cv::Mat result2 =  do_panorama2(partial_result, right, mask);
	
	// Create a 2nd mask for cropping
	cv::Mat mask2;
	cv::cvtColor(result2, mask2, cv::COLOR_RGB2GRAY);
	cv::medianBlur(mask2, mask2, 5);
	cv::threshold(mask2, mask2, 1.0, 255.0, cv::THRESH_BINARY);

	cv::Mat result2_cropped = crop_image(result2, mask2);
	
	imshow( "Result2", result2_cropped );

	cv::waitKey(0);
	return 0;
}

cv::Mat crop_image(cv::Mat source, cv::Mat mask)
{
	cv::Mat temp; 
	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;

	// Find Contours destroy the input image
	mask.copyTo(temp);
	cv::findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	cv::Rect roi  = cv::boundingRect(contours[0]);
	cv::Mat cropped = source(roi);

	return cropped;
}
