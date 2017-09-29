#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>       /* sqrt */

#include "imageprocessing.h"
#include "sprinkler.h"
#include "decision.h"


using namespace std;

/* Funcao para caso se leia os sprinklers de um arquivo, provavelmente sera deletada */
vector<sprinkler> read_sprinklers(){
        ifstream input;
        vector<sprinkler> sprinklers;
        int i = 0;

        input.open("sprinklerpos.txt");
        string line;
        while (getline (input,line))
        {
                sprinklers.push_back(sprinkler());
                istringstream resultado(line);
                resultado >> sprinklers[i].x >> sprinklers[i].y;
                if(sprinklers[i].x < 1 ) {
                        sprinklers[i].orientacao = ESQUERDA;
                }
                else if((sprinklers[i].x > 1 && sprinklers[i].x < 4) && sprinklers[i].y < 1.5) {
                        sprinklers[i].orientacao = CIMA;
                }
                else if(sprinklers[i].x > 3) {
                        sprinklers[i].orientacao = DIREITA;
                }
                else if((sprinklers[i].x > 1 && sprinklers[i].x < 4) && sprinklers[i].y > 1.5) {
                        sprinklers[i].orientacao = BAIXO;
                }
                i++;
        }


        input.close();
        return sprinklers;
}

/* Funcao responsavel por atribuir a posicao dos sprinklers */
vector<sprinkler> set_sprinklers(int max_col, int max_row, int raio){
        vector<sprinkler> sprinklers;
        int valx = raio;
        int valy = 0;
        int cont_sp = 0;
        int n_sprinklers_x;
        int n_sprinklers_y;

        n_sprinklers_x = floor(2*((max_col + 1)/(raio*2)));
        n_sprinklers_y = floor(2*((max_row + 1)/(raio*2)));

        for(int i=0; i<n_sprinklers_x; i++) {
                sprinklers.push_back(sprinkler());
                if(valx > max_col) {
                        valx = raio;
                        valy = max_row;
                }

                sprinklers[i].x = valx*5;
                sprinklers[i].y = valy*5;
                valx = valx + raio*2;
                cont_sp++;
        }
        valy = 0;
        valx = raio;
        for(int i=0; i<n_sprinklers_y; i++) {
                sprinklers.push_back(sprinkler());
                if(valx > max_row) {
                        valx = raio;
                        valy = max_col;
                }

                sprinklers[cont_sp].x = valy*5;
                sprinklers[cont_sp].y = valx*5;
                valx = valx + raio*2;
                cont_sp++;
        }
        sprinklers[0].orientacao = CIMA;
        sprinklers[1].orientacao = BAIXO;
        sprinklers[2].orientacao = ESQUERDA;
        sprinklers[3].orientacao = DIREITA;

        return sprinklers;
}

/* Calcula a distancia entre dois pontos (bloco a ser regado e sprinkler) */
float distancia (float xa, float xb, float ya, float yb){
        float distancia = 0;
        float equacao;

        equacao = ((xb*xb) - (xa*xa)) + ((yb*yb) - (ya*ya));
        equacao = fabs(equacao);
        if(equacao <1) {
                equacao = 0;
        }
        return distancia = sqrt(equacao)/5;
}

/* Calcula o angulo entre dois pontos (bloco a ser regado e sprinkler) */
int angulo (float xa, float xb, float ya, float yb){
        int angulo;
        float cos0, cos0_den, cos0_div;

        if(((xa == 0) && (ya ==0)) || ((xb == 0) && (yb ==0))) {
                return 180;
        }
        cos0_den = (xa*xb) + (ya*yb);
        float xa2 = pow(xa, 2);
        float xb2 = pow(xb, 2);
        float ya2 = pow(ya, 2);
        float yb2 = pow(yb, 2);
        cos0_div = sqrt(xa2+ ya2)*sqrt(xb2 +yb2);
        cos0 = cos0_den/cos0_div;
        if (cos0 >= 1.0) {
                return 0;
        }
        angulo = acos (cos0) * 180.0 / PI;
        return angulo;
}

/* Funcao para pegar as informações do jato */
jato get_jato(vector<sprinkler> sprinklers, block_result grass_block, int max_row, int max_col){

        jato jato;
        if((grass_block.x)/5 < 2 ) {
                jato.distancia = distancia(sprinklers[2].x, grass_block.x, sprinklers[2].y, grass_block.y);
                jato.angle = angulo(sprinklers[2].x, grass_block.x, sprinklers[2].y, grass_block.y);
                jato.orientacao = sprinklers[2].orientacao;
        }
        else if(((grass_block.x)/5 >= 2 && (grass_block.x)/5 < 5) && (grass_block.y)/5 < 2) {
                jato.distancia = distancia(sprinklers[0].x, grass_block.x, sprinklers[0].y, grass_block.y);
                jato.angle = angulo(sprinklers[0].x, grass_block.x, sprinklers[0].y, grass_block.y);
                jato.orientacao = sprinklers[0].orientacao;
        }
        else if((grass_block.x)/5  == 5) {
                jato.distancia = distancia(sprinklers[3].x, grass_block.x, sprinklers[2].y, grass_block.y);
                jato.angle = angulo(sprinklers[3].x, (grass_block.x)/100, sprinklers[3].y, grass_block.y);
                jato.orientacao = sprinklers[3].orientacao;
        }
        else if(((grass_block.x)/5 > 1 && (grass_block.x)/5 <= 4) && (grass_block.y)/5 >=2 ) {
                jato.distancia = distancia(sprinklers[1].x, grass_block.x, sprinklers[1].y, grass_block.y);
                jato.angle = angulo(sprinklers[1].x, grass_block.x, sprinklers[1].y, grass_block.y);
                jato.orientacao = sprinklers[1].orientacao;
        }

        return jato;

}
