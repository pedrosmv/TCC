#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

/* Representa os intervalos usados para processamento de imagem */
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

vector<map_block> image_processing(Mat field, int &max_col, int &max_linha);
Mat stich(int argc, char** argv);
Mat calculateAvgPxlColor(Mat final_field, int quad_dim, int quad_linha, int quad_col, int black_pixel_maximum);
Mat apply_mask(Mat squared_field, range limites_rgb);
void mapUnhelthyGrass(Mat field, Mat field_mask, int quad_dim, int quad_linha, int quad_col, int block_size, vector<map_block> &mapBlock, range limites_rgb);
float calc_dif_cor(int blue, int green, int red, range limites_rgb);

#endif
