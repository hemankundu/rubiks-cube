//g++ cameraInput.cpp `pkg-config opencv --cflags --libs`
//g++ rubikCube.cpp `pkg-config opencv --cflags --libs`
//g++ rubikCube.cpp `pkg-config opencv --cflags --libs` -lserial -lpthread -std=c++11 -pthread


//LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libv4l/v4l1compat.so" ./a.out
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include "serial.cpp"

using namespace cv;
using namespace std;


string winName="input";
int GAP=75, SIZE=25, WINDOWX=800, WINDOWY=600;

int X=(WINDOWX/2)-(GAP + 1.5*SIZE)-145;
int Y=(WINDOWY/2)-(GAP + 1.5*SIZE);

Scalar m[3][3]={Scalar(255)};
char clrs[3][3]={'\0'};
const char outFileName[]="cube-color.txt", configFileName[]="color-config.txt";
double prc=10;
//vxy recPos={{}};

class color{
    public:
    char clr;
    double min[3], max[3]; //min GBR and Max GBR
    color(){
        for(int i=0;i<3;i++){
            min[i]=256;
            max[i]=-1;
        }
    }
};

color colors[6];

char getClr(Scalar &s){
    int i,j;
    for(i=0;i<6;i++){
        for(j=0;j<3;j++){
            if(s[j]<colors[i].min[j]-prc || s[j]>colors[i].max[j]+prc ){
                break;
            }
        }
        if(j>=3){
            break;
        }
    }
    if(i>=6){
        cout<<"[ERROR] Can not determine ["<<s[0]<<", "<<s[1]<<", "<<s[2]<<"] this color. Please rescan or reconfigure.\n";
        return 'u';
    }
    return colors[i].clr;
}

bool readColors(Mat& frame){ 
    bool success=true;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            int x=X+(i*GAP), y=Y+(j*GAP);
            Mat mask= Mat::zeros(frame.rows, frame.cols, CV_8U);
            mask(Rect(x,y, SIZE, SIZE))=1;
            m[i][j]=mean(frame, mask);   
            char ch=getClr(m[i][j]);
            if(ch=='u'){
                success=false;
            }
            clrs[i][j]=ch;
        }
    }
    return success;
}

void adjustFrame(Mat &frame){
    Mat hsv;
    cvtColor(frame, hsv, CV_BGR2HSV);
    const unsigned char hue_shift = 0, sat_shift=10;

    for (int j = 0; j < frame.rows; j++){
        for (int i = 0; i < frame.cols; i++){
            signed short h = hsv.at<Vec3b>(j, i)[0];
            signed short s = hsv.at<Vec3b>(j, i)[1];
            signed short h_plus_shift = h;
            h_plus_shift += hue_shift;
            if (h_plus_shift < 0)
                h = 180 + h_plus_shift;
            else if (h_plus_shift > 180)
                h = h_plus_shift - 180;
            else
                h = h_plus_shift;
            hsv.at<Vec3b>(j, i)[0] = static_cast<unsigned char>(h);
            hsv.at<Vec3b>(j, i)[1]+=sat_shift;
        }
    }
    cvtColor(hsv, frame, CV_HSV2BGR);
    imshow(winName, frame);
}

bool configureColors(){
    FILE *cfgfp=fopen(configFileName, "w");
    if(cfgfp==NULL){
        cout<<"[ERROR] Color configuration file could not be created!\n";
        return false;
    }

    cout<<"Video device? \n:";
    int camid=0;
    cin>>camid;
    VideoCapture cap(camid);
    Mat frame, frame2;
    if(!cap.isOpened()){
        cout<<"[ERROR] Camera could not be opened!\n";
        return false;
    }
    cout<<"Camera opened.\n";
    string clrs="woyrbg";
    int c=0;
    bool read=false;
    cout<<"Bring "<<clrs[c++]<<" color..\n";
    Scalar scl=Scalar(255);
    for(;;){
        cap>>frame;
        
        frame.copyTo(frame2);
        //frame.
        namedWindow("configure", WINDOW_NORMAL);
        resizeWindow("configure", WINDOWX, WINDOWY);
        rectangle(frame2, Rect(X+GAP, Y+GAP, SIZE, SIZE), scl, 3);
        imshow("configure", frame2);
        int key=waitKey(20);
        if(key==27){
            //ESC
            cout<<"Camera closed.\n";
            return false;
        }else if(key==32){
            //SPACE
            Mat mask= Mat::zeros(frame.rows, frame.cols, CV_8U);
            mask(Rect(X+GAP, Y+GAP, SIZE, SIZE))=1;
            scl=mean(frame, mask);
            for(int i=0;i<3;i++){
                if(scl[i]<colors[c-1].min[i]){
                    colors[c-1].min[i]=scl[i];
                }
                if(scl[i]>colors[c-1].max[i]){
                    colors[c-1].max[i]=scl[i];
                }
            }
            read=true;
        }else if(key==10){
            //ENTER
            if(read){
                read=false;
                fprintf(cfgfp, "%c ", clrs[c-1]);
                for(int j=0;j<3;j++){
                    fprintf(cfgfp, "%lf %lf ", colors[c-1].min[j], colors[c-1].max[j]);
                }
                fprintf(cfgfp, "\n");
                if(c==6){
                    fclose(cfgfp);
                    cap.release();
                    destroyWindow("configure");
                    return true;
                }
                cout<<"Bring "<<clrs[c++]<<" color..\n";
                
            }else{
                cout<<"Read color first using SPACE\n";
            }
        }
    }
}

const int sliderVal_max = 100;
int sliderVal;

static void on_trackbar( int, void* )
{
   prc=sliderVal;
}


bool readCamera(string* str, vector<char*>* inputCmdStr){
    //string str[7]={"front", "right", "back", "left", "up", "down", ""};
    int s=0;

    while(true){
        cout<<"Do you want to configure colors first?\n[y/n]: ";
        char ch=getchar();
        if(ch=='y' || ch=='Y'){
            if(!configureColors()){
                return false;
            }
            cout<<"\nConfigured colors successfully!\n";
            break;
        }else if(ch=='n'||ch=='N'){
            break;
        }
    }
    FILE *cfgfp=fopen(configFileName, "r");
    if(cfgfp==NULL){
        cout<<"[ERROR] Color configuration file could not be opened!\n";
        return false;
    }
    //read color config
    cout<<"Reading color configuration..\n";
    for(int i=0;i<6;i++){
        fscanf(cfgfp, "%c ", &colors[i].clr);
        cout<<colors[i].clr<<" ";
        for(int j=0;j<3;j++){
            fscanf(cfgfp, "%lf %lf ", &colors[i].min[j], &colors[i].max[j]);
        }
    }
    cout<<"..done!\n";

    FILE *outfp=fopen(outFileName, "w");
    bool read=false;
    if(outfp==NULL){
        cout<<"[ERROR] Output file could not be created!\n";
        return false;
    }
    cout<<"Video device? \n:";
    int camid=0;
    cin>>camid;
    VideoCapture cap(camid);
    Mat frame, frame2;
    namedWindow(winName, WINDOW_NORMAL);
    resizeWindow(winName, WINDOWX, WINDOWY);
    createTrackbar("", winName, &sliderVal, sliderVal_max, on_trackbar );

    if(!cap.isOpened()){
        cout<<"[ERROR] Camera could not be opened!\n";
        return false;
    }
    cout<<"Camera opened.\n";
    cout<<"Bring "<<str[s++]<<" side for input..\n";
    for(;;){
        cap>>frame;
        frame.copyTo(frame2);
        //flip(frame, frame, 1);
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                //boundingRect
                rectangle(frame2, Rect(X+(i*GAP), Y+(j*GAP), SIZE, SIZE), m[i][j], 3);
            }
        }
        //adjustFrame(frame);
        imshow(winName, frame2);
        int key=waitKey(20);
        if(key==27){
            //ESC
            cout<<"Camera closed.\n";
            return false;
        }else if(key==32){
            //SPACE
            bool success=readColors(frame);
            for(int i=0;i<3;i++){
                for(int j=2; j>=0;j--){
                    printf("%c ", clrs[i][j]);
                    //putText(frame2, ""+clrs[i][j], Point(X+(i*GAP)+SIZE/2, Y+(j*GAP)+SIZE/2), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(50,50,50), 1, CV_AA);
                }
                printf("\n");
            }
            //imshow(winName, frame2);
            if(!success){
                cout<<"[ERROR] Invalid read\n";
                read=false;
            }else{
                cout<<"Read success..\n";
                read=true;
            }
            //sleep(1);
        }else if(key==10){
            //ENTER
            if(read){
                read=false;
                cout<<"Final input..\n";
                if(s-1==2 || s-1==5){
                    //cout<<"if...............s="<<s<<"\n";
                    for(int i=0;i<3;i++){
                        for(int j=2; j>=0;j--){
                            fprintf(outfp, "%c ", clrs[2-j][i]);
                            printf("%c ", clrs[2-j][i]);
                        }
                        fprintf(outfp, "\n");
                        printf("\n");
                    }
                }else{
                    //cout<<"else...............s="<<s<<"\n";
                    for(int i=0;i<3;i++){
                        for(int j=2; j>=0;j--){
                            fprintf(outfp, "%c ", clrs[j][2-i]);
                            printf("%c ", clrs[j][2-i]);
                        }
                        fprintf(outfp, "\n");
                        printf("\n");
                    }
                }
                
                fprintf(outfp, "\n");
                cout<<"Bringing "<<str[s++]<<" side for input..\n";
                for(int i=0;i<inputCmdStr[s-2].size();i++){
                    startSerialSendThread((char*)inputCmdStr[s-2][i]);
                    startSerialReceiveThread();
                    printf("Sent: %s ---> \nPerformed: ",inputCmdStr[s-2][i]);
                    sleep(1);
                }
                cout<<"..done!\n";
                if(s==7){
                    fclose(outfp);
                    cap.release();
                    destroyWindow(winName);
                    return true;
                }
            }else{
                cout<<"Read color first using SPACE\n";
            }

        }
        
    }
}

// int main(){
//     if(readCamera()){
//         cout<<"Successfully read cube from camera.\n";
//     }else{
//         cout<<"[ERROR] Camera input error\n";
//         exit(1);
//     }
// }