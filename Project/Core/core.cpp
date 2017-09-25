#include "decision.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {
  int i= 0;
  float coeficiente;
  Mat field;
  vector<map_block> grass_blocks;
  vector<map_block>::iterator it;
  coeficiente = parse_weather();

  field = stich(argc, argv);
  grass_blocks = image_processing(field);

  for(it = grass_blocks.begin(); it != grass_blocks.end(); it++){

    grass_blocks[i].qtd_agua = state_machine((*it).regado, coeficiente);
    i++;
  }

  return 0;
}
