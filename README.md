# Check in y Check out de objetos en unidades móviles. 

## Introducción

El presente documento tiene como objetivo presentar una propuesta para el desarrollo de un prototipo de prospecto de cliente que aborda una problemática crucial en las industrias de seguridad, policiaca y militar. El prototipo propuesto busca integrar tecnologías de identificación avanzadas, específicamente Radio Frecuencia de Identificación (RFID) y Visión por Computadora, para proporcionar una solución integral que agilice y fortalezca el control de accesos en áreas restringidas.

## Problemática

En entornos críticos como instalaciones de seguridad, policiacas y militares, el acceso no autorizado y la detección de individuos o equipos no autorizados son problemas persistentes. Los métodos tradicionales de identificación y control de acceso a menudo resultan ineficientes y pueden llevar a situaciones de riesgo. La falta de una identificación segura y rápida de personas y objetos autorizados puede comprometer la seguridad en estas instalaciones. También se necesita responsabilizar a cada oficial sobre el cuidado de equipo que se les proporciona de manera gratuita.

## Solución Propuesta

El prototipo de prospecto de cliente propuesto aborda esta problemática mediante la integración de dos tecnologías clave: RFID y Visión por Computadora(FaceID).

### Radio Frecuencia de Identificación (RFID)

El RFID permite la identificación sin contacto mediante el uso de etiquetas o tarjetas equipadas con chips RFID. Estos dispositivos emiten señales de radio que son captadas por lectores RFID, permitiendo una identificación rápida y eficiente. La implementación de RFID en el prototipo de cliente permitiría:

- Acceso seguro y rápido a áreas restringidas: El personal autorizado puede acceder a zonas sensibles sin demoras innecesarias.
- Control preciso de activos: Equipos de seguridad y vehículos autorizados pueden ser rastreados y gestionados de manera efectiva.

### Visión por Computadora

La visión por computadora se refiere a la capacidad de las máquinas para interpretar y comprender el entorno visual mediante el análisis de imágenes o videos. En este prototipo, la visión por computadora se utiliza para el reconocimiento facial y eficiencia en un registro de asistencia. Esto proporciona:

- Identificación precisa de individuos: La tecnología FaceID permite una identificación biométrica confiable.
- Detección de comportamientos anómalos: El sistema puede alertar sobre acciones sospechosas, como personas no autorizadas.

## Objetivos Específicos
1. Integrar con éxito sistemas de lectura y escritura RFID en el prototipo para permitir la autenticación y el acceso rápido de individuos, vehículos y equipos autorizados.
2. Evaluar la escalabilidad del prototipo para adaptarse a diferentes tamaños de instalaciones y niveles de tráfico de usuarios.
3. Cumplir con los requisitos de seguridad y privacidad establecidos por las regulaciones pertinentes y garantizar la protección de datos personales.
4. Aumentar la eficiencia en la burocracia de registro de entradas y salidas, dándoles más tiempo a los oficiales de poder hacer su trabajo eficazmente.

## Materiales necesarios
- ESP32 DEVKIT V1
- ESP32 CAM
- Módulo sensor RFID MFRC522 RF IC (con tarjeta RFID)
- Display LCD 16x2; I2C para display 
- Raspberry Pi 4 mod B 8gb
- Cargador USB para Raspberry pi
- Pantalla touch 10.1" 
- Servomotor SG90
- Micro push button 2 pines
- Relevador de 5V 
- Leds rojo y verde.

## Servicios

1. **Registro de Asistencia:** Se tomará una foto a la entrada y salida del turno de patrullaje, guardando el registro de horas y también dando acceso a las llaves de la patrulla. 
2. **Chequeo de Seguridad y Control:** Se hace un chequeo de todo el equipo de protección (chaleco antibalas, arma de fuego y placa-emblema) para arrancar la unidad, y también poder registrar salida de turno.
3. **Sensor de tráfico:** Un sensor que lleva un recuento de qué unidades siguen en las instalaciones y cuáles andan en su ruta de patrullaje.

## Resultados esperados
De ser implementado el proyecto, vemos posible reducir el tiempo necesario para la identificación y el acceso autori-zado en las instalaciones en al menos un 15%, mejorando la eficiencia operativa.
Contribuir a una reducción del 2% en la resolución de delitos e incidentes de seguridad dentro de las áreas de prueba piloto.

## Conclusiones

El prototipo presenta una solución innovadora para abordar problemáticas cruciales en las industrias de seguridad, policiaca y militar. Esta combinación de tecnologías permitirá una identificación segura y rápida de individuos, vehículos y equipos autorizados. En conjunto, este prototipo fortalecerá la seguridad y el control en entornos críticos, brindando una herramienta esencial para operaciones policiacas y militares efectivas.
