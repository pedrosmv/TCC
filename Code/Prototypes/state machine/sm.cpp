#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

struct parameters {
  int corGrama;
  int vento;
  int insolacao;
  int umidade;
  float coeficienteChuva;
  float resultadoAnterior;
};

float formula(parameters p ){
  float result;

  result = 100*p.coeficienteChuva*((p.corGrama + (p.vento*p.insolacao)/p.umidade) - p.resultadoAnterior)/30;

  return result;
}

int main(){

  parameters p;
  float result;

  p.corGrama = 10;
  p.vento = 3;
  p.insolacao = 2;
  p.umidade = 3;
  p.coeficienteChuva = 0.53;
  p.resultadoAnterior = 0;

  result = formula(p);

  cout << result;


}
