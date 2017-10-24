// YL-39 + YL-69 humidity sensor
int pino_umidade = A0;
int pino_insolacao = A1;

int umidade;
int ins;
void setup() {
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

        if (Serial.available() > 0) { // is a character available?
                rx_byte = Serial.read(); // get the character

                if ((rx_byte >= '0') && (rx_byte <= '9')) {
                        rx_str += rx_byte;
                }
                else if (rx_byte == '\n') {
                        // end of string
                        if (!numero) {
                                Serial.println("Not a number");
                        }
                        else {
                                // multiply the number by 2
                                result = rx_str.toInt();
                                // print the result
                                Serial.print(result);

                        }
                        numero = true; // reset flag
                        rx_str = ""; // clear the string for reuse
                }
                else {
                        // non-number character received
                        numero = false; // flag a non-number
                }
        } // end: if (Serial.available() > 0)


        delay(2000);
}
