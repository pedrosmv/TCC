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
        int max_linha, max_col;
        float scalar;
        int qtd_agua;
        int vento, umidade;
        float insolacao;
        ofstream output;
        ifstream input;

        Mat field, final_field;
        vector<map_block> grass_blocks;
        vector<map_block>::iterator it;
        vector<block_result> resultados;
        vector<jato> jatos;
        vector<sprinkler> sprinklers;

        // system("python read.py");

        /* --- Variaveis de decisao */
        coeficiente = parse_tempo();
        cout << "Coeficiente: " << coeficiente << endl;
        vento = get_vento();
        umidade = get_umidade();
        cout << "Umidade: " << umidade << endl;
        insolacao = get_insolacao();
        cout << "Insolacao: " << insolacao << endl;

        /* Processamento de imagem */
        field = imread("field.jpg");
        grass_blocks = image_processing(field, max_col, max_linha);
        final_field = imread("final.jpg");

        sprinklers = read_sprinklers();

        /* Tomada de decisão */
        if (coeficiente*100 > 10) {
                for(it = grass_blocks.begin(); it != grass_blocks.end(); it++) {
                        parameters p;
                        p.coeficienteChuva = coeficiente;
                        p.umidade = umidade;
                        p.vento = vento;
                        p.insolacao = insolacao;
                        p.resultadoAnterior = get_resAnterior((*it).x/100, (*it).y/100, input);
                        p.corGrama = get_cor((*it).regado, (*it).dif_cor);
                        qtd_agua = state_machine(p);
                        scalar = 255*qtd_agua/100;
                        circle(final_field, Point((*it).x, (*it).y), 10, Scalar(scalar, 0, 0), -1, 8);

                        resultados.push_back(block_result());
                        resultados[i].x = (*it).x/100;
                        cout << "X: " << resultados[i].x << endl;
                        resultados[i].y = (*it).y/100;
                        cout << "Y: " << resultados[i].y << endl;
                        resultados[i].qtd_agua = qtd_agua;
                        jatos.push_back(get_jato(sprinklers, resultados[i]));
                        i++;
                }
        }
        else {
                i = 0;
                for(it = grass_blocks.begin(); it != grass_blocks.end(); it++) {
                        resultados.push_back(block_result());
                        resultados[i].x = (*it).x/100;
                        resultados[i].y = (*it).y/100;
                        resultados[i].qtd_agua = 0;
                        i++;
                }
        }

        imwrite("x.jpg", final_field);
        imshow("Final Result", final_field)
        remove("resAnterior");
        save_Angulos(jatos);
        // system("python send.py");
        save_resAnterior(resultados, output);

        return 0;
}
