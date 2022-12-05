# Práctica 2 - Manual De Usuario

## INTEGRANTES
| Nombre                           | Carné                                      |
|:--------------------------------:|:------------------------------------------:|
| Daniel Reginaldo Dubón Rodríguez | 201901772                                  |
| Erick Ivan Mayorga Rodríguez     | 201901758                                 	|
| Edmy Marleny Mendoza Pol         | 201901212                                	|
| Kevin Steve Martínez Lemus       | 202004816                  				|

## Funciones de la Aplicación

- Setup - Mensaje Inicial
- Laberinto

## Setup - Mensaje Inicial

Al inciar el arduino, en la pantalla LCD se podrá observar el mensaje incial correspondiente a: "😊GRUPO 13 ACE1 😊SECC A", por aproximadamente 10 segundos, teniendo un moviento de izquierda a derecha en la linea superior de la pantalla. Al terminar el tiempo estipulado en el setup empezará el Laberinto.

## Laberinto

Una vez terminado el setup el dispositivo entrará al Laberinto, el mensaje de la pantalla cambiará, y el dispositivo permitirá al usuario ingresar a los siguientes modos:
- **Resolución Automática :** Pulsando el botón Left en el centro de control.
- **Modo Manual :** Pulsando el botón Right en el centro de control. 

### Resolución Automática

Al entrar en este modo, se mostrará un mensaje en la pantalla LCD durante 5 segundos que se está iniciando el modo de resolución automática. En este modo, el automóvil debe de ser colocado en cualquier parte del laberinto y a partir de este punto este será capaz de salir del laberinto; su único desplazamiento será hacia adelante y cuando este encuentre con un obstáculo girará 360 grados para realizar un escaneo y así saber cual es el camino correcto. Al finalizar el laberinto se mostrarán las estadísticas recolectadas durante la resolución.

### Modo Manual
Al entrar en este modo, se mostrará un mensaje en la pantalla LCD durante 5 segundos que se está iniciando el modo manual. En este modo el automóvil recibirá todas sus instrucciones desde el centro de control antes de empezar a moverse. Al terminar de ingresar las instrucciones el automóvil se moverá de acuerdo a estas; al detectar un obstáculo se pausará y únicamente podrá salir de esta pausa al presionar el botón Up. Al finalizar todos los movimientos guardados se mostrarán las estadísticas recolectadas durante la ejecución.

