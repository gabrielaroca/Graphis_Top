#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

#define pi 3.1416

float Zoom=10.0;
Mat ImagenEntrada = imread("Pika.png");
int h = ImagenEntrada.rows;
int w = ImagenEntrada.cols;

Mat ImagenSalida(h,w, CV_8UC3, Scalar(0, 0, 0));


void InterpolacionVecinosCercanos(){
	Mat ImagenEntrada = imread("pikachu.png");
	int h = ImagenEntrada.rows*Zoom;
	int w = ImagenEntrada.cols*Zoom;

	Mat ImagenSalida(h,w, CV_8UC3, Scalar(0, 0, 0));

	for(int x = 0; x < h ; x++){
		for (int y = 0; y < w; y++)
		{
			float Rx=x/Zoom;
			float Ry=y/Zoom;

			int F1=int ((x/Zoom)+0.5);
			int F2=int ((y/Zoom)+0.5);

			ImagenSalida.at<Vec3b>(x,y)[0] = ImagenEntrada.at<Vec3b>(F1,F2)[0];
			ImagenSalida.at<Vec3b>(x,y)[1] = ImagenEntrada.at<Vec3b>(F1,F2)[1];
			ImagenSalida.at<Vec3b>(x,y)[2] = ImagenEntrada.at<Vec3b>(F1,F2)[2];
		}
	}
	imshow("Imagen salida",ImagenSalida);
}



void InterpolacionBilineal(){
	Mat ImagenEntrada = imread("pikachu.png");
	
	int h = ImagenEntrada.rows*Zoom;
	int w = ImagenEntrada.cols*Zoom;
	Mat ImagenSalida2(h,w, CV_8UC3, Scalar(0, 0, 0));


	for(int x = 0; x < h ; x++){
		for (int y = 0; y < w; y++)
		{

			float px=x/Zoom;
			float py=y/Zoom;

			//if (px >= ImagenEntrada.cols - 1 || py >= ImagenEntrada.rows - 1) break;
			
			int i=(int)px;
			int s=(int)py;

			int d=i+1;
			int r=s+1;

			float a=px-i;
			float b=py-s;

			float R1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[0];
			float G1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[1];
			float B1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[2];

			float R2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[0];
			float G2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[1];
			float B2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[2];
			
			float R3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[0];
			float G3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[1];
			float B3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[2];

			float R4=a*b*ImagenEntrada.at<Vec3b>(d,r)[0];
			float G4=a*b*ImagenEntrada.at<Vec3b>(d,r)[1];
			float B4=a*b*ImagenEntrada.at<Vec3b>(d,r)[2];

			int aa=int(R1+R2+R3+R4);
			int bb=int(G1+G2+G3+G4);
			int cc=int(B1+B2+B3+B4);

			//cout<<(aa+bb+cc)/Zoom<<" | ";

			ImagenSalida2.at<Vec3b>(x,y)[0] = aa;
			ImagenSalida2.at<Vec3b>(x,y)[1] = bb;
			ImagenSalida2.at<Vec3b>(x,y)[2] = cc;

			//cout<<ImagenSalida.at<int>(x,y)<<" ";
			//cout<<" | "<<F1<<" "<<F2<<" ";
		}//cout<<endl;
	}
	imshow("Imagen salida",ImagenSalida2);

}


void Tranlacion(int x,int y){

	for (int i = 0; i < h-x; i++)
	{
		for (int j = 0; j < w-y; j++)
		{

			ImagenSalida.at<Vec3b>(i,j)[0]=ImagenEntrada.at<Vec3b>(i+x,j+y)[0];
			ImagenSalida.at<Vec3b>(i,j)[1]=ImagenEntrada.at<Vec3b>(i+x,j+y)[1];
			ImagenSalida.at<Vec3b>(i,j)[2]=ImagenEntrada.at<Vec3b>(i+x,j+y)[2];
			
		}
	}
}

void ScalarImage(float Scale_x,float Scale_y){

	Mat ImagenSalida2(h*Scale_x,w*Scale_y, CV_8UC3, Scalar(0, 0, 0));

	int end_x=h*Scale_x;
	int end_y=w*Scale_y;
	for(int x = 0; x < end_x; x++){
		for (int y = 0;y < end_y; y++)
		{

			float px=x/Scale_x;
			float py=y/Scale_y;

			if (px >= ImagenEntrada.cols - 1 || py >= ImagenEntrada.rows - 1) break;
			
			int i=(int)px;
			int s=(int)py;

			int d=i+1;
			int r=s+1;

			float a=px-i;
			float b=py-s;

			float R1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[0];
			float G1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[1];
			float B1=(1.0-a)*(1.0-b)*ImagenEntrada.at<Vec3b>(i,s)[2];

			float R2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[0];
			float G2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[1];
			float B2=a*(1.0-b)*ImagenEntrada.at<Vec3b>(d,s)[2];
			
			float R3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[0];
			float G3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[1];
			float B3=(1.0-a)*b*ImagenEntrada.at<Vec3b>(i,r)[2];

			float R4=a*b*ImagenEntrada.at<Vec3b>(d,r)[0];
			float G4=a*b*ImagenEntrada.at<Vec3b>(d,r)[1];
			float B4=a*b*ImagenEntrada.at<Vec3b>(d,r)[2];

			int aa=int(R1+R2+R3+R4);
			int bb=int(G1+G2+G3+G4);
			int cc=int(B1+B2+B3+B4);

			//cout<<(aa+bb+cc)/Zoom<<" | ";

			ImagenSalida2.at<Vec3b>(x,y)[0] = aa;
			ImagenSalida2.at<Vec3b>(x,y)[1] = bb;
			ImagenSalida2.at<Vec3b>(x,y)[2] = cc;

			//cout<<ImagenSalida.at<int>(x,y)<<" ";
			//cout<<" | "<<F1<<" "<<F2<<" ";
		}//cout<<endl;
	}
	imshow("Imagen salida2",ImagenSalida2);
	
}

void rotarImage(double angle,int CX,int CY){
	cout<<angle*pi/180<<" "<<CX<<" "<<CY<<endl;
	double matriz[2][3]={
		{cos(angle*pi/180)  ,sin(angle*pi/180) ,CX - CX*cos(angle*pi/180) - CY*sin(angle*pi/180)},
		{-sin(angle*pi/180) ,cos(angle*pi/180) ,CY + CX*sin(angle*pi/180) - CY*cos(angle*pi/180)}
	};

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{


			int Xp=int(matriz[0][0]*i+matriz[0][1]*j+matriz[0][2]*1);
			int Yp=int(matriz[1][0]*i+matriz[1][1]*j+matriz[1][2]*1);
			
			if(Yp>=0 && Yp<=w && Xp>=0&&Xp<=h){
				ImagenSalida.at<Vec3b>(i,j)[0]=ImagenEntrada.at<Vec3b>(Xp,Yp)[0];
				ImagenSalida.at<Vec3b>(i,j)[1]=ImagenEntrada.at<Vec3b>(Xp,Yp)[1];
				ImagenSalida.at<Vec3b>(i,j)[2]=ImagenEntrada.at<Vec3b>(Xp,Yp)[2];
			}
			

			
		}//cout<endl;
	}
}

int main(){
	//InterpolacionVecinosCercanos();
	InterpolacionBilineal();


	//namedWindow( "ImagenEntrada", WINDOW_AUTOSIZE );// Create a window for display.
	
	waitKey(0);
	return 0;
}
