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

#ifndef motorIR_Cabecera_h
#define motorIR_Cabecera_h

// Constantes
#define STANDBY 	999
#define INC_PWM 	25	// PWM step
#define v_max 	250
#define v_min	0

// Coeficientes para ajuste de la velocidad de los motores
#define coef_ML 	1
#define coef_MR		1

// #defines para mejorar la comprensión del código
#define LEFT 0
#define RIGHT 1
#define BOTH 2

// Pin de entrada de la señal infrarroja
#define p_IR	10		

// Pins motor izquierdo	
#define p_LM	11			
#define p_PWM_L	5
#define p_MI_L	3
#define p_MD_L	4

// Pins motor derecho
#define p_RM	12	
#define p_PWM_R	6
#define p_MI_R	7
#define p_MD_R	2

// Código del fabricante
#define VALID_REFERENCE 0xBF00 // 48896 ó 1011 1111 0000 0000

// Control simple de los motores
#define LEFT_ENGINE				0XF30C	// *
#define RIGHT_ENGINE			0XF10E	// #
#define CLOCKWISE 				0XEE11	// up
#define COUNTER_CLOCKWISE		0XE619	// down
#define FORWARD					0XE916	// right
#define BACKWARD				0XEB14	// left

// Control avanzado de los motores
#define EMERGENCY_STOP			0XF20D	// 0
#define	EMERGENCY_FORWARD		0XFF00	// 1
#define	EMERGENCY_BACKWARD		0XFE01	// 2
#define SYNCHRONIZATION			0XFD02	// 3
#define	TURN_CLOCKWISE			0XFB04	// 4
#define	TURN_COUNTER_CLOCKWISE	0XFA05	// 5

#endif
