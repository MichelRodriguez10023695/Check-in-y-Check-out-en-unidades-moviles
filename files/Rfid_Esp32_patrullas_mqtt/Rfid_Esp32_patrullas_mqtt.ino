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
const char* mqttServer = "broker.hivemq.com";      //broker publico
//const char* mqttServer = "ip_local";             //ip local 
const int mqttPort = 1883;
const char* mqttTopic = "Proyecto/mqtt/PatrullasControl";

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
   ch3 = false;
   ch2 = false;
   ch1 = false;
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
       // se concatenan los datos del lector a un vector para generar el numero hexadecimal
       for (byte i = 0; i < 4; i++) {nuidPICC[i] = rfid.uid.uidByte[i];}
       
       DatoHex = printHex(rfid.uid.uidByte, rfid.uid.size);     //se define la variable del vector leido

       //---------------------------------------------------LECTURA DE PATRULLA1---------------------------

       if(patrulla3 == DatoHex && ch3==false)                  // se define la condicional de entrada
       {
        Serial.println("PATRULLA3 - SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);
         }
       }
       else if(patrulla3 == DatoHex && ch3==true)          //condicional de dato de entrada
       {
        Serial.println("PATRULLA3 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = false; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);
           }    
       }//----------------------------------------------------LECTURA DE PATRULLA2--------------
      else if(patrulla2 == DatoHex && ch2==false)
       {
        Serial.println("PATRULLA2 - SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);
       }
      }
       else if(patrulla2 == DatoHex && ch2==true) 
       {
        Serial.println("PATRULLA2 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = false; 
          }  
          else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);
       }
      } //-------------------------------------------LECTURA DE PATRULLA1---------------------------------------
       else if(patrulla1 == DatoHex && ch1==false)
       {
        Serial.println("PATRULLA1 - SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch1 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);
       }
      }
       else if(patrulla1 == DatoHex && ch1==true)
       {
        Serial.println("PATRULLA1 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "Disponible";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch1 = false; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
       }
         else {
           //-----------------------------------------------------------Acceso no permitido
           Serial.println("NO ESTA REGISTRADO - PROHIBIDO EL INGRESO");
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
           delay(100);
           digitalWrite(chin, HIGH);
           delay(100);
           digitalWrite(chin, LOW);
       }
       // si al tarjeta se escanea consecuitivamente-----------
   } else {
       Serial.println("TARJETA LEIDA PREVIAMENTE");
      if(patrulla3 == DatoHex && ch3==true)
       {
        Serial.println("PATRULLA3 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "DISPONIBLE";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = false; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
      }
       else if(patrulla3 == DatoHex && ch3==false)
       {
        Serial.println("PATRULLA3 - SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "3";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch3 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
      }
       else if(patrulla2 == DatoHex && ch2==false)
       {
        Serial.println("PATRULLA2 -SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch2 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
       }
       else if(patrulla2 == DatoHex && ch2==true)
       {
        Serial.println("PATRULLA2 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "2";
           doc["estado"] = "DISPONIBLE";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch2 = false; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
       }
       else if(patrulla1 == DatoHex && ch1==false)
       {
        Serial.println("PATRULLA1 - SALIDA CONCEDIDA");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "PATRULLANDO";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch1 = true; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
       }
       else if(patrulla1 == DatoHex && ch1==true)
       {
        Serial.println("PATRULLA1 - ACCESO CONCEDIDO");    //se imprime el dato en el monitor serial
        digitalWrite(chout,HIGH);                          //se enciende el indicador LED
        delay(500);
        digitalWrite(chout,LOW);
       //--------------------------------------------------------------
         //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["patrulla"] = "1";
           doc["estado"] = "DISPONIBLE";
           char buffer[200];
           serializeJson(doc, buffer);
          //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           Serial.println("JSON enviado exitosamente por MQTT con confirmación");
          //se abre la pluma
          myservo.write(90);
          delay(3000);
          myservo.write(1);
          delay(100);
          ch1 = false; 
          } else {
          Serial.println("Error al enviar el JSON por MQTT");
          digitalWrite(chin,HIGH);                          //se enciende el indicador LED
          delay(1000);
          digitalWrite(chin,LOW);     
       }
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