import datetime
import requests
import os
import json
import face_recognition
import pandas as pd
from paho.mqtt import client as mqtt_client
import random
import time
import argparse

#fecha y hora
fechayhora = datetime.datetime.now()
fecha_hora = fechayhora.strftime("%d-%m-%Y %H:%M")


# comexion al broquer
broker = '127.0.0.1'
port = 1883
topic = "PaseDeLista/mqtt/entrada"
topic2 = "PaseDeLista/mqtt/index"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'


# Conexion al broker
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client, mensaje):
    #while True:
    time.sleep(1)
    msg = mensaje
    result = client.publish(topic, msg)
    time.sleep(1)
    print (result)
    # result: [0, 1]
    status = result[0]
    if status == 0:
        print(f"Send {msg} to topic {topic}")
    else:
        print(f"Failed to send message to topic {topic}")


# Ruta de la carpeta con las imágenes de referencia
base_paths = ['/home/andres/RecFacial/Imagenes/Oficial1/','/home/andres/RecFacial/Imagenes/Oficial2/','/home/andres/RecFacial/Imagenes/Oficial3/']  # Agrega las rutas de las carpetas

# Dirección IP de la ESP32-CAM (cambia según tu configuración)
esp32_ip = "http://192.168.0.119"

# Captura de una imagen desde la ESP32-CAM
image_url = f"{esp32_ip}/capture"

response = requests.get(image_url)
if response.status_code == 200:
    with open("captured_image.jpg", "wb") as f:
        f.write(response.content)
else:
    print("Error al capturar la imagen desde la ESP32-CAM")
    exit()

# Cargar la imagen capturada
captured_image = face_recognition.load_image_file("captured_image.jpg")
captured_face_encoding = face_recognition.face_encodings(captured_image)[0]
print("imagen capturada")#

# Variables para realizar la comparación
best_match_path = None
best_match_distance = float('inf')

# Recorrer las carpetas con imágenes de referencia
for base_path in base_paths:
    for filename in os.listdir(base_path):
        if filename.endswith(".jpg"):
            reference_image = face_recognition.load_image_file(os.path.join(base_path, filename))
            reference_face_encoding = face_recognition.face_encodings(reference_image)[0]
            # Calcular la distancia entre las codificaciones faciales
            distance = face_recognition.face_distance([reference_face_encoding], captured_face_encoding)[0]

            if distance < best_match_distance:
                best_match_distance = distance
                best_match_path = base_path

if best_match_distance <0.50:
# Imprimir el contenido del archivo JSON en la carpeta con la mejor coincidencia
    json_path = os.path.join(best_match_path, "arribo.json")
    json_view = json.dumps(best_match_path)
    
    if os.path.exists(json_path):
        with open(json_path, "r") as json_file:
            data = json.load(json_file)
            data["FECHA_HORA"] = fecha_hora

        with open(json_path, "w") as json_file:
            json.dump(data,json_file,indent=4)
            print(json.dumps(data, indent=4))

else:
    print("No se encontró concidencia. Usted no esta autorizado")

print("Proceso completado.")
#envio
client = connect_mqtt()
client.loop_start()
publish(client, json_view)
