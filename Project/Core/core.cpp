#include "decision.h"
#include "sprinkler.h"
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
        int max_x;
        int max_y;
        float scalar;
        int qtd_agua;
        int vento, umidade, insolacao;
        ofstream output;
        ifstream input;

        Mat field, final_field;
        vector<map_block> grass_blocks;
        vector<map_block>::iterator it;
        vector<block_result> resultados;
        vector<jato> jatos;
        vector<sprinkler> sprinklers;


        /* --- Variaveis de decisao */
        coeficiente = parse_weather();
        vento = get_vento();
        umidade = get_umidade();
        insolacao = get_insolacao();

        /* Processamento de imagem */
        // field = stich(argc, argv);
        field = imread("field.jpg");
        grass_blocks = main_image(field, max_y, max_x);
        final_field = imread("final.jpg");

        sprinklers = read_sprinklers();
        // for (i=0; i<4; i++) {
        //         cout << sprinklers[i].x  << endl;
        //         cout << sprinklers[i].y << endl;
        // }

        /* Tomada de decisão */
        for(it = grass_blocks.begin(); it != grass_blocks.end(); it++) {
                parameters p;
                p.coeficienteChuva = coeficiente;
                p.umidade = umidade;
                p.vento = vento;
                p.insolacao = insolacao;
                p.resultadoAnterior = get_resAnterior((*it).block_numx, (*it).block_numy, input);
                p.corGrama = get_cor((*it).regado);
                qtd_agua = state_machine(p);

                scalar = 255*qtd_agua/100;
                circle(final_field, Point((*it).x, (*it).y), 10, Scalar(scalar, 0, 0), -1, 8);

                resultados.push_back(block_result());
                // resultados[i].x = (*it).block_numx;
                // resultados[i].y = (*it).block_numy;
                resultados[i].qtd_agua = qtd_agua;

                jatos.push_back(get_jato(sprinklers, (*it)));

                i++;
        }

        imwrite("x.jpg", final_field);
        remove("resAnterior");
        save_resAnterior(resultados, output);

        return 0;
}
