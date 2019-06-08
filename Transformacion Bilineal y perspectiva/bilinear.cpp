#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

#include "gaus.cpp"
int  main()
{

	// Read source image.
	Mat ImagenEntrada = imread("Pika.png");

	int h = ImagenEntrada.rows;
	int w = ImagenEntrada.cols;

	Mat ImagenSalida(h,w, CV_8UC3, Scalar(0, 0, 0));

	// Create a vector of destination points.
	vector<Point2f> pts_origen;
	vector<Point2f> pts_destino;

	pts_origen.push_back(Point2f(0, 0));
	pts_origen.push_back(Point2f(w - 1, 0));
	pts_origen.push_back(Point2f(h - 1, h - 1));
	pts_origen.push_back(Point2f(0, h - 1));




	pts_destino.push_back(Point2f(52,0));
	pts_destino.push_back(Point2f(228,46));
	pts_destino.push_back(Point2f(255,229));
	pts_destino.push_back(Point2f(0,24));
	// Warp source image to destination
	//warpPerspective(im_src, im_dst, h, size);

	// Show image
	/*a[0][0]=1;     a[0][1]=1;     a[0][2]=1*1;         a[0][3]=1; a[0][4]=52;
	a[1][0]=(w-1); a[1][1]=1;     a[1][2]=(w-1)*1;     a[1][3]=1; a[1][4]=228;
	a[2][0]=(h-1); a[2][1]=(h-1); a[2][2]=(h-1)*(h-1); a[2][3]=1; a[2][4]=225;
	a[3][0]=1;     a[3][1]=(h-1); a[3][2]=(0)*(h-1);   a[3][3]=1; a[3][4]=0;*/

	a[0][0]=1;     a[0][1]=1;     a[0][2]=1*1;         a[0][3]=1; a[0][4]=0;
	a[1][0]=(w-1); a[1][1]=1;     a[1][2]=(w-1)*1;     a[1][3]=1; a[1][4]=46;
	a[2][0]=(h-1); a[2][1]=(h-1); a[2][2]=(h-1)*(h-1); a[2][3]=1; a[2][4]=229;
	a[3][0]=1;     a[3][1]=(h-1); a[3][2]=(0)*(h-1);   a[3][3]=1; a[3][4]=24;

	gaus();

	float C11=0.344909 ;
	float C12=-0.101961 ;
	float C13=0.00018802;
	float C14= 51.7569;

	float C21=0.089586 ;
	float C22=0.04706 ;
	float C23=0.000610105 ;
	float C24=-0.137256;



	for (int x = 0; x < h; x++)
	{
		for (int y = 0; y < w; y++)
		{

			int temp =(C11*x + C12*y + x*y*C13 + C14*1)*2;
			int temp2=(C21*x + C22*y + x*y*C23 + C24*1)*2;

			cout<<temp<<" "<<temp2<<endl;

			ImagenSalida.at<Vec3b>(x,y)[0]=ImagenEntrada.at<Vec3b>(temp,temp2)[0];
			ImagenSalida.at<Vec3b>(x,y)[1]=ImagenEntrada.at<Vec3b>(temp,temp2)[1];
			ImagenSalida.at<Vec3b>(x,y)[2]=ImagenEntrada.at<Vec3b>(temp,temp2)[2];
			
		}
	}
	imshow("Imagen Transformada", ImagenSalida);
	
	//imwrite("Image.jpg", im_dst);
	//CloseHandle(handle1);//Ïú»ÙÏß³Ì1  
	waitKey(0);


}
