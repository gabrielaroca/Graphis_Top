#include "cuda_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "convolucion.h"
#include <cuda.h>
#include <string>
#include <math.h>   
#define TxB 512

using namespace std;
using namespace cv;


Mat imageInput;
Mat imageOutput;

uchar4 *d_inputImg__;
unsigned char *d_outputImg__;

float *h_filter;

size_t numRows() { return imageInput.rows; }
size_t numCols() { return imageInput.cols; }


void preProcess(uchar4 **imgInput, unsigned char **imgOutput, uchar4 **d_inputImg, unsigned char **d_outputImg, string& nameImg){
      checkCudaErrors(cudaFree(0));

      Mat image;
      image = imread(nameImg.c_str(),CV_LOAD_IMAGE_COLOR);
      if (image.empty()) {
            cout << "No se puede abrir el archivo: " <<nameImg <<endl;
            exit(1);
      }
      cvtColor(image, imageInput, CV_BGR2RGBA);

      imageOutput.create(image.rows, image.cols, CV_8UC1);

      if (!imageInput.isContinuous() || !imageOutput.isContinuous()) {
            std::cerr << "No son imagenes continuas" << std::endl;
            exit(1);
          }

      *imgInput = (uchar4*)imageInput.ptr<unsigned char>(0);
      *imgOutput = imageOutput.ptr<unsigned char>(0);

      const size_t numPixels = numCols()* numRows();

      checkCudaErrors(cudaMalloc(d_inputImg, sizeof(uchar4) * numPixels));
      checkCudaErrors(cudaMalloc(d_outputImg, sizeof(unsigned char) * numPixels));
      checkCudaErrors(cudaMemset(*d_outputImg, 0, numPixels * sizeof(unsigned char)));

      checkCudaErrors(cudaMemcpy(*d_inputImg, *imgInput, sizeof(uchar4) * numPixels, cudaMemcpyHostToDevice));

      d_inputImg__ = *d_inputImg;
      d_outputImg__ = *d_outputImg;
}

// __global__ blurKernel()


__global__
void rgba_to_grey_kernel(const uchar4* const rgbaImage,
                       unsigned char* const greyImage,
                       int numRows, int numCols)
{
  // The mapping from components of a uchar4 to RGBA is:
  // .x -> R ; .y -> G ; .z -> B ; .w -> A
  //
  //The output (greyImage) at each pixel should be the result of
  //applying the formula: output = .299f * R + .587f * G + .114f * B;
  //Note: We will be ignoring the alpha channel for this conversion
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  uchar4 px = rgbaImage[i]; // thread pixel to process
  greyImage[i] = .299f * px.x +
                 .587f * px.y +
                 .114f * px.z;
}

void rgba_to_grey(uchar4 * const d_rgbaImage,
                  unsigned char* const d_greyImage, size_t numRows, size_t numCols)
{

  // Since it does not matter the relative position of a pixel
  // the block - grid assign strategy will simply be to cover
  // all pixels secuencially in 'x' axis
  long long int total_px = numRows * numCols;  // total pixels
  long int grids_n = ceil(total_px / TxB); // grids numer
  const dim3 blockSize(TxB, 1, 1);
  const dim3 gridSize(grids_n, 1, 1);
  rgba_to_grey_kernel<<<gridSize, blockSize>>>(d_rgbaImage, d_greyImage, numRows, numCols);
  
  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
}

__global__ void convolutionGPU(float *d_Result, float *d_Data, int dataW, int dataH )
{
      
      // global mem address for this thread
      const int gLoc = threadIdx.x +  blockIdx.x * blockDim.x + threadIdx.y * dataW + blockIdx.y * blockDim.y * dataW; 

      float sum = 0;
      float value = 0;
      
      for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++)	// row wise
      for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++)	// col wise
      {
            // check row first
            if (blockIdx.x == 0 && (threadIdx.x + i) < 0)	// left apron
                  value = 0;
            else if ( blockIdx.x == (gridDim.x - 1) && (threadIdx.x + i) > blockDim.x-1 )	// right apron
            value = 0;
      else { 
            // check col next
            if (blockIdx.y == 0 && (threadIdx.y + j) < 0)	// top apron
            value = 0;
            else if ( blockIdx.y == (gridDim.y - 1) && 
            (threadIdx.y + j) > blockDim.y-1 )	// bottom apron
            value = 0;
            else	// safe case
            value = d_Data[gLoc + i + j * dataW];
      } 
      sum += value * d_Kernel[KERNEL_RADIUS + i] * d_Kernel[KERNEL_RADIUS + j];
      }
      d_Result[gLoc] = sum; 
}


void convolutionGPU(float *d_Result, float *d_Data, int dataW, int dataH )
{
      
      long long int total_px = dataH * dataW;  // total pixels
      long int grids_n = ceil(total_px / TxB); // grids numer
      const dim3 blockSize(TxB, 1, 1);
      const dim3 gridSize(grids_n, 1, 1);
      rgba_to_grey_kernel<<<gridSize, blockSize>>>(d_Data, d_Result, dataW, dataH);
      
      cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError()); 
}


int main(int argc, char **argv) {
      uchar4        *h_inputImage, *d_inputImage;
      unsigned char *h_outputImage, *d_outputImage;
    
      string nameImage;
      string outNameImage;

      float *h_filter;
      float *d_filter;
      unsigned int h_ tam_filter = 3;
      unsigned int d_tam_filter = h_tam_filter*h_tam_filter*sizeof(float);

      h_filter = new float[h_tam_filter*h_tam_filter];
    
      checkCudaErrors(cudaFree(0));
    
      switch (argc)
      {
          case 2:
            nameImage = std::string(argv[1]);
            outNameImage= "output.png";
            break;
          default:
          std::cerr << "Use: ./cuda [nombre_imagen]" << std::endl;
          exit(1);
      }
     
      preProcess(&h_inputImage, &h_outputImage, &d_inputImage, &d_outputImage, nameImage);
      cudaMalloc(d_filter,d_tam_filter);
      // rgba_to_grey(d_inputImage, d_outputImage, numRows(), numCols());
      convolutionGPU(d_outputImage, d_inputImage, numRows(), numCols())
    
      size_t numPixels = numRows()*numCols();
      checkCudaErrors(cudaMemcpy(h_outputImage, d_outputImage, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));
 
      cv::Mat outputImg(numRows(), numCols(), CV_8UC1, (void*)h_outputImage);
      // Open the window
      cv::namedWindow("Nueva Imagen");
      // Display the image m in this window
      cv::imshow("Nueva Imagen", outputImg);
      cvWaitKey (0);
      cvDestroyWindow ("Nueva Imagen");
      //output the image
      cv::imwrite(outNameImage.c_str(), outputImg);
    
      /* Cleanup */
      cudaFree(d_inputImg__);
      cudaFree(d_outputImg__);
    
      return 0;
    }
