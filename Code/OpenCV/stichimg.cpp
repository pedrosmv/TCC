#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;
/* As imagens precisam ter overlap para o stitch funcionar */
int main(int argc, char** argv){
  int i;
  vector<Mat> imgs;
  string result_name = "result.jpg";

  for(i = 1; i < argc; ++i){
    Mat img = imread(argv[i]);
    if (img.empty())
     {
         // Exit if image is not present
         cout << "Can't read image '" << argv[i] << "'\n";
         return -1;
     }
    imgs.push_back(img);
  }

  Mat pano;
  Stitcher stitcher = Stitcher::createDefault(false);
  stitcher.setPanoConfidenceThresh(0.6);
  Stitcher::Status status = stitcher.stitch(imgs, pano);
  if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return -1;
    }
  imwrite(result_name, pano);
  return 0;
}
