// YL-39 + YL-69 humidity sensor
int pino_sensor = A0;
int valor;
void setup()
  Serial.begin(9600);
}

void loop() {
  valor = analogRead(pino_sensor);
  Serial.print("Umidade (0-1023): ");
  Serial.println(valor);
  delay(2000);
}
