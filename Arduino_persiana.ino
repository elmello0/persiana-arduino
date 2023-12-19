#include <SoftwareSerial.h>

// Pines para la comunicación Serial con ESP8266
const int RX_PIN = 10; // Conectar al TX del ESP8266
const int TX_PIN = 11; // Conectar al RX del ESP8266

// Crea una instancia de SoftwareSerial
SoftwareSerial esp8266(RX_PIN, TX_PIN);

// Credenciales de Wi-Fi para el modo AP del ESP8266
const char* ssid = "Alan 2,4G";
const char* password = "Fran2002";

// Entradas y salidas para los botones y el motor
int sw_arriba = 2;
int sw_abajo = 3;
int giro_derecha = 8;
int giro_izquierda = 9;

void setup() {
  // Configuración de los pines de entrada para los botones
  pinMode(sw_arriba, INPUT_PULLUP);
  pinMode(sw_abajo, INPUT_PULLUP);
  
  // Configuración de los pines de salida para el motor
  pinMode(giro_derecha, OUTPUT);
  pinMode(giro_izquierda, OUTPUT);
  digitalWrite(giro_derecha, LOW);
  digitalWrite(giro_izquierda, LOW);

  // Inicia la comunicación Serial con el ordenador
  Serial.begin(115200);
  
  // Inicia la comunicación Serial con el ESP8266
  esp8266.begin(115200); 
  
  // Comandos AT para configurar el ESP8266
  esp8266.println("AT"); // Comando de prueba para verificar la comunicación
  delay(1000);
  esp8266.println("AT+CWMODE=2"); // Establece el ESP8266 en modo AP
  delay(1000);
  esp8266.println("AT+CWSAP=\"" + String(ssid) + "\",\"" + String(password) + "\",1,3"); // Configura el SSID y contraseña del AP
  delay(1000);
  esp8266.println("AT+CIPMUX=1"); // Habilita múltiples conexiones
  delay(1000);
  esp8266.println("AT+CIPSERVER=1,80"); // Inicia el servidor en el puerto 80
  delay(1000);
}

void loop() {
  // Comprueba el estado de los botones y actúa en consecuencia
  if (digitalRead(sw_arriba) == LOW) {
    // Si el botón de subir está presionado, subir la persiana
    subir_persiana();
  }
  else if (digitalRead(sw_abajo) == LOW) {
    // Si el botón de bajar está presionado, bajar la persiana
    bajar_persiana();
  }
  else {
    // Si no se está presionando ninguno de los botones, detener el motor
    parar_motor();
  }
  
  // Comprueba si hay datos disponibles desde el ESP8266
  if (esp8266.available()) {
    String command = esp8266.readStringUntil('\n');
    
    if (command.indexOf("/SUBIR") != -1) {
      subir_persiana();
    }
    else if (command.indexOf("/BAJAR") != -1) {
      bajar_persiana();
    }
  }
}

void subir_persiana() {
  digitalWrite(giro_derecha, LOW);
  digitalWrite(giro_izquierda, HIGH); // Activa el motor para subir la persiana
}

void bajar_persiana() {
  digitalWrite(giro_derecha, HIGH); // Activa el motor para bajar la persiana
  digitalWrite(giro_izquierda, LOW);
}

void parar_motor() {
  digitalWrite(giro_derecha, LOW);
  digitalWrite(giro_izquierda, LOW); // Apaga ambos pines para detener el motor
}
