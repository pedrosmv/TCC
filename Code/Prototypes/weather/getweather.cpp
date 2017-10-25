#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>

using namespace std;

float calculate_coeficiente(int tempo[], float rain[]){
        int i = 0;
        float coeficiente = 0;
        for (i=0; i<12; i++) {
                coeficiente = coeficiente + (tempo[i] * (1 -(rain[i]/100)));
        }

        return coeficiente/39;
}

float find_wind(){
        string stream;
        float vento;
        string result;
        system("ansitempo -l Campinas > t.txt");
        ifstream file("t.txt");
        stream.assign( (istreambuf_iterator<char>(file)),
                       (istreambuf_iterator<char>()));

        const string s = stream;
        std::string::const_iterator start, end;
        start = s.begin();
        end   = s.end();
        regex expr{"[0-9]?[0-9]?(\.[0-9][0-9]?)? m/s"};
        smatch match;

        while(regex_search(start, end,match, expr)) {
                result = match[0];
                start = match[0].second;
        }
        cout << result << endl;

        vento = stof(result);
        cout << vento << endl;
        file.close();
        return vento = vento * 3.6;
}

int main(){
        string stream;
        string s;
        const string parameter = s;
        int i=0;
        int j=0;
        int nclear = 0;
        int nrain = 0;
        int ncloudy = 0;
        int nsunny = 0;
        int nsnow = 0;
        int novercast = 0;
        int tempo_coeficiente[12];
        float rain_percentage[12];
        float coeficiente;
        system("curl wttr.in/'London' > tempo.txt");



        find_wind();

        ifstream tempo("tempo.txt");
        rain_percentage[0] = 0.0;
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
                        nsunny++;
                        tempo_coeficiente[j] = 3;
                        j++;
                }
                size_t clear = stream.find("Clear", 0);
                if(clear != string::npos)
                {
                        nclear++;
                        tempo_coeficiente[j] = 2;
                        j++;
                }
                size_t rain = stream.find("Rain", 0);
                if(rain != string::npos)
                {
                        nrain++;
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                rain = stream.find("rain", 0);
                if(rain != string::npos)
                {
                        nrain++;
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t cloudy = stream.find("Cloudy", 0);
                if(cloudy != string::npos)
                {
                        ncloudy++;
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                cloudy = stream.find("cloudy", 0);
                if(cloudy != string::npos)
                {
                        ncloudy++;
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                size_t snow = stream.find("Snow", 0);
                if(snow != string::npos)
                {
                        nsnow++;
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t overcast = stream.find("Overcast", 0);
                if(overcast != string::npos)
                {
                        novercast++;
                        tempo_coeficiente[j] = 1;
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
        coeficiente = calculate_coeficiente(tempo_coeficiente, rain_percentage);

        tempo.close();
        return 0;
}
