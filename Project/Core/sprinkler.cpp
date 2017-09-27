#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>       /* sqrt */

#include "imageprocessing.h"
#include "sprinkler.h"


using namespace std;

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

float distancia (float xa, float xb, float ya, float yb){
        float distancia = 0;
        float equacao;
        cout << "xa: " << xa << "xb: " << xb << "ya: " << ya << "yb: " << yb << endl;

        equacao = ((xb*xb) - (xa*xa)) + ((yb*yb) - (ya*ya));
        equacao = fabs(equacao);
        if(equacao <1) {
                equacao = 0;
        }
        return distancia = sqrt(equacao);
}

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

jato get_jato(vector<sprinkler> sprinklers, map_block grass_block){

        jato jato;
        if(grass_block.block_numx <= 1 ) {
                jato.distancia = distancia(sprinklers[0].x, (float) grass_block.block_numx, sprinklers[0].y, (float) grass_block.block_numy);
                jato.angle = angulo(sprinklers[0].x, (float) grass_block.block_numx, sprinklers[0].y, (float) grass_block.block_numy);
                jato.orientacao = sprinklers[0].orientacao;
        }
        else if((grass_block.block_numx > 1 && grass_block.block_numx <= 4) && grass_block.block_numy <= 1.5) {
                jato.distancia = distancia(sprinklers[3].x, (float) grass_block.block_numx, sprinklers[3].y, (float) grass_block.block_numy);
                jato.angle = angulo(sprinklers[3].x, (float) grass_block.block_numx, sprinklers[3].y, (float) grass_block.block_numy);
                jato.orientacao = sprinklers[3].orientacao;
        }
        else if(grass_block.block_numx > 4) {
                jato.distancia = distancia(sprinklers[2].x, (float) grass_block.block_numx, sprinklers[2].y, (float) grass_block.block_numy);
                jato.angle = angulo(sprinklers[2].x, grass_block.block_numx, sprinklers[2].y, grass_block.block_numy);
                jato.orientacao = sprinklers[2].orientacao;
        }
        else if((grass_block.block_numx > 1 && grass_block.block_numx <= 4) && grass_block.block_numy > 1.5) {
                jato.distancia = distancia(sprinklers[1].x, (float) grass_block.block_numx, sprinklers[1].y, (float) grass_block.block_numy);
                jato.angle = angulo(sprinklers[1].x, (float) grass_block.block_numx, sprinklers[1].y, (float) grass_block.block_numy);
                jato.orientacao = sprinklers[1].orientacao;
        }

        return jato;

}
