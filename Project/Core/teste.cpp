#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <vector>
#include <string>     /* sqrt */
#define PI 3.14159265

using namespace std;

int angulo (float xa, float xb, float ya, float yb){
        int angulo;
        float cos0, cos0_den, cos0_div;

        cout << "xa: " << xa << "xb: " << xb << "ya: " << ya << "yb: " << yb << endl;

        if(((xa == 0) && (ya ==0)) || ((xb == 0) && (yb ==0))) {
                return 180;
        }

        cos0_den = (xa*xb) + (ya*yb);


        cout << "(xa*xb) + (ya*yb): " << cos0_den;
        float xa2 = pow(xa, 2);
        float xb2 = pow(xb, 2);
        float ya2 = pow(ya, 2);
        float yb2 = pow(yb, 2);
        cout << "X: " << xa2 << xb2 << endl;
        cout << "Y: " << ya2 << yb2 << endl;
        cos0_div = sqrt(xa2+ ya2)*sqrt(xb2 +yb2);
        cout << "cos0_div: " << cos0_div << endl;
        // cos0_div = sqrt(pow(xa, 2) + pow(ya, 2))*sqrt(pow(xb, 2) + pow(yb, 2));
        // cout << "sqrt((xa*xa) + (ya*ya))*sqrt((xb*xb) + (yb*yb)): "<< cos0_div << endl;
        cos0 = cos0_den/cos0_div;
        cout << "cos0: " << cos0;
        cos0 = fabs(cos0);
        angulo = acos (cos0) * 180.0 / PI;

        cout << "@@@@@@@@@@@@@@@@" << angulo << endl;
        return angulo;
}


int main(){

        int ang;

        ang = angulo(3, 0, 0, 0);
        cout << ang;
}
