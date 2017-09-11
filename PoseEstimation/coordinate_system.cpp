//Title:  coordinate_system.cpp
//Author: Nicholas Ballard

#include <stdio.h>
#include <string>
#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;



// Globals ----------------------------------------------------------------------------------------

int boardHeight = 5;
int boardWidth = 7;
Size cbSize = Size(boardHeight,boardWidth);

string filename = "/home/alyssa/Área de Trabalho/esse/3DPoseEstimation/src/out_camera_data.yml";

bool doneYet = false;

//default image size
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//function prototypes
//void generate_boardPoints();




// Main -------------------------------------------------------------------------------------------
int main()
{

	//set up a FileStorage object to read camera params from file
	FileStorage fs;
	fs.open(filename, FileStorage::READ);
	// read camera matrix and distortion coefficients from file
	Mat intrinsics, distortion;
	fs["Camera_Matrix"] >> intrinsics;
	fs["Distortion_Coefficients"] >> distortion;
	// close the input file
	fs.release();




	//set up matrices for storage
	Mat webcamImage, gray, one;
	Mat rvec = Mat(Size(3,1), CV_64F); //CV_64F = grayscale, matriz 3x1 preto e branco
	Mat tvec = Mat(Size(3,1), CV_64F);

	//setup vectors to hold the chessboard corners in the chessboard coordinate system and in the image
	vector<Point2d> imagePoints, imageFramePoints, imageOrigin; //guarda coordenadas xy
	vector<Point3d> boardPoints, framePoints;//guarda coordenadas xyz


	//generate vectors for the points on the chessboard
	for (int i=0; i<boardWidth; i++)
	{
		for (int j=0; j<boardHeight; j++)
		{
			boardPoints.push_back( Point3d( double(i), double(j), 0.0) ); //gera pontos da borda da placa
		}
	}
	//generate points in the reference frame //pontos das coordenadas
	framePoints.push_back( Point3d( 0.0, 0.0, 0.0 ) );
	framePoints.push_back( Point3d( 5.0, 0.0, 0.0 ) );
	framePoints.push_back( Point3d( 0.0, 5.0, 0.0 ) );
	framePoints.push_back( Point3d( 0.0, 0.0, 5.0 ) );


	//set up VideoCapture object to acquire the webcam feed from location 0 (default webcam location)
	VideoCapture capture;
	capture.open(1);
	//set the capture frame size
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

	while(!doneYet)
	{
		 //store image to matrix
		 capture.read(webcamImage);

		 //make a gray copy of the webcam image
		 cvtColor(webcamImage,gray,COLOR_BGR2GRAY);


		 //detect chessboard corners
		 bool found = findChessboardCorners(gray, cbSize, imagePoints, CALIB_CB_FAST_CHECK); //output eh o imagePoints
		 //drawChessboardCorners(webcamImage, cbSize, Mat(imagePoints), found);


		 /*AQUI COMEÇA O TESTE*/
		 /* SE CONSEGIUR SUBSTITUIR OS PONTOS do imageFramePoints, TA OK*/
        int i;
        for (i=0; i<imageFramePoints.size(); i++){
            cout<< imageFramePoints[i]<<endl;
        }


		 //find camera orientation if the chessboard corners have been found
		 if ( found )
		 {
			 //find the camera extrinsic parameters
			 solvePnP( Mat(boardPoints), Mat(imagePoints), intrinsics, distortion, rvec, tvec, false );

			 //project the reference frame onto the image
			 projectPoints(framePoints, rvec, tvec, intrinsics, distortion, imageFramePoints );


			 //DRAWING
			 //draw the reference frame on the image
			 circle(webcamImage, (Point) imagePoints[0], 4 ,CV_RGB(255,0,0) );

			 Point one, two, three;
			 one.x=10; one.y=10;
			 two.x = 60; two.y = 10;
			 three.x = 10; three.y = 60;

			 line(webcamImage, one, two, CV_RGB(255,0,0) );
			 line(webcamImage, one, three, CV_RGB(0,255,0) );


			 line(webcamImage, imageFramePoints[0], imageFramePoints[1], CV_RGB(255,0,0), 2 );
			 line(webcamImage, imageFramePoints[0], imageFramePoints[2], CV_RGB(0,255,0), 2 );
			 line(webcamImage, imageFramePoints[0], imageFramePoints[3], CV_RGB(0,0,255), 2 );



			 //show the pose estimation data
			 cout << fixed << setprecision(2) << "rvec = ["
				  << rvec.at<double>(0,0) << ", "
				  << rvec.at<double>(1,0) << ", "
				  << rvec.at<double>(2,0) << "] \t" << "tvec = ["
				  << tvec.at<double>(0,0) << ", "
				  << tvec.at<double>(1,0) << ", "
				  << tvec.at<double>(2,0) << "]" << endl;

		 }

		 //show the image on screen
		 namedWindow("OpenCV Webcam", 0);
		 imshow("OpenCV Webcam", webcamImage);


		 //show the gray image
		 //namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
		 //imshow("Gray Image", gray);


		 waitKey(10);
	}

	return 0;
}
