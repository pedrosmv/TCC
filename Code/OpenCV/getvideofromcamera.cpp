#include "opencv2/opencv.hpp"

using namespace cv;

int main(){

  VideoCapture cap(0); /*Open the camera */
  if (!cap.isOpened()){
    return -1;
  }

  Mat square;
  namedWindow("square", 1);

  while (true){
    Mat frame;
    cap >> frame;
    cvtColor(frame, square, COLOR_BGR2GRAY);
    imshow("square", square);
    if (waitKey(30) >= 0){
      break;
    }
  }

  return 0;
}
