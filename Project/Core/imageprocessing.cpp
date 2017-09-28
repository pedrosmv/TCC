#include <iostream>
#include <fstream>
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include "imageprocessing.h"

using namespace std;
using namespace cv;

Mat stich(int argc, char** argv){
        int i;
        vector<Mat> imgs;
        string result_name = "field.jpg";

        for(i = 1; i < argc; ++i) {
                Mat img = imread(argv[i]);
                if (img.empty())
                {
                        // Exit if image is not present
                        cout << "Can't read image '" << argv[i] << "'\n";
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
        }
        imwrite(result_name, pano);

        return pano;
}

Mat calculateAvgPxlColor(Mat final_field, int square_dimensions, int square_row, int square_col, int black_pixel_maximum){
        Mat squared_field;

        int n_rows, n_cols;
        int bgr_array_row, bgr_array_col;
        int square_numbers, square_numbers_col, square_numbers_row;
        int avg_color, blue_color, green_color, red_color, not_black;

        square_numbers_col = 0;
        square_numbers_row = 0;
        square_numbers = 0;
        n_rows = square_row*square_dimensions;
        n_cols = square_col*square_dimensions;

        /* Returns a zero array of the specified size and type. */
        squared_field = Mat::zeros(n_rows, n_cols, CV_8UC3);

        /* Esse loop vai iterar sobre o vetor dos canais de cores da imagem e pegar a media de imagem duma area retangular e transformar
            todos os pixels dentro daquela area na cor media */
        while(square_numbers < square_row*square_col) {
                avg_color = 0;
                blue_color = 0;
                green_color = 0;
                red_color = 0;
                not_black = 0;

                for(bgr_array_row = (square_dimensions*square_numbers_row); bgr_array_row < (square_dimensions*(square_numbers_row+1)); bgr_array_row++) {
                        for(bgr_array_col = (square_dimensions*square_numbers_col); bgr_array_col < (square_dimensions*(square_numbers_col+1)); bgr_array_col++) {
                                if( final_field.at<Vec3b>(bgr_array_row, bgr_array_col) != Vec3b(0,0,0) && final_field.at<Vec3b>(bgr_array_row, bgr_array_col) != Vec3b(255,255,255)) {
                                        blue_color += final_field.at<Vec3b>(bgr_array_row, bgr_array_col)[0];
                                        green_color += final_field.at<Vec3b>(bgr_array_row, bgr_array_col)[1];
                                        red_color += final_field.at<Vec3b>(bgr_array_row, bgr_array_col)[2];
                                        avg_color++;
                                        not_black++;
                                }
                        }
                }
                if(not_black <= black_pixel_maximum || not_black == 0) {
                        blue_color = 0;
                        green_color = 0;
                        red_color = 0;
                }
                if(avg_color == 0) {
                        avg_color = 1;
                }


                rectangle(squared_field, Point(0+(square_dimensions*square_numbers_col),(0+(square_dimensions*square_numbers_row))),Point((square_dimensions*(square_numbers_col+1))-1,(square_dimensions*(square_numbers_row+1))-1),
                          Scalar(blue_color/avg_color,green_color/avg_color,red_color/avg_color),-1,8);


                square_numbers++;
                square_numbers_col = square_numbers%square_col;
                if(square_numbers_col == 0) {
                        square_numbers_row++;
                }
        }
        return squared_field;
}

Mat apply_mask(Mat squared_field, range rgb_limits){
        Mat int_mask, final_mask;


        inRange(squared_field, rgb_limits.min, rgb_limits.max, int_mask); /* A mascara intermediaria separa os quadrados com grama que precisam ser regadas */
        squared_field.copyTo(final_mask, int_mask); /* A mascara final recebe os pixels que não são preto na imagem da primeira mascara */

        return final_mask;
}

float calc_dif_cor(int blue, int green, int red, range rgb_limits){
        float vetor_dif[3];
        float dif;
        vetor_dif[0] = rgb_limits.min[0] - blue;
        vetor_dif[1] = rgb_limits.min[1] - green;
        vetor_dif[2] = rgb_limits.min[2] - red;

        return dif = sqrt(pow(vetor_dif[0], 2) + pow(vetor_dif[1], 2) + pow(vetor_dif[2], 2));
}

/* mapUnhelthyGrass vai mapear os quadrados que precisam ser regados e colocar a posição dos quadrados num vetor de structs */
void mapUnhelthyGrass(Mat field, Mat field_mask, int square_dimensions, int square_row, int square_col, int block_size, vector<map_block> &mapBlock, range rgb_limits){
        int x, y, row, col, i, number_square;
        int r, g, b;
        x = square_dimensions/2;
        y = square_dimensions/2;
        i = 0;
        number_square = 0;

        /* Esse loop vai procurar pelos quadrados pretos na mascara que representam as areas que precisam ser regadas */
        while(number_square < square_row*square_col) {
                for(row = 0; row < square_row; row++) {
                        for(col =0; col < square_col; col++) {
                                if(field.at<Vec3b>(y, x) != Vec3b(0,0,0)) {
                                        if(field_mask.at<Vec3b>(y, x) == Vec3b(0,0,0)) {
                                                mapBlock.push_back(map_block());
                                                mapBlock[i].x = x;
                                                mapBlock[i].y = y;
                                                mapBlock[i].regado = true;
                                                b = field.at<Vec3b>(y, x)[0];
                                                g = field.at<Vec3b>(y, x)[1];
                                                r = field.at<Vec3b>(y, x)[2];
                                                mapBlock[i].block_numx = (x/(block_size));
                                                mapBlock[i].block_numy = (y/(block_size));
                                                // circle(field, Point(x, y), 10, Scalar(255, 0, 0), -1, 8);
                                                mapBlock[i].dif_cor = calc_dif_cor(b, g, r, rgb_limits);
                                                i++;
                                        }
                                }
                                number_square++;
                                x += square_dimensions;
                        }
                        x = (square_dimensions/2);
                        y += square_dimensions;
                }
        }
}

vector<map_block> image_processing(Mat field, int &max_col, int &max_row){
        /* squarees that are going to be used */
        Mat hsv_field; /* hsv_field vai receber a imagem passada por parametro após o seu color space ser convertido */
        Mat field_treshold; /* field_treshold vai receber um array com um treshold baseado nos ranges que foram passados */
        Mat element;
        Mat processed_field;
        Mat final_field;
        Mat squared_field;
        Mat mask_field;

        vector<map_block> mapBlock;
        vector<block> error;

        range field_range;
        range rgb_limits;
        /* Valores retirados da monografia do Tiago -############## CHECAR DEPOIS ##########*/
        field_range.min = Scalar(12, 50, 30);
        field_range.max = Scalar(80,255,200);

        rgb_limits.min = Scalar(13, 80, 60);
        rgb_limits.max = Scalar(64, 134, 105);

        /* ########################### */

        /* Numerical values */
        int bound_min, bound_max;
        int i, j, aux;
        int square_dimensions, square_row, square_col;
        int black_pixel_maximum;
        int block_size;

        square_dimensions = 100;
        block_size = square_dimensions*5;
        black_pixel_maximum = square_dimensions*square_dimensions*0.6;
        square_row = field.rows/square_dimensions;
        square_col = field.cols/square_dimensions;

        if(field.rows % square_dimensions) {
                square_row = (field.rows/square_dimensions) + 1;
        }

        if(field.cols % square_dimensions) {
                square_col = (field.cols/square_dimensions) + 1;
        }

        /* Pre-processamento da imagem */
        blur(field, field, Size(2,2)); /* Blurs an image using the normalized box filter. */
        cvtColor(field, hsv_field, CV_BGR2HSV); /* Converts an image from one color space to another. BGR -> HSV */
        inRange(hsv_field, field_range.min, field_range.max, field_treshold);
        element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(4,4));
        morphologyEx(field_treshold, processed_field, MORPH_CLOSE, element);

        bound_min = processed_field.rows;
        bound_max = 0;

        for(i = 0; i < processed_field.cols; i++) {
                for(j = 0; j < processed_field.rows; j++) {
                        if(processed_field.at<uchar>(j,i) == 0) {
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

        /* Processamento da imagem para encontrar as areas que precisam ser regadas */
        squared_field = calculateAvgPxlColor(final_field, square_dimensions, square_row, square_col, black_pixel_maximum);
        mask_field = apply_mask(squared_field,rgb_limits);
        mapUnhelthyGrass(squared_field, mask_field, square_dimensions, square_row, square_col, block_size, mapBlock, rgb_limits);

        imwrite("final.jpg", squared_field);
        imwrite("mask.jpg", mask_field);
        field.release();

        return mapBlock;
}
