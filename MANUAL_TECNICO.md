# Práctica 2 - Manual Técnico

## INTEGRANTES
| Nombre                           | Carné                                      |
|:--------------------------------:|:------------------------------------------:|
| Daniel Reginaldo Dubón Rodríguez | 201901772                                  |
| Erick Ivan Mayorga Rodríguez     | 201901758                                 	|
| Edmy Marleny Mendoza Pol         | 201901212                                	|
| Kevin Steve Martínez Lemus       | 202004816                  				|

## INTRODUCCIÓN

Se realizó un dispositivo tipo automóvil capaz de encontrar una ruta para resolver un laberinto, detectando obstáculos, obteniendo estadísticas de los movimientos y presentando diversos estados como mensajes en una pantalla LCD, este cuenta con un centro de control conformado por 4 botones, y se utilizó una red de Arduinos usando I2C uno siendo maestro y otro esclavo.

## COMPONENTES UTILIZADOS
- 2 Arduinos Mega <br>
<img src="./Imagenes/arduinoMega.png" alt="drawing" style="width:100px;"/> <br>
- 1 Sensor Ultrasónico <br>
<img src="./Imagenes/sensorUltrasonico.png" alt="drawing" style="width:100px;"/> <br>
- 1 Sensor De Color <br>
<img src="./Imagenes/sensorColor.png" alt="drawing" style="width:20px;"/> <br>
- 4 Botones <br>
<img src="./Imagenes/pulsador.jpg" alt="drawing" style="width:80px;"/> <br>
- 1 Pantalla LCD <br>
<img src="./Imagenes/lcd.png" alt="drawing" style="width:20px;"/> <br>
- 2 Motores <br>
<img src="./Imagenes/motor.png" alt="drawing" style="width:100px;"/> <br>
- Puente H <br>
<img src="./Imagenes/puenteh.png" alt="drawing" style="width:100px;"/> <br>
- PowerBank <br>
<img src="./Imagenes/powebank.png" alt="drawing" style="width:15px;"/> <br>

## RED DE ARDUINOS
Para la desarrollar la red de arduinos se utilizo el protocolo I2C.

<img src="./Imagenes/i2c.png" alt="drawing" style="width:100px;"/>

Se utilizaron los arduinos de la siguiente manera:

- **Maestro :** El dispositivo maestro se encarga de:
    - Controlar la pantalla LCD.
	- Recabar la información recibida.
	- Manejar la lista de moviemientos del modo manual
	- Calcular estadísticas
	- Control de botones
	- Contadores

- **Esclavo :** El dispositivo esclavo manejará:
	- Los movimientos del automóvil.
	- La detección de obstáculos
	- La detección de colores
	
## CODIGO UTILIZADO
### Maestro
Las variables globales nos permitirán definir las entradas, la salidas y manejar los datos correspondientes a los estados del mismo. <br>

<img src="./Imagenes/globalesM.png" alt="drawing" style="width:100px;"/>

Se definieron los caracteres especiales que se mostrarán en la pantalla LCD. <br>

<img src="./Imagenes/caracteres.png" alt="drawing" style="width:100px;"/>

En la sección de declaración de pines se inicia la comunicación entre los dos arduinos con el comando `Wire.begin();`

<img src="./Imagenes/pinesM.png" alt="drawing" style="width:100px;"/>

Con el método `mensajeInicial` se muestra el mensaje de inicio con un desplazamiento en la
línea superior de izquierda a derecha de la pantalla LCD.

<img src="./Imagenes/mensajeInicial.png" alt="drawing" style="width:100px;"/>

Se utilizó la variable `estado` para saber que debe de ejecutar el arduino maestro, ya sea mostrar los diferentes mensajes en la pantalla o entrar a los diferentes modos de Laberinto.

<img src="./Imagenes/estadoM.png" alt="drawing" style="width:100px;"/>

Para la recolección de intrucciones dentro del modo manual se utilizo el siguiente código.

<img src="./Imagenes/instrucciones.png" alt="drawing" style="width:100px;"/>


### Esclavo
Las variables globales nos permitirán definir las entradas, la salidas y manejar los datos correspondientes a los estados del mismo.

<img src="./Imagenes/globalesE.png" alt="drawing" style="width:100px;"/>

En la sección de declaración de pines se inicia la comunicación entre los dos arduinos con el comando `Wire.begin();` estableciendo como canal 0x01

<img src="./Imagenes/pinesE.png" alt="drawing" style="width:100px;"/>

Se utilizó la función `ComandoDeEntrada` para recibir el comando del arduino maestro y la función `peticion` para realizar una solicitud al arduino maestro.

<img src="./Imagenes/request.png" alt="drawing" style="width:100px;"/>

Para la parte de movimiento del automóvil se utilizo las funciones `goLeft` y `goRight` para realizar los giros y las funciones `goForward` y `goBackward` para avanzar y retroceder respectivamente.

<img src="./Imagenes/movs.png" alt="drawing" style="width:100px;"/>

El manejo del sensor ultrasonico esta estipulado en la funcion `getDistancia` que permite generar y recibir pulsos del mismo, para poder asi medir la distancia entre el automóvil y el obstacúlo.

<img src="./Imagenes/distance.png" alt="drawing" style="width:100px;"/>

Para controlar el sensor de color se utilizo el siguiente codigo, el cual permite establecer fotodiodos con su respectivo filtro y leer el color mediante `pulseIn()`.

<img src="./Imagenes/color.png" alt="drawing" style="width:100px;"/>

De la misma manera que el arduino maestro, se utiliza una variable `estado`, que nos permite saber que se debe de ejecutar.

<img src="./Imagenes/estadoE.png" alt="drawing" style="width:100px;"/>

La función `modoMecanico()` ejecuta el modo automático de resolución del Laberinto, teniendo el siguiente codigo.

<img src="./Imagenes/mecanico.png" alt="drawing" style="width:100px;"/>
