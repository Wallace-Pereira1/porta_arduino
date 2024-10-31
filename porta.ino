#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

String senha = "9999"; // Senha padrão
String buf = "";

const byte LINHAS = 4;
const byte COLUNAS = 3;
char KEYS[LINHAS][COLUNAS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pino_linha[LINHAS] = {2, 3, 4, 5};
byte pino_coluna[COLUNAS] = {6, 7, 8};

Keypad keypad = Keypad(makeKeymap(KEYS), pino_linha, pino_coluna, LINHAS, COLUNAS);

Servo servo_9;

int greenLedPin = 10;
int redLedPin = 11;
int buzzerPin = 12;

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("DIGITE A SENHA");

  servo_9.attach(9);
  servo_9.write(0);

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    switch (key) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        lcd.setCursor(buf.length(), 1);
        lcd.print(key);
        buf += key;
        break;

      case '*': // Limpar a entrada
        buf = "";
        lcd.setCursor(0, 1);
        lcd.print("               ");
        break;

      case '#': 
        if (buf == senha) { 
          lcd.setCursor(0, 1);
          lcd.print("ACESSO LIBERADO");
          digitalWrite(greenLedPin, HIGH);
          digitalWrite(redLedPin, LOW);
          digitalWrite(buzzerPin, HIGH);
          delay(1000);
          digitalWrite(buzzerPin, LOW);
          servo_9.write(90); 
          delay(6000); 
          servo_9.write(0); 
          digitalWrite(greenLedPin, LOW); 

          // Obter o tempo desde que o Arduino começou
          unsigned long tempo = millis();
          int horas = (tempo / 3600000) % 24;
          int minutos = (tempo / 60000) % 60;
          int segundos = (tempo / 1000) % 60;

          // Enviar hora para o Python
          Serial.print("Acesso Liberado às: ");
          Serial.print(horas);
          Serial.print(":");
          Serial.print(minutos);
          Serial.print(":");
          Serial.println(segundos);
        } else { 
          lcd.setCursor(0, 1);
          lcd.print("ACESSO NEGADO");
          digitalWrite(greenLedPin, LOW);
          digitalWrite(redLedPin, HIGH);
          delay(2000);
          digitalWrite(redLedPin, LOW);
          servo_9.write(0); 

          Serial.println("Acesso Negado"); // Envia mensagem para o Python
        }
        buf = "";
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("               ");
        break;
    }
  }

  delay(100); 
}
