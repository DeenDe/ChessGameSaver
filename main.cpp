#include <iostream>

using namespace std;

int main()
{
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;



int main(int argc, char** argv)
{

    // Wczytywanie obrazu
    string path = string(PROJECT_SOURCE_DIR) + "/data/wm_lab_3/szachy_3.mp4";
    VideoCapture capture(path);
    cv::Mat src;
    capture>>src;
    if (src.empty()) return -1;
     cvtColor(src, src, CV_RGB2GRAY);
     adaptiveThreshold(src, src, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 17);
    //Canny(src, src, 255, 200, 3);

     Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(5,5));

     morphologyEx(src, src, MORPH_CLOSE, kernel);
    // morphologyEx(src, src, MORPH_OPEN, Mat());


     vector<vector<Point> > contours;
     findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

         // Analiza powierzchni konturow

     float maxpropotion;
     float propotion;
     int biggest;
     int second;
     cvtColor(src, src, CV_GRAY2RGB);
     for (int i = 0; i<contours.size(); i++)
         {
             float area = contourArea(contours[i]);
             float length = arcLength(contours[i], true);
             if ( i == 0) maxpropotion = 0;
             if(length > 0)
             {
                 propotion = area/length;
                 if(propotion > maxpropotion)
                 {
                     maxpropotion = propotion;
                     second = biggest;
                     biggest = i;
                 }
             }
         }
    float match;
    drawContours(src, contours, second, Scalar(255,0,0), 2, 8);
    int xmin, xmax, ymin, ymax, square_area;
    for(int i = 0; i<contours[second].size(); i++)
    {
        if( i ==0){
            xmin = contours[second][i].x;
            xmax = contours[second][i].x;
            ymin = contours[second][i].y;
            ymax = contours[second][i].y;
        }
        if(contours[second][i].x>xmax) xmax =  contours[second][i].x;
        if(contours[second][i].x<xmin) xmin =  contours[second][i].x;
        if(contours[second][i].y>ymax) ymax =  contours[second][i].y;
        if(contours[second][i].y<ymin) ymin =  contours[second][i].y;
    }
    square_area = (xmax-xmin)*(ymax-ymin);
    int y = ymax - ymin;
    int x = xmax - xmin;
    for (int i = 0; i<contours.size(); i++)
    {
        for(int j = 0; j<contours[i].size(); j++)
        {
            if( j ==0){
                xmin = contours[i][j].x;
                xmax = contours[i][j].x;
                ymin = contours[i][j].y;
                ymax = contours[i][j].y;
            }
            if(contours[i][j].x>xmax) xmax =  contours[i][j].x;
            if(contours[i][j].x<xmin) xmin =  contours[i][j].x;
            if(contours[i][j].y>ymax) ymax =  contours[i][j].y;
            if(contours[i][j].y<ymin) ymin =  contours[i][j].y;
        }
    int square_area2 = (xmax-xmin)*(ymax-ymin);
    if(xmax- xmin >x*0.9 && xmax - xmin < x*1.1 &&  ymax- ymin >y*0.9 && ymax - ymin < y*1.1 && (square_area2>square_area*0.9) && (square_area2<square_area*1.2)) drawContours(src, contours, i, Scalar(255,0,0), 2, 8);
    }



     imshow("source", src);


     waitKey();
}

}
