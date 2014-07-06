#include <iostream>
#include <fstream>
#include <iomanip>  
//#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// I'm not using a header file to make the compilation easier, since it's just
// an exercise, not a real program
Mat getNextImage();
int addChessboardPoints(Size& boardSize);
void addPoints(const vector<Point2f>& imageCorners, const vector<Point3f>& objectCorners);
double calibrate();
void writeFile();

// Number of image files to load from disk
int nrImages; 
// Number of "shots" do make the calibration
int nrFrames;
// Index of current read image
int currentImage;
// Holds the image from image source
Mat image, grayImage;
// Output Matrices
Mat cameraMatrix; // Intrinsic parameters
Mat distCoeffs;	  // Distortion coefficienst
// List of vectors with 2D points from the chessboard
vector<vector<Point2f>> imagePoints;
// List of vectors with 3D points from the chessboard
vector<vector<Point3f>> objectPoints;
//Output rotations and translations matrices
vector<Mat> rvecs, tvecs;


int main() {

	nrImages = 50;
	nrFrames = 50;
	currentImage = 1;

	// 1. Find the corners on the chessboard
	// Remember to count only the inner corners
	addChessboardPoints(Size(10, 7));

	// 2. Calibrate the camera based on the paramenters from the chessboard
	calibrate();
	
	// 3. Save the calibration result in a text file
	writeFile();
}

// Get chessboard images and extract corner points
int addChessboardPoints(Size& boardSize) {

	// 2D position (image coordinate) of the corners for each image
	vector<Point2f> imageCorners;  
	// 3D position (world coordinate) of the corners for each image
	vector<Point3f> objectCorners; 
	
	int success = 0;

	// Initialize the vector with 3D points in arbitray measure unit
	// The corners are at 3D location (X,Y,Z)= (i,j,0)
	// Ex: <(0,0,0), (0,1,0), (0,2,0) ... (1,0,0), (1,1,0)...>
	for (int i = 0; i < boardSize.height; i++) {
		for (int j=0; j<boardSize.width; j++) {
			objectCorners.push_back(Point3f(i, j, 0.0f));
		}
	}

	while(success < nrFrames) {

		image = getNextImage();
		
		// Convert the color image to grayscale
		// in order to use the SubPix accuracy
		cvtColor(image, grayImage, COLOR_BGR2GRAY);

		// if we don't call the "waitKey", the imshow doesn't work
		int key = waitKey(1);

		// Get the chessboard corners. Possible filters to use
		// CV_CALIB_CB_FAST_CHECK
		//CV_CALIB_CB_ADAPTIVE_THRESH
		//CV_CALIB_CB_NORMALIZE_IMAGE
		//CV_CALIB_CB_FILTER_QUADS
		bool found = findChessboardCorners(grayImage, boardSize, imageCorners);
		
		if(found){
			// Get subpixel accuracy on the corners
			cornerSubPix(grayImage, imageCorners, Size(5,5), Size(-1,-1), 
				TermCriteria( TermCriteria::MAX_ITER + TermCriteria::EPS, 
				30,	//30: max number of iterations 
				0.1 //0.1: min accuracy
				));     

			drawChessboardCorners(image, boardSize, imageCorners, found);
			
			addPoints(imageCorners, objectCorners);
			success++;

			cout << "corners found! " << endl;
			
		}
		else {

			cout << "corners NOT found! " << endl;
		}
		
		imshow("Corners on Chessboard", image);
	}

	return success;
}

// Add scene points and corresponding image points
void addPoints(const vector<Point2f>& imageCorners, const vector<Point3f>& objectCorners) {

	// 2D image points from one view
	imagePoints.push_back(imageCorners);          
	// corresponding 3D scene points
	objectPoints.push_back(objectCorners);
	cout << "corners stored" << endl;
}

// Calibrate the camera
// returns the re-projection error
double calibrate()
{
		cout << "Proceeding with calculations..." << endl;

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

// Load chessboard images from disk
Mat getNextImage() {

	stringstream filename;
	// Build the filename in the format provided
	filename << "data/" << setw(5) << setfill('0')  << currentImage << ".jpg";

	if(currentImage <= nrImages){
	
		currentImage++;
	}

	// Print as debug
	cout << filename.str() << std::endl;

	return imread(filename.str());
}

// Output the calibration result to a text file
void writeFile(){
		
	ofstream calibrationResult;
	calibrationResult.open ("calibration-result-luiz-sampaio.txt");
	calibrationResult << "Intrinsic parameters: [a11] ... [a33] \n";
	calibrationResult << fixed;
	
	for (int i = 0; i < cameraMatrix.rows; i++)
	{
		for (int j = 0; j < cameraMatrix.cols; j++)
		{
			calibrationResult << setprecision(6) << cameraMatrix.at<double>(i, j) << "\t";
		}
	}
	
	calibrationResult << "\n\n";
	calibrationResult << "Extrinsic parameters : [r11] ... [r33][t1][t2][t3] \n";

	for (int i = 0; i < (int)rvecs.size(); i++)
	{
		Mat rotationMatix;
		// Convert the rotation vector (3x1) to a rotation matrix (3x3)
		Rodrigues(rvecs[i], rotationMatix);

		calibrationResult << "[Image " << setw(2) << setfill('0') << i << "] "; 

		// Writing the rotation matrix
		for (int j = 0; j  < rotationMatix.rows; j++)
		{
			for (int k = 0; k < rotationMatix.cols; k++)
			{
				calibrationResult << setprecision(6) << rotationMatix.at<double>(j, k) << "\t";
			}
		}

		// Writing the translation vector
		for (int j = 0; j  < tvecs[i].rows; j++)
		{
			for (int k = 0; k < tvecs[i].cols; k++)
			{
				calibrationResult << setprecision(6) << tvecs[i].at<double>(j, k) << "\t";
			}
		}

		calibrationResult << "\n";
	}
	
	calibrationResult.close();
}
