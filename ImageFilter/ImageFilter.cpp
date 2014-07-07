#include <iostream>
#include <ctime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::RNG rng(time(0));

cv::Mat makeWharol(cv::Mat image){
	
	cv::Mat samples(image.rows * image.cols, 3, CV_32F);

	for(int y = 0; y < image.rows; y++) {
		for(int x = 0; x < image.cols; x++) {
			for( int z = 0; z < 3; z++) {
				samples.at<float>(y + x * image.rows, z) = image.at<cv::Vec3b>(y,x)[z];
			}
		}
	}

	const int clusterCount = 2;
	int attempts = 3;
	cv::Mat labels;
	cv::Mat centers;

	// Image segmentation creating 
	kmeans(samples, clusterCount, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10, 1.0), attempts, cv::KMEANS_PP_CENTERS, centers);

	// We are creating space for one more color than required by segmentation cluster
	// because we will use for the floodfill function
	cv::Vec3b colors[clusterCount+1];

	// Generating random colors
	for(int i = 0; i < clusterCount+1; i ++) {
		cv::Vec3b color = cv::Vec3b(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		colors[i] = color;
	}

	// 
	cv::Mat newImage(image.size(), image.type());

	// Set the new (random) color to the color blocks, by setting each channel separately
	// Probably this code has bad performance, I'm going to change in the future
	for( int y = 0; y < image.rows; y++ ) {
		for( int x = 0; x < image.cols; x++ ) {

			int cluster_idx = labels.at<int>(y + x * image.rows,0);
			newImage.at<cv::Vec3b>(y,x)[0] = colors[cluster_idx][0];
			newImage.at<cv::Vec3b>(y,x)[1] = colors[cluster_idx][1];
			newImage.at<cv::Vec3b>(y,x)[2] = colors[cluster_idx][2];
		}
	}

	// Getting the "Point" at the center of image
	cv::Point point(image.rows / 2, image.cols / 2);
	// This is also probably a bad way to do that
	cv::Scalar color(colors[2][0], colors[2][1], colors[2][2]);
	// "Paint" the face (usually if it's a portrait) with a different color
	cv::floodFill(newImage, point, color, 0, 10000, 10000, 4);

	return newImage;
}

// Creates the result image, which is composed by 4 copies of original image
cv::Mat makePanel(cv::Mat image) {

	cv::Size smallSize;
	smallSize.width = image.size().width / 2;
	smallSize.height = image.size().height / 2;
	
	// Resize the image for half of its size
	cv::Mat smallImage = cv::Mat(smallSize, CV_8UC3);
	resize(image, smallImage, smallSize, 0,0, cv::INTER_LINEAR);
	
	cv::blur(smallImage, smallImage, cv::Size(3, 3));

	// Create a Mat that will receive the 4 small images, forming the result image
	cv::Mat resultImage = cv::Mat(smallImage.rows * 2, smallImage.cols * 2, smallImage.type());
	
	for(int i = 0; i < 2; i ++) {
		for (int j = 0; j < 2; j ++) {
			// Calls the Warhol 4 times, placing each picture in one sector of the result image
			makeWharol(smallImage).copyTo(resultImage(cv::Rect(j * smallImage.cols, i * smallImage.rows, smallImage.cols, smallImage.rows)));
		}
	}

	return resultImage;
}

bool fileExists (const std::string& name) {

	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	} 
	else {
		return false;
	}   
}

int main(int argc, char *argv[]) {

	// Ask user to input a file?
	//std::cout << "Enter the path for the image file: " ;
	//std::cin >> filename;

	// Use "auto suggested" image
	std::string filename = "che.jpg";

	while (!fileExists(filename))
	{
		std::cout << "File not found! Try again: " ;
		std::cin >> filename;
	}

	cv::Mat image = cv::imread(filename);

	cv::Mat resultImage = makePanel(image);
	
	// Show the result image
	imshow( "Warholized image", resultImage);
	
	// Save the result image in the disk
	imwrite("warholized.jpg", resultImage);

	cv::waitKey(0);

	return 0;
}
