#ifndef SPRINKLER_H
#define SPRINKLER_H
#define PI 3.14159265
#include <stdio.h>

#include "imageprocessing.h"
using namespace std;

enum orientacao {ESQUERDA, DIREITA, CIMA, BAIXO};

struct sprinkler {
        float x;
        float y;
        enum orientacao orientacao;
};

struct jato {
        float distancia;
        int angle;
        enum orientacao orientacao;
};

jato get_jato(vector<sprinkler> sprinklers, map_block grass_block, int max_row, int max_col);
int angulo (float xa, float xb, float ya, float yb);
float distancia (float xa, float xb, float ya, float yb);
vector<sprinkler> read_sprinklers();
vector<sprinkler> set_sprinklers(int max_col, int max_row, int raio);

#endif
