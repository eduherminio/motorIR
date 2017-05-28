/* Author: 		Eduardo Cáceres de la Calle
 * Subject: 	Sistemas Digitales Avanzados (SDA)
 * Degree: 		Industrial Electronics and Automatic Control Engineering
 * University:	Universidad de Valladolid (UVa) - EII
 *
 * Code written in 2016, during my first contact with Arduino.
 * Uploaded for educational purposes only, don't be too hard on me :)
 * 
 */

/*  Librería que permite controlar dos motores de corriente continua a 
 *	través de infrarrojos.
 * 	Usa, a su vez, la librería pública NECIRrcv para la captación de 
 *	las señales infrarrojas. 
 *  1/06/16
*/

#ifndef motorIR_h
#define motorIR_h

#include "Arduino.h"
#include "NECIRrcv.h"
#include "motorIR_Cabecera.h"

class motorIR
{
public:     // Funciones públicas
	
	motorIR();		// Constructor
	/*	Define los pines digitales de salida que serán usando,
	*	tomando para ello los programados por defecto para tal fin.
	*/
	
	void begin();	//	Alto nivel
	/*	Activa el monitor serie, inicializando su velocidad.
	*	Activa el objeto de la clase NECIRrcv que nos permitirá
	*	acceder a las siguientes funciones de esa librería:
	*	.read() y .available();
	*/

	void run();		// Alto nivel
	/*	Función de alto nivel (la que se invoca desde Arduino):
	*	1. 	Comprueba que si se ha recibido una señal infrarroja
	*		mediante la función available() de NECIRrcv.
	*	2. 	Lee ese código infrarrojo mediante read(), también
	*		de NECIRrcv.
	*	3.	Comprueba si es un código válido (enviado por un mando
			válido mediante la llamada a validation()
	*	4. 	Llama a la función selecFunction(), que contrastará las
	*		instrucciones recibidas con los códigos de actuación
	*		programados.
	*/
	
	int validation();
	/*	Comprueba mediante una máscara si el código del fabricante del
	*	mando (las 4 últimas cifras del número hexadecimal) coincide
	*	con el programado.
	*	En caso afirmativo, descarta los bits en los que está
	*	la información información anterior y devuelve 1.
	*	En caso contrario, muestra por pantalla el error y devuelve 0.
	*/
	
	void selectFunction();
	/*	1. Comprueba que hay un motor seleccionado (o, en su defecto,
	*		si la orden recibida selecciona algún motor).
	*	2. Muestra por el monitor el código de la orden recibida
	*	3. Comprueba si alguno de los códigos programados coincide
	*		con el recibido.
	*	4.1 Realiza las acciones pertinentes (normalmente llamadas a
	*		otras funciones) para llevar a cabo la orden.
	*	4.2	En caso de que el botón no esté programado, lo indica
	*		por el monitor serie.
	*	5	Llamada a Motor_on() para actualizar el estado de los
	*		motores tras hacer cumplir la orden recibida por IR.
	*/

	void setActiveMotor(int PWM, int MI, int MD, int pos);
	/*	Modifica las variables mienmbro de la clase correspondientes
	*	a los pines activos de PWM, MI y MD y selección del motor,
	*	dándoles los valores de los argumentos que recibe.
	*/	
	
	void selectMotor(int mi, int md);
	/*	Selecciona el motor correspondiente según las
	*	combinaciones mi-md que reciba.
	*	En caso de que ambos estén activados, _PWM[1]=_PWM[2];
	*	se llama a synchro() para que, sea cual sea la situación
	*	previa, se mantenga la velocidad y se actualice
	*	correctamente el motor seleccionado.		
	*/
	
	void synchro(int PWM, int MI, int MD);
	/*	Actualiza los vectores-variables miembro correspondientes
	*	a PWM, MI y MD para ambos motores, según los parámetros que
	*	se envíen
	*/
	
	void set_PWM(int k);
	/*	Aumenta o disminuye el valor del PWM del/los motores activos
	*	en k unidades. Se vale de set_PWM_ para ello.
	*/
	
	void set_PWM_(int k, int DIR);
	/*	Aumenta o disminuye en k unidades el valor del PWM de un motor,
	*	recibiendo índice del vector de PWM a modificar y el valor k.
	*/
	
	void turn(int MI, int MD);
	/*	Llama a synchro() de manera que inicialice a 0 _MI[] y _MD[],
	*	 y a v_max _PWM[].
	*	Pone a 1 los vectores de las posiciones MI y MD (recibidas como
	*	argumentos) de los vectores _MI y _MD respectivamente.
	*	Esto permite que ambos motores giren en sentido opuesto,
	*	desplazándose el conjunto a un lado y otro.
	*/

	void Motor_on();
	/*	Función encargada de enviar toda la información a los motores
	*	(MI, MD y PWM) y a los LEDs de señalización
	*/
	
	
	private:    	// Variables privadas
	
	unsigned long _IRcode;		// Código IR devuelto por NECIRrcv
	
    int _pin_PWM;		// Pines activos en cada momento
    int _pin_MI;
    int _pin_MD;
	
	int _pin_LED;	// Pin del LED que muestra el motor activo
					// alterna entre p_LM y p_RM
	
	int active_m= STANDBY;	// Número que indica el motor activo
					
	int _PWM[2]={0};	// Vectores que almacenan los parámetros 	
	int _MI[2]={0};		// de los motores en tiempo de ejecución
	int _MD[2]={0};
	
};

#endif
