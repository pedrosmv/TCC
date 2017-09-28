#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

struct range {
        Scalar min;
        Scalar max;
};

/* Essa struct representa um bloco da imagem, sendo que a variavel regado diz se ele precisa ser regado ou nao */
struct map_block {
        int x;
        int y;
        bool regado = false;
        float dif_cor;
        int block_numx;
        int block_numy;
        float res_anterior;
};

struct block {
        int x;
        int y;
};

Mat stich(int argc, char** argv);
Mat calculateAvgPxlColor(Mat final_field, int square_dimensions, int square_row, int square_col, int black_pixel_maximum);
Mat apply_mask(Mat squared_field, range rgb_limits);
void mapUnhelthyGrass(Mat field, Mat field_mask, int square_dimensions, int square_row, int square_col, int block_size, vector<map_block> &mapBlock, range rgb_limits);
vector<map_block> image_processing(Mat field, int &max_col, int &max_row);
float calc_dif_cor(int blue, int green, int red, range rgb_limits);

#endif
