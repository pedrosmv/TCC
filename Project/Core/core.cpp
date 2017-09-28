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
        int max_row, max_col;
        int raio;
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

        cout << "Entre a distancia max do sprinkler: " << endl;
        cin >> raio;


        /* --- Variaveis de decisao */
        coeficiente = parse_weather();
        vento = get_vento();
        umidade = get_umidade();
        insolacao = get_insolacao();

        /* Processamento de imagem */
        // field = stich(argc, argv);
        field = imread("field.jpg");
        grass_blocks = image_processing(field, max_col, max_row);
        final_field = imread("final.jpg");

        sprinklers = set_sprinklers(max_col, max_row, raio);
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
                p.corGrama = get_cor((*it).regado, (*it).dif_cor);
                qtd_agua = state_machine(p);

                scalar = 255*qtd_agua/100;
                circle(final_field, Point((*it).x, (*it).y), 10, Scalar(scalar, 0, 0), -1, 8);

                resultados.push_back(block_result());
                resultados[i].x = (*it).block_numx;
                resultados[i].y = (*it).block_numy;
                resultados[i].qtd_agua = qtd_agua;
                cout << (*it).dif_cor << endl;
                jatos.push_back(get_jato(sprinklers, (*it), max_row, max_col));
                // cout << endl;
                // cout << "Sprinkler: "  << endl;
                // if(jatos[i].orientacao == ESQUERDA)
                //         cout << "Esquerda " << endl;
                // if(jatos[i].orientacao == DIREITA)
                //         cout << "Direita " << endl;
                // if(jatos[i].orientacao == CIMA)
                //         cout << "Cima " << endl;
                // if(jatos[i].orientacao == BAIXO)
                //         cout << "Baixo " << endl;
                //
                // cout << "Distancia: " << jatos[i].distancia << endl;
                // cout << "Angulo: " << jatos[i].angle << endl;
                i++;
        }

        imwrite("x.jpg", final_field);
        remove("resAnterior");
        save_resAnterior(resultados, output);

        return 0;
}
