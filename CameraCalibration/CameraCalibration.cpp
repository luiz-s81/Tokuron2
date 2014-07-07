#include <iostream>
#include <fstream>
#include <iomanip>  

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

// Number of image files to load from disk
int nrImages; 
// Number of "shots" do make the calibration
int nrFrames;
// Index of current read image
int currentImage;
// Holds the image from image source
cv::Mat image, grayImage;
// Output Matrices
cv::Mat cameraMatrix; // Intrinsic parameters
cv::Mat distCoeffs;	  // Distortion coefficienst
// List of vectors with 2D points from the chessboard
cv::vector<cv::vector<cv::Point2f>> imagePoints;
// List of vectors with 3D points from the chessboard
cv::vector<cv::vector<cv::Point3f>> objectPoints;
//Output rotations and translations matrices
cv::vector<cv::Mat> rvecs, tvecs;

// Output the calibration result to a text file
void writeFile(){
		
	std::ofstream calibrationResult;
	calibrationResult.open ("calibration-result-luiz-sampaio.txt");
	calibrationResult << "Intrinsic parameters: [a11] ... [a33] \n";
	calibrationResult << std::fixed;
	
	for (int i = 0; i < cameraMatrix.rows; i++)
	{
		for (int j = 0; j < cameraMatrix.cols; j++)
		{
			calibrationResult << std::setprecision(6) << cameraMatrix.at<double>(i, j) << "\t";
		}
	}
	
	calibrationResult << "\n\n";
	calibrationResult << "Extrinsic parameters : [r11] ... [r33][t1][t2][t3] \n";

	for (int i = 0; i < (int)rvecs.size(); i++)
	{
		cv::Mat rotationMatix;
		// Convert the rotation vector (3x1) to a rotation matrix (3x3)
		Rodrigues(rvecs[i], rotationMatix);

		calibrationResult << "[Image " << std::setw(2) << std::setfill('0') << i << "] "; 

		// Writing the rotation matrix
		for (int j = 0; j  < rotationMatix.rows; j++)
		{
			for (int k = 0; k < rotationMatix.cols; k++)
			{
				calibrationResult << std::setprecision(6) << rotationMatix.at<double>(j, k) << "\t";
			}
		}

		// Writing the translation vector
		for (int j = 0; j  < tvecs[i].rows; j++)
		{
			for (int k = 0; k < tvecs[i].cols; k++)
			{
				calibrationResult << std::setprecision(6) << tvecs[i].at<double>(j, k) << "\t";
			}
		}

		calibrationResult << "\n";
	}
	
	calibrationResult.close();
}

// Load chessboard images from disk
cv::Mat getNextImage() {

	std::stringstream filename;
	// Build the filename in the format provided
	filename << "data/" << std::setw(5) << std::setfill('0')  << currentImage << ".jpg";

	if(currentImage <= nrImages){
	
		currentImage++;
	}

	// Print as debug
	std::cout << filename.str() << std::endl;

	return cv::imread(filename.str());
}

// Calibrate the camera
// returns the re-projection error
double calibrate()
{
		std::cout << "Proceeding with calculations..." << std::endl;

	// start calibration
	return calibrateCamera(
		objectPoints, // (input) the 3D points
		imagePoints,  // (input) the image points
		image.size(), // (input) image size
		cameraMatrix, // (output) 3x3 floating-point camera matrix
		distCoeffs,   // (output) vector of distortion coefficients of 4, 5, or 8 elements
		rvecs,        // (output) vector of rotation vectors
		tvecs);		  // (output) vector of translation vectors
	//flag		  // set options
	//);        
	//,CV_CALIB_USE_INTRINSIC_GUESS);
}

// Add scene points and corresponding image points
void addPoints(const cv::vector<cv::Point2f>& imageCorners, const cv::vector<cv::Point3f>& objectCorners) {

	// 2D image points from one view
	imagePoints.push_back(imageCorners);          
	// corresponding 3D scene points
	objectPoints.push_back(objectCorners);
	std::cout << "corners stored" << std::endl;
}

// Get chessboard images and extract corner points
int addChessboardPoints(cv::Size& boardSize) {

	// 2D position (image coordinate) of the corners for each image
	cv::vector<cv::Point2f> imageCorners;  
	// 3D position (world coordinate) of the corners for each image
	cv::vector<cv::Point3f> objectCorners; 
	
	int success = 0;

	// Initialize the vector with 3D points in arbitray measure unit
	// The corners are at 3D location (X,Y,Z)= (i,j,0)
	// Ex: <(0,0,0), (0,1,0), (0,2,0) ... (1,0,0), (1,1,0)...>
	for (int i = 0; i < boardSize.height; i++) {
		for (int j=0; j<boardSize.width; j++) {
			objectCorners.push_back(cv::Point3f(i, j, 0.0f));
		}
	}

	while(success < nrFrames) {

		image = getNextImage();
		
		// Convert the color image to grayscale
		// in order to use the SubPix accuracy
		cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

		// if we don't call the "waitKey", the imshow doesn't work
		int key = cv::waitKey(1);

		// Get the chessboard corners. Possible filters to use
		// CV_CALIB_CB_FAST_CHECK
		//CV_CALIB_CB_ADAPTIVE_THRESH
		//CV_CALIB_CB_NORMALIZE_IMAGE
		//CV_CALIB_CB_FILTER_QUADS
		bool found = findChessboardCorners(grayImage, boardSize, imageCorners);
		
		if(found){
			// Get subpixel accuracy on the corners
			cornerSubPix(grayImage, imageCorners, cv::Size(5,5), cv::Size(-1,-1), 
				cv::TermCriteria( cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 
				30,	//30: max number of iterations 
				0.1 //0.1: min accuracy
				));     

			drawChessboardCorners(image, boardSize, imageCorners, found);
			
			addPoints(imageCorners, objectCorners);
			success++;

			std::cout << "corners found! " << std::endl;
			
		}
		else {

			std::cout << "corners NOT found! " << std::endl;
		}
		
		imshow("Corners on Chessboard", image);
	}

	return success;
}

int main() {

	nrImages = 50;
	nrFrames = 50;
	currentImage = 1;

	// 1. Find the corners on the chessboard
	// Remember to count only the inner corners
	addChessboardPoints(cv::Size(10, 7));

	// 2. Calibrate the camera based on the paramenters from the chessboard
	calibrate();
	
	// 3. Save the calibration result in a text file
	writeFile();

	return 0;
}





