#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>


// Definiciones de pines y constantes
#define SS_PIN 5
#define RST_PIN 22

// Configuración de WiFi
const char* ssid = "IZZI-DA6C";
const char* password = "AZ3mtcErcpp6NmfeaZ";

// Configuración de MQTT
const char* mqttServer = "192.168.0.48";   //IP local
//const char* mqttServer = "52.28.62.138";     // IP Broker Publico
const int mqttPort = 1883;
const char* mqttTopic = "Proyecto/mqtt/ProyectoPoliciaco";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Definiciones de objetos y variables globales
Servo myservo;
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
//--------------------------------------------------------------------------------------------------
//SE ASIGNAN LOS DATOS HEXADECIMALES A LAS CONSTANTES EN CADENAS DE CARACTER
String DatoHex;                         
const String patrulla3 = "C3BC8193";     
const String patrulla1 = "832E5C93";
const String patrulla2 = "D35BA293";
//--------------------------------------------------------------------------------------------------
int chin = 33;
int chout = 32;
bool ch1;
bool ar1;
bool ch3;
bool ar3;
bool pl3;
bool ch2;

void setup() {
   Serial.begin(115200);

   // Conexión a la red WiFi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println("Connecting to WiFi...");
   }
  //confiduracion de servo
   myservo.setPeriodHertz(50);
   myservo.attach(13, 500, 2400);
   // Configuración del cliente MQTT
   client.setServer(mqttServer, mqttPort);
   
   SPI.begin();
   rfid.PCD_Init();
   Serial.println();
   Serial.print(F("Reader :"));
   rfid.PCD_DumpVersionToSerial();
   
   // Configuración de la clave de autenticación para las tarjetas
   for (byte i = 0; i < 6; i++) {
       key.keyByte[i] = 0xFF;
   } 
   
   pinMode(chin, OUTPUT);
   pinMode(chout, OUTPUT);
   DatoHex = printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
   Serial.println();
   Serial.println();
   Serial.println("Acceso Patrullas Zona Restringida");
   Serial.println("");
}

void loop() {
   // Reconectar al servidor MQTT si la conexión se perdió
   if (!client.connected()) {
       reconnect();
   }
   
   // Verificar si hay una tarjeta nueva presente en el lector
   if (!rfid.PICC_IsNewCardPresent()) {
       return;
   }
   
   // Leer los datos de la tarjeta
   if (!rfid.PICC_ReadCardSerial()) {
       return;
   }
   
   // Comparar los datos de la tarjeta con los datos almacenados
   if (rfid.uid.uidByte[0] != nuidPICC[0] || rfid.uid.uidByte[1] != nuidPICC[1] || rfid.uid.uidByte[2] != nuidPICC[2] || rfid.uid.uidByte[3] != nuidPICC[3] )
     { 
       // Store NUID into nuidPICC array
       for (byte i = 0; i < 4; i++) {nuidPICC[i] = rfid.uid.uidByte[i];}
    
       DatoHex = printHex(rfid.uid.uidByte, rfid.uid.size);
       
       if(patrulla3 == DatoHex && ch3==false)
       {
        Serial.println("PATRULLA3 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch3 = true;     
       }
       else if(patrulla3 == DatoHex && ch3==true)
       {
        Serial.println("PATRULLA3 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch3 = false;     
       }
      else if(patrulla2 == DatoHex && ch2==false)
       {
        Serial.println("PATRULLA2 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch2 = true;     
       }
       else if(patrulla2 == DatoHex && ch2==true)
       {
        Serial.println("PATRULLA2 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch2 = false;     
       }
       else if(patrulla1 == DatoHex && ch1==false)
       {
        Serial.println("PATRULLA1 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch1 = true;     
       }
       else if(patrulla1 == DatoHex && ch1==true)
       {
        Serial.println("PATRULLA1 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch1 = false;     
       }
         else {
           // Acceso no permitido
           Serial.println("NO ESTA REGISTRADO - PROHIBIDO EL INGRESO");
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
           delay(100);
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
       }
       
   } else {
       Serial.println("TARJETA LEIDA PREVIAMENTE");
      if(patrulla3 == DatoHex && ch3==true)
       {
        Serial.println("PATRULLA3 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch3 = false;     
       }
       else if(patrulla3 == DatoHex && ch3==false)
       {
        Serial.println("PATRULLA3 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "Patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch3 = true;     
       }
       else if(patrulla2 == DatoHex && ch2==false)
       {
        Serial.println("PATRULLA2 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch2 = true;     
       }
       else if(patrulla2 == DatoHex && ch2==true)
       {
        Serial.println("PATRULLA2 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch2 = false;     
       }
       else if(patrulla1 == DatoHex && ch1==false)
       {
        Serial.println("PATRULLA1 - SALIDA CONCEDIDA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "patrullando";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch1 = true;     
       }
       else if(patrulla1 == DatoHex && ch1==true)
       {
        Serial.println("PATRULLA1 - ACCESO CONCEDIDO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        //se abre la pluma
        myservo.write(90);
        delay(3000);
        myservo.write(1);
        delay(100);
        //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
           client.publish(mqttTopic, buffer);
          //------------------------------------------------------------
          ch1 = false;     
       }
         else {
           // Acceso no permitido
           Serial.println("NO ESTA REGISTRADO - PROHIBIDO EL INGRESO");
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
           delay(100);
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
       }
       digitalWrite(chout, HIGH);
       delay(100);
       digitalWrite(chout, LOW);
       delay(100);
       digitalWrite(chout, HIGH);
       delay(100);
       digitalWrite(chout, LOW);
   }
    

   // Halt PICC
   rfid.PICC_HaltA();
   // Detener cifrado en PCD
   rfid.PCD_StopCrypto1();
   
   delay(1000);
}

// Función para convertir datos a formato hexadecimal
String printHex(byte *buffer, byte bufferSize) {
   String DatoHexAux = "";
   for (byte i = 0; i < bufferSize; i++) {
       if (buffer[i] < 0x10) {
           DatoHexAux += "0";
           DatoHexAux += String(buffer[i], HEX);
       } else {
           DatoHexAux += String(buffer[i], HEX);
       }
   }
   
   DatoHexAux.toUpperCase();
   return DatoHexAux;
}

// Función para reconectar al servidor MQTT
void reconnect() {
   while (!client.connected()) {
       //Serial.print("Attempting MQTT connection...");
       if (client.connect("ESP32Client")) {
           //Serial.println("connected");
       } else {
           Serial.print("failed, rc=");
           Serial.print(client.state());
           Serial.println(" try again in 5 seconds");
           delay(5000);
       }
   }
}