
#include <string>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

const string trackbarWindowName = "Trackbars";

int threshold1;
int const max_threshold1 = 100;
int threshold2;
int const max_threshold2 = 300;


int ratio = 3;
int kernel_size = 3;
const char* window_name = "Canny";

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}

void createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "threshold1", threshold1);
	sprintf( TrackbarName, "threshold2", threshold2);

	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "threshold1", trackbarWindowName, &threshold1, max_threshold1, on_trackbar );
	createTrackbar( "threshold2", trackbarWindowName, &threshold2, max_threshold2, on_trackbar );

}


int main(int, char**)
{
    createTrackbars();

    VideoCapture cap(0); // open the default camera

    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    Mat edges2;
    namedWindow("edges",1);
    namedWindow("edges2",2);


    for(;;)
    {
        Mat frame;

        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);


        bilateralFilter(edges, edges2, 3, 150, 150);
        medianBlur(edges2, edges2, 15);

        Canny(edges, edges, threshold1, threshold2, 3);
        Canny(edges2, edges2, threshold1, threshold2, 3);

        imshow("edges", edges);
        imshow("edges2", edges2);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

