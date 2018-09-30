/* Author: 		Eduardo Cáceres de la Calle
 * Subject: 	Sistemas Digitales Avanzados (SDA)
 * Degree: 		Industrial Electronics and Automatic Control Engineering
 * University:	Universidad de Valladolid (UVa) - EII
 *
 * Code written in 2016, during my first contact with Arduino.
 * Uploaded for educational purposes only, don't be too hard on me :)
 * 
 */

/*
 *  Ejemplo que ilustra el uso de la librería de creación propia motorIR.
 *  Permite controlar de manera indefinida dos motores DC a través de un
 *  mando de infrarrojos.
 *  1/06/16
*/

#include <motorIR.h>
motorIR car;

void setup()
{
  Serial.begin(9600);
  car.begin();
}

void loop()
{
    car.run();
}
