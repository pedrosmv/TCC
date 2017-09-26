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
  int qtd_agua;
  int vento, umidade, insolacao;
  ofstream output;
  ifstream input;

  Mat field;
  vector<map_block> grass_blocks;
  vector<map_block>::iterator it;
  vector<block_result> resultados;


  /* --- Variaveis de decisao */
  coeficiente = parse_weather();
  vento = get_vento();
  umidade = get_umidade();
  insolacao = get_insolacao();

  /* Processamento de imagem */
  // field = stich(argc, argv);
  field = imread("field.jpg");
  grass_blocks = image_processing(field);

  /* Tomada de decisão */
  for(it = grass_blocks.begin(); it != grass_blocks.end(); it++){
    parameters p;
    p.coeficienteChuva = coeficiente;
    p.umidade = umidade;
    p.vento = vento;
    p.insolacao = insolacao;
    p.resultadoAnterior = get_resAnterior((*it).block_numx, (*it).block_numy, input);
    p.corGrama = get_cor((*it).regado);
    qtd_agua = state_machine(p);

    resultados.push_back(block_result());
    resultados[i].x = (*it).block_numx;
    resultados[i].y = (*it).block_numy;
    resultados[i].qtd_agua = qtd_agua;
    i++;
  }

  remove("resAnterior");
  save_resAnterior(resultados, output);

  return 0;
}
