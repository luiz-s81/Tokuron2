#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat do_panorama2(cv::Mat image1, cv::Mat image2)
{
	// Load the images
	/*cv::Mat image1= cv::imread("image/naist_center.jpg",1);
	cv::Mat image2= cv::imread("image/naist_left.jpg",1);*/

	cv::Mat gray_image1;
	cv::Mat gray_image2;

	// Convert to Grayscale
	cvtColor(image1, gray_image1, CV_RGB2GRAY);
	cvtColor(image2, gray_image2, CV_RGB2GRAY);

	/*imshow("first image",image2);
	imshow("second image",image1);*/

	/*if( !gray_image1.data || !gray_image2.data )
	{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }*/

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector(minHessian);

	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;

	detector.detect(gray_image2, keypoints_object);
	detector.detect(gray_image1, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_object, descriptors_scene;

	extractor.compute(gray_image2, keypoints_object, descriptors_object);
	extractor.compute(gray_image1, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::FlannBasedMatcher matcher;
	std::vector<cv::DMatch> matches;

	//should I change the order here??
	matcher.match(descriptors_object, descriptors_scene, matches);
	//matcher.match(descriptors_scene, descriptors_object, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for(int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;

		if(dist < min_dist) 
		{
			min_dist = dist;
		}

		if(dist > max_dist) 
		{
			max_dist = dist;
		}
	}

	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector<cv::DMatch> good_matches;

	for(int i = 0; i < descriptors_object.rows; i++)
	{ 
		if(matches[i].distance < 2*min_dist)
		{ 
			good_matches.push_back(matches[i]); 
		}
	}

	//-- Draw only "good" matches
	/*cv::Mat img_matches;
	drawMatches(image1, keypoints_object, image2, keypoints_scene,
		good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
		cv::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );*/

	//imshow( "Good Matches", img_matches );

	std::vector<cv::Point2f> obj;
	std::vector<cv::Point2f> scene;

	for( int i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	// Find the Homography Matrix
	cv::Mat H = findHomography(obj, scene, CV_RANSAC);
	// Use the Homography Matrix to warp the images
	cv::Mat result;
	
	warpPerspective(image2, result, H, cv::Size(image1.cols+(image2.cols), image1.rows+(image2.rows)));
	
	imshow( "Warp Perspective", result );
	cv::Mat ROI(result, cv::Rect(0,0,image1.cols,image1.rows));
	image1.copyTo(ROI);

	return result;

	//cv::waitKey(0);
	//return 0;
}

