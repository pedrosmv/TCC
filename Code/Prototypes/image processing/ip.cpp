#include <iostream>
#include <fstream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct range{
  Scalar min;
  Scalar max;
};

int main(int argc, char** argv){

  Mat field = imread(argv[1], 1);
  Mat hsv_field; /* hsv_field vai receber a imagem passada por parametro após o seu color space ser convertido */
  Mat field_treshold; /* field_treshold vai receber um array com um treshold baseado nos ranges que foram passados */
  Mat element;
  Mat processed_field;
  Mat final_field;

  range field_range;
  field_range.min = Scalar(12, 50, 30);
  field_range.max = Scalar(80,255,200);

  int bound_min, bound_max;
  int i, j, aux;

  /* Pre-processing image */
  blur(field, field, Size(2,2)); /* Blurs an image using the normalized box filter. */
  cvtColor(field, hsv_field, CV_BGR2HSV); /* Converts an image from one color space to another. BGR -> HSV */
  inRange(hsv_field, field_range.min, field_range.max, field_treshold);
  element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(4,4));
  morphologyEx(field_treshold, processed_field, MORPH_CLOSE, element);

  bound_min = processed_field.rows;
  bound_max = 0;

  for(i = 0; i < processed_field.cols; i++){
    for(j = 0; j < processed_field.rows; j++){
      if(processed_field.at<uchar>(j,i) == 0){
        aux = j;
        break;
      }
    }
    if (aux < bound_min)
      bound_min = aux;
    if (aux > bound_max)
      bound_max = aux;
  }

  field.copyTo(final_field, field_treshold);
  imwrite("imagem pre processada.jpg", field);
  field.release();
}
