#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;
void on_low_r_thresh_trackbar(int, void *);
void on_high_r_thresh_trackbar(int, void *);
void on_low_g_thresh_trackbar(int, void *);
void on_high_g_thresh_trackbar(int, void *);
void on_low_b_thresh_trackbar(int, void *);
void on_high_b_thresh_trackbar(int, void *);

void pausevideo(int, void *);

int low_r=0, low_g=0, low_b=0;
int high_r=240, high_g=240, high_b=66;
int pause = 0;

void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3), Point(1, 1));

	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(3,3),Point(1, 1));

	//erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	//dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

int main(int argc, char** argv)
{
    VideoCapture cap(argv[1]); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    Mat filtrocor;
    Mat withline;
    namedWindow("original", WINDOW_AUTOSIZE);
    namedWindow("color filter", WINDOW_AUTOSIZE);
    namedWindow("withline",WINDOW_AUTOSIZE);
  //  namedWindow("teste", WINDOW_AUTOSIZE);
/*
    resizeWindow("original",  300,  300);
    resizeWindow("color filter",  600, 600);
    resizeWindow("withline",  600,  600);
*/
    for(;;)
    {

    createTrackbar("Low R","color filter", &low_r, 255, on_low_r_thresh_trackbar);
    createTrackbar("High R","color filter", &high_r, 255, on_high_r_thresh_trackbar);
    createTrackbar("Low G","color filter", &low_g, 255, on_low_g_thresh_trackbar);
    createTrackbar("High G","color filter", &high_g, 255, on_high_g_thresh_trackbar);
    createTrackbar("Low B","color filter", &low_b, 255, on_low_b_thresh_trackbar);
    createTrackbar("High B","color filter", &high_b, 255, on_high_b_thresh_trackbar);
    createTrackbar("Pause","original", &pause, 1, pausevideo);

        Mat frame;
        if (pause != 1)
            cap >> frame; // get a new frame from camera


        cvtColor(frame,filtrocor,COLOR_BGR2HSV);

        inRange(filtrocor,Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),filtrocor);
        morphOps(filtrocor);
       // cvtColor(filtrocor, edges, COLOR_BGR2GRAY);




       //GaussianBlur(filtrocor, edges, Size(7,7), 1.5, 1.5);
        bilateralFilter(filtrocor, edges, 3, 150, 150);
        Canny(filtrocor, edges, 10, 100, 3);
        cvtColor(edges, withline, CV_GRAY2BGR);

        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI/180, 20, 30, 30 );
        /*
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        dst: Output of the edge detector. It should be a grayscale image (although in fact it is a binary one)
        lines: A vector that will store the parameters (x_{start}, y_{start}, x_{end}, y_{end}) of the detected lines
        rho : The resolution of the parameter r in pixels. We use 1 pixel.
        theta: The resolution of the parameter \theta in radians. We use 1 degree (CV_PI/180)
        threshold: The minimum number of intersections to “detect” a line
        minLinLength: The minimum number of points that can form a line. Lines with less than this number of points are disregarded.
        maxLineGap: The maximum gap between two points to be considered in the same line.
*/
    if (lines.size()>=2){
    Point ponto1(lines[0][0], lines[0][1]);
    Point ponto2(lines[0][2], lines[0][3]);
    Point ponto3(lines[1][0], lines[1][1]);
    Point ponto4(lines[1][2], lines[1][3]);

        float length1 = sqrt(pow((lines[0][0]-lines[0][2]),2) + pow((lines[0][1]-lines[0][3]),2));
    float length2 = sqrt(pow((lines[1][0]-lines[1][2]),2) + pow((lines[1][1]-lines[1][3]),2));

    float dif = abs(length1-length2);

    if (dif > 3)
    {
        cout <<"se a difereca for muito grande, vai fzr alguma coisa indicando q algo esta esquisito"<<endl;
    cout << "saida do lines.\n"<<lines[0]<<"\n"<<lines[1]<<endl;
 cout << "sao pontos dos cantos, output do programa.\n"<<ponto1<<"\n"<< ponto2<<"\n"<< ponto3<<"\n"<< ponto4<<"\n"<<endl;
 cout << "diferenca entre comprimento dos lados detectados :" << dif <<endl;
cout << "comprimento da linha 1 :" << length1<<endl;
cout << "comprimento da linha 2 :" << length2<<endl;


   }}

          for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( withline, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }

        imshow("original", frame);
        imshow("withline", withline);
        imshow("color filter", filtrocor);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}



void on_low_r_thresh_trackbar(int, void *)
{
    low_r = min(high_r-1, low_r);
    setTrackbarPos("Low R","color filter", low_r);
}
void on_high_r_thresh_trackbar(int, void *)
{
    high_r = max(high_r, low_r+1);
    setTrackbarPos("High R", "color filter", high_r);
}
void on_low_g_thresh_trackbar(int, void *)
{
    low_g = min(high_g-1, low_g);
    setTrackbarPos("Low G","color filter", low_g);
}
void on_high_g_thresh_trackbar(int, void *)
{
    high_g = max(high_g, low_g+1);
    setTrackbarPos("High G", "color filter", high_g);
}
void on_low_b_thresh_trackbar(int, void *)
{
    low_b= min(high_b-1, low_b);
    setTrackbarPos("Low B","color filter", low_b);
}
void on_high_b_thresh_trackbar(int, void *)
{
    high_b = max(high_b, low_b+1);
    setTrackbarPos("High B", "color filter", high_b);
}

void pausevideo(int, void *)
{
    setTrackbarPos("Pause", "original", pause);
}

