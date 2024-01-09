#include <Arduino.h>
#include <ESP8266WiFi.h>

const int MAX_STRING_LEN = 50;

char ssid[MAX_STRING_LEN];
char password[MAX_STRING_LEN];
const int pinSpeaker = D8;

int PEPERANDOM1 = 0;
int PEPERANDOM2 = 0;
int PEPESUMA = 0;

WiFiServer server(80);

void setup() {
  pinMode(pinSpeaker, OUTPUT);

  randomSeed(analogRead(A0));

  Serial.begin(115200);
  delay(1000);

  Serial.println("Ingrese la red: ");
  Serial.setTimeout(60000);
  Serial.readBytesUntil('\n', ssid, MAX_STRING_LEN - 1);
  ssid[strlen(ssid)] = '\0';

  Serial.println("Ingrese la contraseña: ");
  Serial.setTimeout(60000);
  Serial.readBytesUntil('\n', password, MAX_STRING_LEN - 1);
  password[strlen(password)] = '\0';

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Conectado a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexion establecida");

  server.begin();
  Serial.println("Server iniciado");

  Serial.print("IP:");
  Serial.print(WiFi.localIP());

  generarNumerosAleatorios();
}

void generarNumerosAleatorios() {
  PEPERANDOM1 = random(1, 21);
  PEPERANDOM2 = random(1, 21);
  PEPESUMA = PEPERANDOM1 + PEPERANDOM2;
}

void playTone(int frequency, int duration) {
  tone(pinSpeaker, frequency, duration);
  delay(duration + 20);
}

void playOdeToJoy() {
  int melody[] = {659, 659, 698, 784, 784, 698, 659, 587, 523, 523, 587, 659, 659, 587, 587, 659, 659, 698, 784, 784, 698, 659, 587, 523, 523, 587, 659, 587, 523};
  int duration[] = {8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 4, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 4, 8, 4, 4};

  for (int PEPE1 = 0; PEPE1 < sizeof(melody) / sizeof(melody[0]); PEPE1++) {
    playTone(melody[PEPE1], duration[PEPE1] * 50);
  }
}

void playErrorMelody() {
  int melody[] = {523, 587, 659, 698, 784, 880, 988, 1047, 1175, 1319, 1397, 1568};
  int duration[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 200};

  for (int PEPE2 = 0; PEPE2 < sizeof(melody) / sizeof(melody[0]); PEPE2++) {
    playTone(melody[PEPE2], duration[PEPE2]);
  }
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) { 
    Serial.println("Nuevo Cliente");
    
    while (!client.available() && client.connected()) {
      delay(1);
    }
    
    client.flush(); 
                
    Serial.println("Enviando respuesta...");   
  
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Cache-Control: no-store, must-revalidate");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE HTML>");
    client.println("<html style='background-color:#55ffff;'>");
    client.println("<body>");
    client.println("<div style='display:flex; flex-direction:column; justify-content:center; align-items:center'>");
    client.println("<br />");    
    client.println("<h1 style='color:#ff55ff'>Juegolin</h1>");
    client.println("<div style='color:#ffff55; display:flex; justify-content:space-between; align-items: center'>");
    client.println("<p>" + String(PEPERANDOM1) + "</p>");
    client.println("<p> + </p>");
    client.println("<p>" + String(PEPERANDOM2) + "</p>");
    client.println("</div>");
    client.println("<br />");
    client.println("<form action='/' method='get' onsubmit='return checkAnswer()'>");
    client.println("<input style='border-style: inset;' type='text' name='respuesta' />");
    client.println("<input style='border-style: outset;' type='submit' value='Comprobar' />");
    client.println("</form>");
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");

    if (client.available()) {
      String request = client.readStringUntil('\r');
      if (request.indexOf("GET /favicon.ico") == -1) {
        int index = request.indexOf("GET /?respuesta=");
        if (index != -1) {
          int endIndex = request.indexOf("HTTP/1.1");
          String respuestaUsuario = request.substring(index + 16, endIndex);

          if (respuestaUsuario.toInt() == PEPESUMA) {
            playOdeToJoy();
            generarNumerosAleatorios();
          } else {
            playErrorMelody();
          }
        }
      }
    }
  }
}
