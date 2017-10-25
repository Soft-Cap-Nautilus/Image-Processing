#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{
 const char* filename = argc >= 2 ? argv[1] : "pic1.jpg";

 Mat src = imread(filename, 0);
 if(src.empty())
 {
     help();
     cout << "can not open " << filename << endl;
     return -1;
 }

 Mat dst, cdst;
 //bilateralFilter(src, src, 3, 100, 100);
 //se fizesse filtro por cor seria bem melhor hein
 medianBlur(src, src, 3);
 Canny(src, dst, 10,100, 3);
 cvtColor(dst, cdst, CV_GRAY2BGR);




  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 30 );

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
    }
cout << "saida do lines.\n"<<lines[0]<<"\n"<<lines[1]<<endl;
 cout << "sao pontos dos cantos, output do programa.\n"<<ponto1<<"\n"<< ponto2<<"\n"<< ponto3<<"\n"<< ponto4<<"\n"<<endl;
 cout << "diferenca entre comprimento dos lados detectados :" << dif <<endl;
cout << "comprimento da linha 1 :" << length1<<endl;
cout << "comprimento da linha 2 :" << length2<<endl;

  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }

 imshow("source", src);
 imshow("detected lines", cdst);

 waitKey();

 return 0;
}
