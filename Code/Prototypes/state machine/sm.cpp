#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct parameters {
        int corGrama;
        int vento;
        int insolacao;
        int umidade;
        float coeficienteChuva;
        float resultadoAnterior;
};

enum estado {INICIAL, NAOREGA, CALC, REGA100, REGA70, REGA40, FIM};

float calcula_coeficiente(int tempo[], float chuva[]){
        int i = 0;
        float coeficiente = 0;
        for (i=0; i<12; i++) {
                coeficiente = coeficiente + (tempo[i] * (1 -(chuva[i]/100)));
        }

        return coeficiente/39;
}

float coeficiente_chuva(){
        string stream;
        string s;
        int i=0;
        int j=0;
        int tempo_coeficiente[12];
        float chuva_percentage[12];
        float coeficiente;
        system("curl wttr.in/'London' > tempo.txt");


        ifstream tempo("tempo.txt");
        chuva_percentage[0] = 0.0;
        /* This loop will parse the file where the tempo report is saved and for every
           condition it will assign a value. These values are:
           Sunny = 3
           Clear = 2
           Cloudy = 1
           Overcast = 1
           Rain = 0
           Snow = 0
           These values are coeficientes that will be used in the decision whether the
           selected tile will be irrigated or not. Besides the coeficientes, the rain prediction
           will be used as well to calculate a final coeficiente for the given day. */
        while ( i < 12 )
        {
                tempo >> stream;
                size_t sunny = stream.find("Sunny", 0);
                if(sunny != string::npos)
                {
                        tempo_coeficiente[j] = 3;
                        j++;
                }
                size_t clear = stream.find("Clear", 0);
                if(clear != string::npos)
                {
                        tempo_coeficiente[j] = 2;
                        j++;
                }
                size_t rain = stream.find("Rain", 0);
                if(rain != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                rain = stream.find("rain", 0);
                if(rain != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t cloudy = stream.find("Cloudy", 0);
                if(cloudy != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                cloudy = stream.find("cloudy", 0);
                if(cloudy != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                size_t snow = stream.find("Snow", 0);
                if(snow != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t overcast = stream.find("Overcast", 0);
                if(overcast != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                size_t found = stream.find('%');
                if(found != string::npos)
                {
                        float aux;
                        aux = stof(stream);
                        chuva_percentage[i+1] = aux;
                        i++;
                }
        }
        coeficiente = calcula_coeficiente(tempo_coeficiente, chuva_percentage);

        tempo.close();

        return coeficiente;
}

int get_vento(){
        srand (time(NULL));
        int vento;

        vento = rand() % 5 + 1;

        return vento;
}

int get_umidade(){
        srand (time(NULL));
        int umidade;

        umidade = rand() % 4 + 1;
        return umidade;
}

int get_insolacao(){
        srand (time(NULL));
        int insolacao;

        insolacao = rand() % 4 + 1;
        return insolacao;
}

int get_cor(){
        srand (time(NULL));
        int cor;

        cor = rand() % 80;
        return cor;
}

float resultadoAnterior(){
        srand (time(NULL));
        float resultado;

        resultado = rand() % 100;
        cout << resultado << endl;
        return resultado;
}

void get_parameters(parameters* p){
        p->vento = get_vento();
        p->corGrama = get_cor();
        p->insolacao = get_insolacao();
        p->umidade = get_umidade();
        p->coeficienteChuva = coeficiente_chuva();
        p->resultadoAnterior = resultadoAnterior();
}

float formula(parameters p ){
        float result;

        result = p.coeficienteChuva*((p.corGrama + ((p.vento*p.insolacao)/p.umidade)) - p.resultadoAnterior);
        if (result < 0) {
                result = 0;
        }
        return result;
}

void rega(int quantidade_agua){
        cout << "A area selecionada foi regada em " << quantidade_agua << "% " << endl;
}

void state_machine(){
        enum estado estado_atual = INICIAL;
        float decisao;
        parameters param_dia;

        while(estado_atual != FIM) {

                switch(estado_atual) {
                case INICIAL:

                        decisao = 100 * coeficiente_chuva();

                        if (decisao > 10) {
                                estado_atual = CALC;
                        }
                        else{
                                estado_atual = NAOREGA;
                        }
                        break;

                case CALC:
                        get_parameters(&param_dia);
                        decisao = formula(param_dia);
                        cout << decisao;
                        // salva resultado em arquivo
                        if(decisao > 70) {
                                estado_atual = REGA100;
                        }
                        else if (decisao > 50 && decisao < 70) {
                                estado_atual = REGA70;
                        }
                        else if (decisao > 10 && decisao < 50) {
                                estado_atual = REGA40;
                        }
                        else {
                                estado_atual = NAOREGA;
                        }
                        break;

                case REGA100:
                        rega(100);
                        estado_atual = FIM;
                        break;

                case REGA70:
                        rega(70);
                        estado_atual = FIM;
                        break;

                case REGA40:
                        rega(40);
                        estado_atual = FIM;
                        break;

                case NAOREGA:
                        rega(0);
                        estado_atual = FIM;
                        break;

                case FIM:
                        break;
                }
        }
}

int main(){

        state_machine();

}
