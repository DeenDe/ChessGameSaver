#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>


using namespace std;
using namespace cv;

// Funkcja zwracajaca dane pole w stringu
string pole(int i)
{
    string pol;
    if(i>=0&&i<=7) pol = "H" + to_string(i+1);
    if(i>=8&&i<=15) pol = "G" + to_string(i+1-8);
    if(i>=16&&i<=23) pol = "F" + to_string(i+1-16);
    if(i>=24&&i<=31) pol = "E" + to_string(i+1-24);
    if(i>=32&&i<=39) pol = "D" + to_string(i+1-32);
    if(i>=40&&i<=47) pol = "C" + to_string(i+1-40);
    if(i>=48&&i<=55) pol = "B" + to_string(i+1-48);
    if(i>=56&&i<=63) pol = "A" + to_string(i+1-56);
    return pol;
}


int main(int argc, char** argv)
{

    // Wczytywanie obrazu oraz pobranie pierwszej klatki
     string path = string(PROJECT_SOURCE_DIR) + "/data/wm_lab_3/szachy_3.mp4";
     VideoCapture capture(path);
     cv::Mat src, frame, prev, img, img2, img3;
     capture>>frame;
     src = frame.clone();
     if (src.empty()) return -1;
     cvtColor(src, src, CV_RGB2GRAY);
     // Progowanie i operacje morfologiczne
     adaptiveThreshold(src, src, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 17);
     Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(6,6));
     morphologyEx(src, src, MORPH_CLOSE, kernel);

     // Analiza powierzchni konturow
     vector<vector<Point> > contours;
     findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);     

     float maxpropotion;
     float propotion;
     int biggest;
     int second;
     cvtColor(src, src, CV_GRAY2RGB);
     // Znalezienie konturu pola, jest jako drugi
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
    // Znalezienie skrajnych punktow konturu pola
    vector<Moments> mu(16);
    int v = 0;    
    int xmin, xmax, ymin, ymax, square_area;
    for(int i = 0; i<contours[second].size(); i++)
    {
        if( i == 0)
        {
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
    // Odnalezienie kolejnych srodkowych, bialych pol
    for (int i = 0; i<contours.size(); i++)
    {
        for(int j = 0; j<contours[i].size(); j++)
        {
            if( j ==0)
            {
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
            mu[v] = moments(contours[i],false);
            v++;
        }
    }
    // Znalezienie srodkow wykrytych pol
    vector<Point2f> mc(64);
    for (int i = 0; i<mu.size();i++)
    {
        mc[i] = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    // Zdefiniowanie calej planszy
    int j = 16; // tyle mamy pol, od tego zaczynaja sie kolejne
    int jj = 63; // ostatni indeks wektora, w celu prawidlowego znajdowania niektorych pol

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
            mc[j] = Point( mc[i-1].x, mc[i-1].y+2*y);
            j++;
        }
        if(i-1 == 12 || i-1 == 8 || i-1 == 4);

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

// Znalezienie pola w lewym gornym rogu
for( int i=0; i<mc.size(); i++)
{
    if(luc.x>mc[i].x&&luc.y>mc[i].y)
    {
        luc = mc[i];
        pom = i;
    }
}
x=0;
// Uporzadkowanie pol w kolumnach
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
// Uporzadkowanie pol w prawidlowej kolejnosci
int  zamiana;
Point2f schowek;
for (int j =0; j<8;j++)
{
    do
    {
        zamiana=0;
        for (int i=0+j*8; i<8+8*j-1; i++)
        {
            if (up[i].y>up[i+1].y)
            {
                zamiana=zamiana+1;
                schowek=up[i];
                up[i]=up[i+1];
                up[i+1]=schowek;
            }
        }
    }
    while(zamiana!=0);
}
// Zdefiniowanie poczatkowego ustawienia bierek
x=0;
y=0;
vector <string> plansza (64);
for(int i = 0; i <plansza.size(); i++)
{
    if(i==0||i==56) plansza[i] = "WR";
    if(i==7||i==63) plansza[i] = "BR";
    if(i==8||i==48) plansza[i] = "WN";
    if(i==15||i==55) plansza[i] = "BN";
    if(i==16||i==40) plansza[i] = "WB";
    if(i==23||i==47) plansza[i] = "BB";
    if(i==24) plansza[i] = "WK";
    if(i==31) plansza[i] = "BK";
    if(i==32) plansza[i] = "WQ";
    if(i==39) plansza[i] = "BQ";
    if(i==1+8*x)
    {
        plansza[i] = "WP";
        x++;
    }
    if(i==6+8*y)
    {
        plansza[i] = "BP";
        y++;
    }
}

Mat Template = frame.clone();
Mat video;
cvtColor(Template, Template, CV_BGR2GRAY);
int movement = 10000000;
bool w_or_b = 0; //ustala czy na posunieciu sa biale czy czarne
cvtColor(frame, prev, CV_BGR2GRAY);
ofstream zapis("partia.txt");
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
    // Wykrycie braku ruchu i sprawdzenie przemieszczenia
    if(movement == 0 )
    {
       absdiff(Template, frame, img3);
       threshold(img3, img3, 40, 255, THRESH_BINARY);
       Mat kernel2 = getStructuringElement(CV_SHAPE_ELLIPSE, Size(6,6));
       morphologyEx(img3, img3, MORPH_OPEN, kernel2);
       morphologyEx(img3,img3, MORPH_CLOSE, Mat());
       vector<vector<Point> > contours4;
       findContours(img3, contours4, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
       cvtColor(img3, img3, CV_GRAY2RGB);
       vector<Point2f> mc3;
       for(int i =0; i< contours4.size();i++)
       {           
           if(contourArea(contours4[i])>300)
           {
               Moments mu3= moments(contours4[i],false);
               mc3.push_back(Point( mu3.m10/mu3.m00 , mu3.m01/mu3.m00 ));
           }
       }
       int checked = 0;
       string pom, pom1;
       int pom2 = 100;
       int pom3 = 100;
       int pom4 = 100;
       int bicie = 0;
       for(int i =0; i<up.size();i++)
       {
           for(int j = 0; j<mc3.size(); j++)
           {
               if(mc3[j].x>(up[i].x - 30) && mc3[j].x<(up[i].x + 30) && mc3[j].y>(up[i].y - 30) && mc3[j].y<(up[i].y + 30))
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
               if(plansza[i] != "" && bicie==0)
               {
                   pom = plansza[i];
                   pom3=i;
                   bicie =1;
               }
               if(plansza[i] != "" && bicie==1)
               {
                   pom1 = plansza[i];
                   pom4 = i;
               }

           }
           checked = 0;
       }

       if(pom2!=100)
       {
            plansza [pom2] = pom;
            plansza [pom3] = "";
            if(pom[1] != 'P')
            {
                string pol = pole(pom2);
                if(w_or_b == 0)
                    zapis<<pom[1]<<pol<<" ";
                if(w_or_b == 1)
                    zapis<<pom[1]<<pol<<endl;
            }
            if(pom[1] == 'P')
            {
                string pol = pole(pom2);
                if(w_or_b == 0)
                    zapis<<pol<<" ";
                if(w_or_b ==1)
                    zapis<<pol<<endl;
            }
       }

       if(pom2 == 100)
       {
           int white = 0;
           if(pom[0]=='W') white = 1;

           if(w_or_b == 0 && white == 1)
           {
               plansza[pom4] = pom;
               plansza[pom3] = "";
               if(pom[1] != 'P')
               {
                   string pol = pole(pom4);
                   zapis<<pom[1]<<"x"<<pol<<" ";
               }
               if(pom[1] == 'P')
               {
                   string pol = pole(pom4);
                   zapis<<x<<pol<<" ";
               }
           }

           if(pom1[0]=='W') white = 2;
           if(w_or_b == 0 && white == 2)
           {
               plansza[pom3] = pom;
               plansza[pom4] = "";
               if(pom[1] != 'P')
               {
                   string pol = pole(pom3);
                   zapis<<pom[1]<<"x"<<pol<<" ";
               }
               if(pom[1] == 'P')
               {
                   string pol = pole(pom3);
                   zapis<<x<<pol<<" ";
               }
           }

           if(w_or_b == 1 && white == 1)
           {
               plansza[pom4] = "";
               plansza[pom3] = pom1;
               if(pom[1] != 'P')
               {
                   string pol = pole(pom3);
                   zapis<<pom[1]<<"x"<<pol<<endl;
               }
               if(pom[1] == 'P')
               {
                   string pol = pole(pom3);
                   zapis<<x<<pol<<endl;
               }
           }

           if(w_or_b == 1 && white == 2)
           {
               plansza[pom4] = pom1;
               plansza[pom3] = "";
               if(pom[1] != 'P')
               {
                   string pol = pole(pom4);
                   zapis<<pom[1]<<"x"<<pol<<endl;
               }
               if(pom[1] == 'P')
               {
                   string pol = pole(pom4);
                   zapis<<x<<pol<<endl;
               }
           }
       }

       if(w_or_b == 0) w_or_b = 1;
       if(w_or_b == 1) w_or_b = 0;
       Template = frame;
    }

    cvtColor(img, img2, CV_GRAY2RGB);

    if(contours3.size()>30)
    {
        movement = 15;
    }

    if(contours3.size()<2) movement = movement - 1;

    for(int i =0; i<plansza.size();i++)
    {
        putText(video, plansza[i], Point(up[i].x-60, up[i].y+30), FONT_HERSHEY_PLAIN, 5, Scalar(0,0,255,255));

    }
    imshow("vid", video);
    char key = (char) cv::waitKey(30);
    if (key == 27) break;

    // Zapisanie poprzedniej klatki
    prev = frame.clone();
   }
    zapis.close();
    waitKey();
}
