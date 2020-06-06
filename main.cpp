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
    cv::Mat src, frame, prev, img, img2, img3;
    capture>>frame;
    src = frame.clone();
    if (src.empty()) return -1;
     cvtColor(src, src, CV_RGB2GRAY);

     adaptiveThreshold(src, src, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 17);
     Mat dst = src.clone();

     Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(6,6));

     morphologyEx(src, src, MORPH_CLOSE, kernel);

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
    vector<Moments> mu(16);
    int v = 0;
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
    if(xmax- xmin >x*0.9 && xmax - xmin < x*1.1 &&  ymax- ymin >y*0.9 && ymax - ymin < y*1.1 && (square_area2>square_area*0.9) && (square_area2<square_area*1.2) && (ymax>400)&&(ymax<850))
        {
            drawContours(src, contours, i, Scalar(255,0,0), 2, 8);
            mu[v] = moments(contours[i],false);
            v++;
        }
    }

    vector<Point2f> mc(64);
    for (int i = 0; i<mu.size();i++)
    {
        mc[i] = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

int j = 16;
int jj = 63;
    for(int i = 16; i>0;i--)
    {
        x = (mc[i-1].x-mc[i-2].x)/2;
        if(i-1>11)
        {
            y = (mc[i-1].y-mc[i-9].y)/2;
            mc[j] = Point( mc[i-1].x, mc[i-1].y+y );

            j++;


            mc[j] = Point( mc[i-1].x, mc[i-1].y+2*y );

            j++;


            if(i-1 != 15)
            {
                int xx = (mc[j-5].x-mc[j-2].x)/2;
                mc[jj] = Point(mc[j-5].x - xx, mc[j-5].y);

                jj--;
                if(i-1==14)
                {
                    mc[jj] = Point(mc[j-5].x + xx, mc[j-5].y);

                    jj--;
                }

                xx = (mc[j-4].x-mc[j-1].x)/2;
                mc[jj] = Point(mc[j-4].x - xx , mc[j-4].y);

                jj--;

                if(i-1==14)
                {
                    mc[jj] = Point(mc[j-4].x + xx, mc[j-4].y);

                    jj--;
                }
            }


        }
        if((i)>0&&(i)<4)
        {
            y = (mc[i-1].y-mc[i+7].y)/2;
            mc[j] = Point( mc[i-1].x, mc[i-1].y+y );

            j++;
            mc[j] = Point( mc[i-1].x, mc[i-1].y+2*y );

            j++;


                int xx = (mc[j-5].x-mc[j-2].x)/2;
                mc[jj] = Point(mc[j-5].x - xx, mc[j-5].y); //legit

                jj--;

                if(i-1==0)
                {
                    mc[jj] = Point(mc[j-5].x - 3*xx, mc[j-5].y);

                    jj--;
                }
                xx = (mc[j-4].x-mc[j-1].x)/2;
                mc[jj] = Point(mc[j-4].x - xx , mc[j-4].y);

                jj--;
                if(i-1==0)
                {
                    mc[jj] = Point(mc[j-5].x - 3*xx, mc[j-4].y);

                    jj--;
                }




        }
        if(i-1==12)
        {
            y = (mc[i-1].y-mc[i-9].y)/2;
            mc[j] = Point( mc[i-9].x, mc[i-9].y+y);

            j++;
            mc[j] = Point( mc[i-9].x, mc[i-9].y-y);

            j++;


        }

        if(i-1==3)
        {
            y = (mc[i-1].y-mc[i+7].y)/2;
            mc[j] = Point( mc[i+7].x, mc[i+7].y-y);

            j++;
            mc[j] = Point( mc[i+7].x, mc[i+7].y+y);

            j++;
            mc[j] = Point( mc[i-1].x, mc[i-1].y+y);

            j++;
            mc[j] = Point( mc[i-1].x, mc[i-1].y+2*y );

            j++;

        }
        if(i-1 == 12 || i-1 == 8 || i-1 == 4)
        {

        }
        else
        {
            if(i != 1)
            {
            mc[j] = Point( mc[i-1].x-x, mc[i-1].y );

            j++;
            }
        }
    }


vector<Point2f> up(64);
int pom =0;
Point2f luc=mc[0];
for( int i=0; i<mc.size(); i++)
{

    if(luc.x>mc[i].x&&luc.y>mc[i].y)
    {
        luc = mc[i];
        pom = i;
    }

}

x=0;

for(int j = 0; j<4; j++)
{
    for(int i =0; i<mc.size(); i++)
    {

        if((mc[i].x<=mc[pom+j].x+50)&&(mc[i].x>=mc[pom+j].x-50))
        {
            up[x] = mc[i];
            x++;
        }

    }
    for(int i =0; i<mc.size(); i++)
    {

        if((mc[i].x<=mc[pom+12-3*j].x+50)&&(mc[i].x>=mc[pom+12-3*j].x-50)&&j!=3)
        {
            up[x] = mc[i];
            x++;
        }
        if((mc[i].x<=mc[pom-1].x+50)&&(mc[i].x>=mc[pom-1].x-50)&&j==3)
        {
            up[x] = mc[i];
            x++;
        }
    }
}
int  zamiana;
Point2f schowek;
for (int j =0; j<8;j++)
{
    do
    {
        zamiana=0;//przed każdym "przejazdem" pętli for zmienna zamiana jest zerowana
        for (int i=0+j*8; i<8+8*j-1; i++)
        {
            if (up[i].y>up[i+1].y)// jeśli element tablicy jest większy od następnego elementu
            {
                zamiana=zamiana+1; //jeśli jest jakaś zmiana, to zmienne zamiana powiększa swoją wartość
                schowek=up[i];//wartość t[i] jest kopiowana do schowka
                up[i]=up[i+1];//t[i] przyjmuje wartość następnego elementu, gdyż jest on mniejszy
                up[i+1]=schowek;//kolejny element tablicy przejmuje wcześniejszą wartość poprzedniego elementu, gdyż jest on większy
            }
        }
    }
    while(zamiana!=0);
}

x=0;
y=0;
vector <string> plansza (64);
for(int i = 0; i <plansza.size(); i++)
{
    if(i==0||i==7||i==56||i==63) plansza[i] = "R";
    if(i==8||i==15||i==48||i==55) plansza[i] = "N";
    if(i==16||i==23||i==40||i==47) plansza[i] = "B";
    if(i==24||i==31) plansza[i] = "K";
    if(i==32||i==39) plansza[i] = "Q";
    if(i==1+8*x)
    {
        plansza[i] = "P";
        x++;
    }
    if(i==6+8*y)
    {
        plansza[i] = "P";
        y++;
    }
}
for(int i =0; i<plansza.size();i++)
{
    putText(src,plansza[i],up[i], FONT_HERSHEY_PLAIN, 6,  Scalar(0,0,255,255));
}


Mat kernel2 = getStructuringElement(CV_SHAPE_ELLIPSE, Size(6,6));

morphologyEx(dst, dst, MORPH_CLOSE, kernel2);
//morphologyEx(dst, dst, MORPH_OPEN, Mat());
//GaussianBlur( dst, dst, Size(9, 9), 2, 2 );






vector<vector<Point> > contours2;
findContours(dst, contours2, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
cvtColor(dst, dst, CV_GRAY2RGB);
vector<Moments> mu2;

for(int i =0; i<contours2.size();i++)
{
    if(contourArea(contours2[i])>1000)
    {
        drawContours(dst, contours2, i, Scalar(255,0,0), 2, 8);
        mu2.push_back(moments(contours[i],false));
    }
}
vector<Point2f> mc2;
for (int i = 0; i<mu2.size();i++)
{

    mc2.push_back(Point( mu2[i].m10/mu2[i].m00 , mu2[i].m01/mu2[i].m00 ));

}

for (int i = 0; i<mc2.size();i++)
{

        circle(dst, mc2[i],6,Scalar(0,255,0));
}
Mat Template = frame.clone();
Mat video;
cvtColor(Template, Template, CV_BGR2GRAY);
int movement = 10000000;
int first = 0;
cvtColor(frame, prev, CV_BGR2GRAY);
int licznik = 0;
while(true)
{
    // Sprawdzenie czy jest koniec filmu
    if(!capture.read(frame)) break;
    video = frame.clone();

    // Skala szarosci
    cvtColor(frame, frame, CV_BGR2GRAY);

    // Roznicowanie klatek
   absdiff(frame, prev, img);
    // Binaryzacja
    threshold(img, img, 20, 255, THRESH_BINARY);
    // Morfologiczne otwarcie
   morphologyEx(img, img, MORPH_OPEN, Mat());
   morphologyEx(img,img, MORPH_CLOSE, Mat());
    // Wyznaczenie konturow

    vector<vector<Point> > contours3;
    findContours(img, contours3, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    if(movement == 0 )
    {
        absdiff(Template, frame, img3);
        threshold(img3, img3, 40, 255, THRESH_BINARY);
        // Morfologiczne otwarcie
       Mat kernel2 = getStructuringElement(CV_SHAPE_ELLIPSE, Size(6,6));
       morphologyEx(img3, img3, MORPH_OPEN, kernel2);
       morphologyEx(img3,img3, MORPH_CLOSE, Mat());
       vector<vector<Point> > contours4;
       findContours(img3, contours4, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
       cvtColor(img3, img3, CV_GRAY2RGB);
       vector<Point2f> mc3;
       int z =0;
       for(int i =0; i< contours4.size();i++)
       {
           if(contourArea(contours4[i])>300)
           {
               drawContours(img3, contours4, i, Scalar(255,0,0), 2, 8);
               Moments mu3= moments(contours4[i],false);
               mc3.push_back(Point( mu3.m10/mu3.m00 , mu3.m01/mu3.m00 ));
           }
       }
       int checked = 0;
       string pom;
       int pom2 = 0;
       for(int i =0; i<up.size();i++)
       {
           for(int j = 0; j<mc3.size(); j++)
           {
               if(mc3[j].x>(up[i].x - 25) && mc3[j].x<(up[i].x + 25) && mc3[j].y>(up[i].y - 25) && mc3[j].y<(up[i].y + 25))
               {
                   checked = 1;
               }
           }
           if(checked == 1)
           {
               if(plansza[i] == "")
               {
                   pom2 = i;
               }
               if(plansza[i] != "")
               {
                   pom = plansza[i];
                   plansza [i] = "";
               }

           }
           checked = 0;
       }
       plansza [pom2] = pom;

      // imshow("frame", img3);
      // imshow("temp", Template);
      // waitKey(0);
       Template = frame;
    }
    cvtColor(img, img2, CV_GRAY2RGB);
    for(int i =0; i<contours3.size();i++)
    {
        drawContours(img2, contours3, i, Scalar(255,0,0), 2, 8);

    }

    if(contours3.size()>100)
    {
        movement = 10;
    }
    if(contours3.size()<30 &&  licznik >5) movement = movement - 1;
    string number = std::to_string(contours3.size());
   // putText(img2,number,up[0], FONT_HERSHEY_PLAIN, 6,  Scalar(0,0,255,255));
    //putText(img2,to_string(licznik),up[15], FONT_HERSHEY_PLAIN, 6,  Scalar(0,0,255,255));
    for(int i =0; i<plansza.size();i++)
    {
        putText(video,plansza[i],up[i], FONT_HERSHEY_PLAIN, 6,  Scalar(0,0,255,255));
    }
    imshow("vid", video);
    waitKey(30);

    // Zapisanie ;poprzedniej klatki
    prev = frame.clone();
    licznik++;

}

//     imshow("source", dst);


     waitKey();
}
