#include <iostream>
#include <fstream>
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include "imageprocessing.h"

using namespace std;
using namespace cv;

/* Recebe duas imagens por argumento e junta as duas numa panoramica que representa
   a area a ser regada */
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

/* Calcula a media da cor dos pixels numa area quadrada e pinta todos os pixels
   dentro dessa mesma area com a mesma cor */
Mat calculateAvgPxlColor(Mat final_field, int quad_dim, int quad_linha, int quad_col, int black_pixel_maximum){
        Mat squared_field;

        int n_linhas, n_cols;
        int bgr_array_linha, bgr_array_col;
        int num_quad, num_quad_col, num_quad_linha;
        int avg_color, blue_color, green_color, red_color, not_black;

        num_quad_col = 0;
        num_quad_linha = 0;
        num_quad = 0;
        n_linhas = quad_linha*quad_dim;
        n_cols = quad_col*quad_dim;

        /* Returns a zero array of the specified size and type. */
        squared_field = Mat::zeros(n_linhas, n_cols, CV_8UC3);

        /* Esse loop vai iterar sobre o vetor dos canais de cores da imagem e pegar a media de imagem duma area retangular e transformar
            todos os pixels dentro daquela area na cor media */
        while(num_quad < quad_linha*quad_col) {
                avg_color = 0;
                blue_color = 0;
                green_color = 0;
                red_color = 0;
                not_black = 0;

                for(bgr_array_linha = (quad_dim*num_quad_linha); bgr_array_linha < (quad_dim*(num_quad_linha+1)); bgr_array_linha++) {
                        for(bgr_array_col = (quad_dim*num_quad_col); bgr_array_col < (quad_dim*(num_quad_col+1)); bgr_array_col++) {
                                if( final_field.at<Vec3b>(bgr_array_linha, bgr_array_col) != Vec3b(0,0,0) && final_field.at<Vec3b>(bgr_array_linha, bgr_array_col) != Vec3b(255,255,255)) {
                                        blue_color += final_field.at<Vec3b>(bgr_array_linha, bgr_array_col)[0];
                                        green_color += final_field.at<Vec3b>(bgr_array_linha, bgr_array_col)[1];
                                        red_color += final_field.at<Vec3b>(bgr_array_linha, bgr_array_col)[2];
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


                rectangle(squared_field, Point(0+(quad_dim*num_quad_col),(0+(quad_dim*num_quad_linha))),Point((quad_dim*(num_quad_col+1))-1,(quad_dim*(num_quad_linha+1))-1),
                          Scalar(blue_color/avg_color,green_color/avg_color,red_color/avg_color),-1,8);


                num_quad++;
                num_quad_col = num_quad%quad_col;
                if(num_quad_col == 0) {
                        num_quad_linha++;
                }
        }
        return squared_field;
}

/* Recebe a imagem pre processada e vai separar os quadrados que precisam ser
   regados ou nao */
Mat apply_mask(Mat squared_field, range limites_rgb){
        Mat int_mask, final_mask;


        inRange(squared_field, limites_rgb.min, limites_rgb.max, int_mask); /* A mascara intermediaria separa os quadrados com grama que precisam ser regadas */
        squared_field.copyTo(final_mask, int_mask); /* A mascara final recebe os pixels que não são preto na imagem da primeira mascara */

        return final_mask;
}

/* Para cada bloco, e calculada a diferenca entre sua cor e o limite para a
   area ser considerada saudavel, essa diferenca infere na quantidade de agua que
   vai ser gasta para regar a area */
float calc_dif_cor(int blue, int green, int red, range limites_rgb){
        float vetor_dif[3];
        float dif;
        vetor_dif[0] = limites_rgb.min[0] - blue;
        vetor_dif[1] = limites_rgb.min[1] - green;
        vetor_dif[2] = limites_rgb.min[2] - red;

        return dif = sqrt(pow(vetor_dif[0], 2) + pow(vetor_dif[1], 2) + pow(vetor_dif[2], 2));
}

/* mapUnhealthyGrass vai mapear os quadrados que precisam ser regados e colocar a posição dos quadrados num vetor de structs */
void mapUnhealthyGrass(Mat field, Mat field_mask, int quad_dim, int quad_linha, int quad_col, vector<map_block> &mapBlock, range limites_rgb){
        int x, y, row, col, i,num_quad;
        int r, g, b;
        x = quad_dim/2;
        y = quad_dim/2;
        i = 0;
        num_quad = 0;

        /* Esse loop vai procurar pelos quadrados pretos na mascara que representam as areas que precisam ser regadas */
        while(num_quad < quad_linha*quad_col) {
                for(row = 0; row < quad_linha; row++) {
                        for(col =0; col < quad_col; col++) {
                                if(field.at<Vec3b>(y, x) != Vec3b(0,0,0)) {
                                        if(field_mask.at<Vec3b>(y, x) == Vec3b(0,0,0)) {
                                                mapBlock.push_back(map_block());
                                                mapBlock[i].x = x;
                                                mapBlock[i].y = y;
                                                mapBlock[i].regado = true;
                                                b = field.at<Vec3b>(y, x)[0];
                                                g = field.at<Vec3b>(y, x)[1];
                                                r = field.at<Vec3b>(y, x)[2];
                                                mapBlock[i].dif_cor = calc_dif_cor(b, g, r, limites_rgb);
                                                i++;
                                        }
                                }
                                num_quad++;
                                x += quad_dim;
                        }
                        x = (quad_dim/2);
                        y += quad_dim;
                }
        }
}

/* Nucleo principal que comanda o processamento de imagens */
vector<map_block> image_processing(Mat field, int &max_col, int &max_linha){
        /* squarees that are going to be used */
        Mat hsv_field; /* hsv_field vai receber a imagem passada por parametro após o seu color space ser convertido */
        Mat field_treshold; /* field_treshold vai receber um array com um treshold baseado nos ranges que foram passados */
        Mat element;
        Mat processed_field;
        Mat final_field;
        Mat squared_field;
        Mat mask_field;

        vector<map_block> mapBlock;

        range field_range;
        range limites_rgb;
        /* Valores retirados da monografia do Tiago -############## CHECAR DEPOIS ##########*/
        field_range.min = Scalar(12, 50, 30);
        field_range.max = Scalar(80,255,200);

        limites_rgb.min = Scalar(13, 80, 60);
        limites_rgb.max = Scalar(64, 134, 105);

        /* ########################### */

        /* Numerical values */
        int quad_dim, quad_linha, quad_col;
        int black_pixel_maximum;

        quad_dim = 100;
        black_pixel_maximum = quad_dim*quad_dim*0.6;
        quad_linha = field.rows/quad_dim;
        quad_col = field.cols/quad_dim;

        /* Pre-processamento da imagem */
        blur(field, field, Size(2,2)); /* Blurs an image using the normalized box filter. */
        imwrite("blur.jpg", field);
        cvtColor(field, hsv_field, CV_BGR2HSV); /* Converts an image from one color space to another. BGR -> HSV */
        imwrite("hsv_field.jpg", hsv_field);
        inRange(hsv_field, field_range.min, field_range.max, field_treshold);
        imwrite("imagem segmentada.jpg", field_treshold);
        element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(4,4));
        morphologyEx(field_treshold, processed_field, MORPH_CLOSE, element);
        imwrite("process morfologico.jpg", processed_field);
        field.copyTo(final_field, processed_field);

        /* Processamento da imagem para encontrar as areas que precisam ser regadas */
        squared_field = calculateAvgPxlColor(final_field, quad_dim, quad_linha, quad_col, black_pixel_maximum);
        mask_field = apply_mask(squared_field,limites_rgb);
        mapUnhealthyGrass(squared_field, mask_field, quad_dim, quad_linha, quad_col, mapBlock, limites_rgb);

        imwrite("final.jpg", squared_field);
        imwrite("mask.jpg", mask_field);
        field.release();

        return mapBlock;
}
