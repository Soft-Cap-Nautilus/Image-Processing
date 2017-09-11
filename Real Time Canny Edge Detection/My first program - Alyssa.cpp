//UFRJ Nautilus - My first program - Alyssa
//15/07/2017 21:44
//Pega a imagem da camera webcam(input), passa por filtro cany(output) e mostra o input e output ao mesmo tempo



#include <stdio.h>
#include <cv.h>
#include <highgui.h>

using namespace std;

main( int argc, char* argv[] ) {

	cvNamedWindow( "Original", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Cany", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateCameraCapture( 1 );

	int counter;

	if (!capture){
        printf("Deu erro ai no webcam");
		return -1;
	}

	IplImage* original_frame;
    IplImage* median_frame;

	double fps = 50;
	printf( "%s\nfps=%d\n", argv[1], (int)fps );
	CvSize size = cvSize( 640, 480 );

    IplImage* cany_frame = cvCreateImage(
        size,
        IPL_DEPTH_8U,
        1);


	while( (original_frame=cvQueryFrame(capture)) != NULL ) {


        cvCanny( original_frame, cany_frame, 50, 100, 3);



		cvShowImage( "Original", original_frame );



            cvShowImage( "Cany", cany_frame  );


		char c = cvWaitKey(10);
		if( c == 27 ) break;
	}

	cvReleaseImage( &cany_frame );
	cvReleaseCapture( &capture );
	cvDestroyWindow( "Original" );
    cvDestroyWindow( "Canny" );
}
