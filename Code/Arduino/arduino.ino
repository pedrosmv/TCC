#include <Servo.h>
Servo myservo;  // create servo object to control a servo

// YL-39 + YL-69 humidity sensor
int pino_umidade = A0;
int pino_insolacao = A1;

int umidade;
int ins;
void setup() {
        myservo.attach(9);
        Serial.begin(9600);
}

char rx_byte = 0;
String rx_str = "";
boolean numero = true;
int resultado;


void loop() {
        umidade = analogRead(pino_umidade);
        ins = analogRead(pino_insolacao);

        Serial.print("Umidade: ");
        Serial.println(umidade);

        Serial.print("Insolacao: ");
        float voltage = ins * (5.0 / 1023.0);;
        Serial.println(voltage);

        if (Serial.available() > 0) {
                rx_byte = Serial.read();

                if ((rx_byte >= '0') && (rx_byte <= '9')) {
                        rx_str += rx_byte;
                }
                else if (rx_byte == '\n') {
                        if (numero) {
                                Serial.println("Not a number");
                        }
                        else {
                                resultado = rx_str.toInt();
                                Serial.print(resultado);

                        }
                        numero = true;
                        rx_str = "";
                }
                else {
                        numero = false;
                }
        }

        myservo.write(resultado);

        delay(5000);
}
