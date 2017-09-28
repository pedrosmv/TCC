#include "decision.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>

float calcula_coeficiente(int weather[], float rain[]){
        int i = 0;
        float coefficient = 0;
        for (i=0; i<12; i++) {
                coefficient = coefficient + (weather[i] * (1 -(rain[i]/100)));
        }

        return coefficient/39;
}

float parse_weather(){
        string stream;
        string s;
        const string parameter = s;
        int i=0;
        int j=0;
        int weather_coefficient[12];
        float rain_percentage[12];
        float coefficient;
        system("curl wttr.in/'London' > weather.txt");

        ifstream weather("weather.txt");
        rain_percentage[0] = 0.0;
        /* This loop will parse the file where the weather report is saved and for every
           condition it will assign a value. These values are:
           Sunny = 3
           Clear = 2
           Cloudy = 1
           Overcast = 1
           Rain = 0
           Snow = 0
           These values are coefficients that will be used in the decision whether the
           selected tile will be irrigated or not. Besides the coefficients, the rain prediction
           will be used as well to calculate a final coefficient for the given day. */
        while ( i < 12 )
        {
                weather >> stream;
                size_t sunny = stream.find("Sunny", 0);
                if(sunny != string::npos)
                {
                        weather_coefficient[j] = 3;
                        j++;
                }
                size_t clear = stream.find("Clear", 0);
                if(clear != string::npos)
                {
                        weather_coefficient[j] = 2;
                        j++;
                }
                size_t rain = stream.find("Rain", 0);
                if(rain != string::npos)
                {
                        weather_coefficient[j] = 0;
                        j++;
                }
                rain = stream.find("rain", 0);
                if(rain != string::npos)
                {
                        weather_coefficient[j] = 0;
                        j++;
                }
                size_t cloudy = stream.find("Cloudy", 0);
                if(cloudy != string::npos)
                {
                        weather_coefficient[j] = 1;
                        j++;
                }
                cloudy = stream.find("cloudy", 0);
                if(cloudy != string::npos)
                {
                        weather_coefficient[j] = 1;
                        j++;
                }
                size_t snow = stream.find("Snow", 0);
                if(snow != string::npos)
                {
                        weather_coefficient[j] = 0;
                        j++;
                }
                size_t overcast = stream.find("Overcast", 0);
                if(overcast != string::npos)
                {
                        weather_coefficient[j] = 1;
                        j++;
                }
                size_t found = stream.find('%');
                if(found != string::npos)
                {
                        float aux;
                        aux = stof(stream);
                        rain_percentage[i+1] = aux;
                        i++;
                }
        }

        return coefficient = calcula_coeficiente(weather_coefficient, rain_percentage);
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

int get_cor(bool regado){

        if(regado) {
                return 80;
        }

        return 0;
}


float formula(parameters p ){
        float result;

        result = p.coeficienteChuva*((p.corGrama + ((p.vento*p.insolacao)/p.umidade)) - p.resultadoAnterior);
        if (result < 0) {
                result = 0;
        }
        return result;
}

int state_machine(parameters param_dia){
        enum estado estado_atual = INICIAL;
        float decisao;
        int qtd_agua;

        while(estado_atual != FIM) {

                switch(estado_atual) {
                case INICIAL:

                        decisao = 100 * param_dia.coeficienteChuva;

                        if (decisao > 10) {
                                estado_atual = CALC;
                        }
                        else{
                                estado_atual = NAOREGA;
                        }
                        break;

                case CALC:
                        decisao = formula(param_dia);
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
                        qtd_agua = 100;
                        estado_atual = FIM;
                        break;

                case REGA70:
                        rega(70);
                        qtd_agua = 70;
                        estado_atual = FIM;
                        break;

                case REGA40:
                        rega(40);
                        qtd_agua = 40;
                        estado_atual = FIM;
                        break;

                case NAOREGA:
                        rega(0);
                        qtd_agua = 0;
                        estado_atual = FIM;
                        break;

                case FIM:
                        break;
                }
        }
        return qtd_agua;
}

void rega(int quantidade_agua){
        // cout << "A area selecionada foi regada em " << quantidade_agua << "% " << endl;
}

void save_resAnterior(vector<block_result> resultados, ofstream &output){
        vector<block_result>::iterator itr;

        output.open("resAnterior", ios::app);
        for (itr = resultados.begin(); itr != resultados.end(); itr++) {
                output << (*itr).x << " " << (*itr).y << " " << (*itr).qtd_agua << endl;
        }
        output << " ";
        output.close();
}

int get_resAnterior(int x, int y, ifstream &input){
        int res_x, res_y, qtd_agua;
        input.open("resAnterior");
        string line;
        while ( getline (input,line) )
        {
                istringstream resultado(line);
                resultado >> res_x >> res_y >> qtd_agua;
                if(res_x == x && res_y == y) {
                        input.close();
                        return qtd_agua;
                }
        }
        input.close();
        return 0;
}
