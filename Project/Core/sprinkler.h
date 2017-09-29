#ifndef SPRINKLER_H
#define SPRINKLER_H
#define PI 3.14159265
#include <stdio.h>
#include "decision.h"
#include "imageprocessing.h"

using namespace std;

enum orientacao {ESQUERDA, DIREITA, CIMA, BAIXO};

/* Essa struct representa a posicao do sprinkler e sua orientacao */
struct sprinkler {
        float x;
        float y;
        enum orientacao orientacao;
};

/* Essa struct guarda as informacoes para o jato dagua */
struct jato {
        float distancia;
        int angle;
        int qtd_agua;
        enum orientacao orientacao;
};

jato get_jato(vector<sprinkler> sprinklers, block_result grass_block, int max_row, int max_col);
vector<sprinkler> read_sprinklers();
vector<sprinkler> set_sprinklers(int max_col, int max_row, int raio);
int angulo (float xa, float xb, float ya, float yb);
float distancia (float xa, float xb, float ya, float yb);

#endif
