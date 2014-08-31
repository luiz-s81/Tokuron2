// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define RED 255
#define GREEN 0
#define BLUE 0

int imWidth  = 0;   // Image width
int imHeight = 0;   // Image height

cv::Mat image;
static int thick = 2;
bool paint_flag=false;
bool inpainting_flag=false;

void inpainting(cv::Mat);


void onMouse( int event, int x, int y, int flag, void* )
{
	int i,j,p,q;
	static bool leftdown=false;

	switch(event){
	case cv::EVENT_LBUTTONDOWN:
		leftdown=true;
		if(paint_flag==true){
			for(y-thick>=0 ? i=y-thick : i = 0 ; i<=y+thick && i<imHeight ; i++){	
				for(x-thick>=0 ? j=x-thick : j = 0 ; j<=x+thick && j<imWidth ; j++){
					image.data[i*imWidth*3+j*3+0]=BLUE;
					image.data[i*imWidth*3+j*3+1]=GREEN;
					image.data[i*imWidth*3+j*3+2]=RED;
				}
			}	
		}

		break;
	case cv::EVENT_LBUTTONUP:
		leftdown=false;
		break;
	case cv::EVENT_MOUSEMOVE:
		if(leftdown==true && paint_flag==true){
			for(y-thick>=0 ? i=y-thick : i = 0 ; i<=y+thick && i<imHeight ; i++){	
				for(x-thick>=0 ? j=x-thick : j = 0 ; j<=x+thick && j<imWidth ; j++){
					image.data[i*imWidth*3+j*3+0]=BLUE;
					image.data[i*imWidth*3+j*3+1]=GREEN;
					image.data[i*imWidth*3+j*3+2]=RED;
				}
			}	
		}

		break;
	default:
		break;
	}
}

int main(int argc, char** argv) 
{
	char filename[64];
	int key;

	printf("Input image: ");
	scanf("%s",filename);

	/*cv::Mat original_image = cv::imread("image/hito.bmp");
	imWidth = original_image.cols;
	imHeight = original_image.rows;
	original_image.copyTo(image);*/

	cv::Mat original_image = cv::imread(filename,1);
	if(original_image.empty()){
		return -1; 
	}else{
		printf("%s was successfully loaded.\n",filename);
		imWidth = original_image.cols;
		imHeight = original_image.rows;
		original_image.copyTo(image);
	}

	cv::namedWindow("image inpainting", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::setMouseCallback("image inpainting", onMouse, 0);

	printf("Command list\n");
	printf("i: Begin inpainting\n");
	printf("o: Open an image\n");
	printf("s: Save an image as output.bmp\n");
	printf("r: Reload input image\n");
	printf("p: Change paint mode (paint missing regions or not)\n");
	printf("Esc: Quit\n");

	while(1){

		key=cv::waitKey(10);

		if(key==27){//ESC
			break;
		}else if(key=='i'){
			inpainting_flag=true;
			inpainting(image);
		}

		if(inpainting_flag==false){

			if(key=='o'){
				printf("Input image: ");

				scanf("%s",filename);

				original_image = cv::imread(filename,1);
				if(original_image.empty()){
					printf("%s was not loaded.\n",filename);
				}else{
					printf("%s was successfully loaded.\n",filename);
					imWidth = original_image.cols;
					imHeight = original_image.rows;
					original_image.copyTo(image);
				}

			}if(key=='p'){
				if(paint_flag==false){
					paint_flag=true;
					printf("You can paint missing regions.\n");
				}else{
					paint_flag=false;
					printf("You cannot paint missing regions.\n");
				}
			}else if(key=='r'){
				original_image.copyTo(image);
				printf("Reload %s\n",filename);
			}else if(key=='s'){
				printf("Output 'output.bmp'\n");
				cv::imwrite("output.bmp",image);
			}else if(key=='x'){
				thick++;
				printf("paint region %d x %d pixels\n",2*thick+1,2*thick+1);
			}else if(key=='z'){
				thick--;
				if(thick==-1){
					thick=0;
				}
				printf("paint region %d x %d pixels\n",2*thick+1,2*thick+1);

			}
		}

		cv::imshow("image inpainting",image);
	}

	return 0;
}
