#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// Definiciones de pines y constantes
#define SS_PIN 5
#define RST_PIN 22

// Configuración de WiFi
const char* ssid = "IZZI-DA6C";
const char* password = "AZ3mtcErcpp6NmfeaZ";

// Configuración de MQTT
const char* mqttServer = "broker.hivemq.com";              //Broker publico
const int mqttPort = 1883;
const char* mqttTopic = "Proyecto/mqtt/DatosControl";  

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Definiciones de objetos y variables globales
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
//--------------------------------------------------------------------------
//SE ASIGNAN LOS DATOS HEXADECIMALES A LAS CONSTANTES EN CADENAS DE CARACTER
String DatoHex;
const String chaleco3 = "C3BC8193";     
const String arma3 = "832E5C93";
const String placa3 = "D35BA293";
const String chaleco1 = "F3C4E510";
const String arma1 = "A3035895";
//--------------------------------------------------------------------------
int chin = 33;
int chout = 32;
bool mq1;
bool ch1;
bool ar1;
bool mq3;
bool ch3;
bool ar3;
bool pl3;

void setup() {
   Serial.begin(115200);

   // Conexión a la red WiFi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println("Connecting to WiFi...");
   }

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
   Serial.println("check in y check out");
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
       
       if(chaleco3 == DatoHex)
       {
        Serial.println("CHALECO OFICIAL3 - ESCANEADO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        digitalWrite(chin,LOW);
        ch3 = true;     
       }
       else if(chaleco1 == DatoHex)
       {
        Serial.println("CHALECO OFICIAL1 - ESCANEADO");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        digitalWrite(chin, LOW);
        ch1 = true;
       }
       else if(placa3 == DatoHex)
       {
        Serial.println("PLACA EMBLEMA OFICIAL3 - ESCANEADA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        pl3 =true;
       }
       else if(arma1 == DatoHex)
       {
        Serial.println("ARMA DE FUEGO OFICIAL1 - ESCANEADA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        ar1 = true;
       }
       else if(arma3 == DatoHex)
       {
        Serial.println("ARMA DE FUEGO OFICIAL3 - ESCANEADA");
        digitalWrite(chout,HIGH);
        delay(500);
        digitalWrite(chout,LOW);
        ar3 = true;
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
       }else {
       Serial.println("TARJETA LEIDA PREVIAMENTE");
       digitalWrite(chout, HIGH);
       delay(100);
       digitalWrite(chout, LOW);
       delay(100);
       digitalWrite(chout, HIGH);
       delay(100);
       digitalWrite(chout, LOW);
   }
   if(ch3==true && ar3==true && pl3==true && mq3==false){           //CONDICIONAL DE LECTURA DE LOS TRES ELEMENTOS POR OFICIAL
         //--------------------------------------------------------------
         //envio de Json por mqtt (check in)
         StaticJsonDocument<200> doc;
           doc["nombre"] = "Andres Michel Rodriguez Gordillo";
           doc["check_in"] = "1";
           doc["check_out"] = "0";
           doc["patrulla"] = "3";
           char buffer[200];
           serializeJson(doc, buffer);
            //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
          Serial.println("OFICIAL3 CHECK IN COMPLETADO");
          Serial.println("OFICIAL3 EN SERVICIO");
          digitalWrite(chin,HIGH);
           mq3 = true;
           ch3 = false;
           ar3 = false;
           pl3 = false;
          }
           else{
                digitalWrite(chout, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                digitalWrite(chout,LOW);
               }
      } 
      else if(mq3==true && ch3==true && ar3==true && pl3==true){
         //--------------------------------------------------------------
         //envio de Json por mqtt (check in)
         StaticJsonDocument<200> doc;
           doc["nombre"] = "Andres Michel Rodriguez Gordillo";
           doc["check_in"] = "0";
           doc["check_out"] = "1";
           doc["patrulla"] = "3";
           char buffer[200];
           serializeJson(doc, buffer);
            //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
          Serial.println("OFICIAL3 CHECK OUT COMPLETADO");
          Serial.println("OFICIAL3 FUERA DE SERVICIO");
          digitalWrite(chin,HIGH);
           mq3 = false;
           ch3 = false;
           ar3 = false;
           pl3 = false;
           }
           else{
             Serial.println("ERROR REINTENTE DE NUEVO");
                digitalWrite(chout, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                digitalWrite(chout,LOW);
                } 
        }
          //------------------------------------------------------------
         
      else if (ch1==true && ar1 ==true && mq1==false){
        //envio de Json por mqtt
         StaticJsonDocument<200> doc;
           doc["nombre"] = "Carlos Andres Rivera Villamil";
           doc["check_in"] = "1";
           doc["check_out"] = "0";
           doc["patrulla"] = "1";
           char buffer[200];
           serializeJson(doc, buffer);
        //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
        Serial.println("OFICIAL1 CHAECK IN COMPLETADO");
        Serial.println("OFICIAL1 EN SERVICIO");
        digitalWrite(chin,HIGH);
           mq1 = true;
           ch1 = false;
           ar1 = false;

          } 
          else{
             Serial.println("ERROR REINTENTE DE NUEVO");
                digitalWrite(chout, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                digitalWrite(chout,LOW);
                } 
        }
          else if(mq1==true && ch1==true && ar1==true){
          // envio de Jsaon por mqtt (check out)
             StaticJsonDocument<200> doc;
           doc["nombre"] = "Carlos Andres Rivera Villamil";
           doc["check_in"] = "0";
           doc["check_out"] = "1";
           doc["patrulla"] = "1";
           char buffer[200];
           serializeJson(doc, buffer);
           //------------------------------------------------------------
          if (client.publish(mqttTopic, buffer, 1)) {
           digitalWrite(chin,HIGH); 
           mq1 = false;
           ch1 = false;
           ar1 = false;
           }
           else{
             Serial.println("ERROR REINTENTE DE NUEVO");
                digitalWrite(chout, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                delay(100);
                digitalWrite(chin, HIGH);
                delay(100);
                digitalWrite(chin, LOW);
                digitalWrite(chout,LOW);
           }
          }//------------------------------------------------------------

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
       Serial.print("Attempting MQTT connection...");
       if (client.connect("ESP32Client")) {
           Serial.println("connected");
       } else {
           Serial.print("failed, rc=");
           Serial.print(client.state());
           Serial.println(" try again in 5 seconds");
           delay(5000);
       }
   }
}