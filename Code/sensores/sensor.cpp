#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(){
        string stream;
        ifstream file;
        string aux;
        string tmp;
        float umidade_valor, insolacao_valor;

        file.open("sensordata.txt");

        while (getline (file, stream)) {

                stringstream line(stream);
                size_t found = stream.find("Umidade", 0);
                if(found != string::npos)
                {
                        line >> aux >> umidade_valor;
                        cout << umidade_valor << endl;
                }
                found = stream.find("Insolacao", 0);
                if(found != string::npos)
                {
                        line >> aux >> insolacao_valor;
                        cout << insolacao_valor << endl;
                }


        }
        return 0;
}
