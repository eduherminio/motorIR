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

#include "Arduino.h"
#include "motorIR.h"
#include "motorIR_Cabecera.h"
#include <string.h>
  
NECIRrcv sensor(p_IR);		// Da acceso a .available() .read()

motorIR::motorIR()
{
	Serial.println("Default paremeters");
	pinMode(p_MI_L, OUTPUT);
	pinMode(p_MD_L, OUTPUT);
	pinMode(p_MI_R, OUTPUT);
	pinMode(p_MD_R, OUTPUT);
	pinMode(p_LM, OUTPUT);
	pinMode(p_RM, OUTPUT);
	
	//pinMode(p_PWM_R, OUTPUT);		// Pines analógicos usados como PWM
	//pinMode(p_PWM_L, OUTPUT);
}


void motorIR::begin()
{
	Serial.begin(9600);
	sensor.begin();
}

void motorIR::run()
{
	 while (sensor.available())
	 {
		_IRcode= sensor.read();
		
		if(!validation())	// Comprueba que el mando pertenece
			break;			// fabricante
			
		selectFunction();	// Se encarga de las órdenes programadas
	 }
}

int motorIR::validation()
{
	if( (_IRcode & 0xFFFF) == VALID_REFERENCE)
	{
		_IRcode=_IRcode>>16;		// Elimina el identificador
		return (1);					// del fabricante
	}
	
	Serial.println("Mando no original");

	return (0);
}

void motorIR::selectFunction()
{	
	if ( (active_m != STANDBY) || ( (_IRcode != CLOCKWISE) && (_IRcode != COUNTER_CLOCKWISE ) && (_IRcode != FORWARD) && (_IRcode != BACKWARD) ))
	{
		Serial.print("Codigo instruccion:\t");
		Serial.print(_IRcode, HEX);
		Serial.print('\n');

		switch(_IRcode)
		{
			case LEFT_ENGINE:	// Selección motor izquierdo
				setActiveMotor(p_PWM_L, p_MI_L, p_MD_L, p_LM);
				active_m=LEFT;
				break;
				
			case RIGHT_ENGINE:	// Selección motor derecho
				setActiveMotor(p_PWM_R, p_MI_R, p_MD_R, p_RM);
				active_m= RIGHT;	
				break;
				
			case CLOCKWISE:		// Sentido giro agujas del reloj
				selectMotor(0, 1);
				break;
			
			case COUNTER_CLOCKWISE:	// Sentido giro opuesto a agujas
				selectMotor(1, 0);	// del reloj
				break;
				
			case FORWARD:			// Adelante
				set_PWM(+INC_PWM);
				 break;       
				 
			case BACKWARD:			// Atrás
				set_PWM(-INC_PWM);
				break;

			case EMERGENCY_STOP:	// Frenado de emergencia
				active_m= STANDBY;
				synchro(v_min, 0, 0);
				break;	
				
			case EMERGENCY_FORWARD:	// Avance de emergencia
				active_m= BOTH;
				synchro(v_max, 0, 1);
				break;
				
			case EMERGENCY_BACKWARD:	// Retroceso de emergencia
				active_m= BOTH;
				synchro(v_max, 1, 0);			
				break;
			
			case SYNCHRONIZATION:		// Control síncrono de motores
				active_m= BOTH;
				synchro(v_min, 0, 0);	
				break;
			
			case TURN_CLOCKWISE:			// Rotación infinita - der.
				turn(1, 0);
				break;
				
			case TURN_COUNTER_CLOCKWISE:	// Rotación infinita - izq.
				turn(0, 1);
				break;

			default:
				Serial.println("Funcion no programada");
				break;
		}
		
		Motor_on();		// Actuador
	}
	else
		Serial.println("Seleccione primero un motor");		
}

void motorIR::setActiveMotor(int PWM, int MI, int MD, int pos)
{
	_pin_PWM= PWM;
	_pin_MI= MI;
	_pin_MD= MD;
	_pin_LED= pos;
}

void motorIR::selectMotor(int mi, int md)
{
	if(active_m!=BOTH)
	{
		_MI[active_m]= mi;
		_MD[active_m]= md;					
	}
	
	else
		synchro(_PWM[mi], mi, md);
	// _PWM[1]=_PWM[2], mantiene esa velocidad, y actualiza mi y md
}

void motorIR::synchro(int PWM, int MI, int MD)
{
	for(int i=0;i<2;i++)
	{
		_PWM[i]= PWM;
		_MI[i]=	MI;
		_MD[i]= MD;
	}
}

void motorIR::set_PWM(int k)
{
	if(active_m!=BOTH)
		set_PWM_(k, active_m);		
	
	else
	{
		set_PWM_(k, LEFT);
		set_PWM_(k, RIGHT);
	}
}

void motorIR::set_PWM_(int k, int DIR)
{
	if(_PWM[DIR]+k > v_max)
		_PWM[DIR]= v_max;
	
	else if (_PWM[DIR]+k < 0)
		_PWM[DIR]= 0;
	
	else
		_PWM[DIR]+= k;
}

void motorIR::turn(int MI, int MD)
{
	active_m=BOTH;
	synchro(v_max, 0, 0);
	_MI[MI]= 1;
	_MD[MD]= 1;
}


void motorIR::Motor_on()
{
	digitalWrite(p_LM, LOW);
	digitalWrite(p_RM, LOW);
	
	if( (active_m == BOTH) || (active_m == STANDBY) )
	{
		digitalWrite(p_MI_L, _MI[LEFT]);
		digitalWrite(p_MD_L, _MD[LEFT]);
		analogWrite(p_PWM_L, _PWM[LEFT]*coef_ML);
		
		digitalWrite(p_MI_R, _MI[RIGHT]);
		digitalWrite(p_MD_R, _MD[RIGHT]);
		analogWrite(p_PWM_R, _PWM[RIGHT]*coef_MR);
		
		if(active_m == BOTH)
		{
			digitalWrite(p_LM, HIGH);
			digitalWrite(p_RM, HIGH);
		}
	}
	
	else
	{
		digitalWrite(_pin_MI, _MI[active_m]);
		digitalWrite(_pin_MD, _MD[active_m]);
		
		if(active_m==LEFT)
			analogWrite(_pin_PWM, _PWM[active_m]*coef_ML);
		else
			analogWrite(_pin_PWM, _PWM[active_m]*coef_MR);
		
		digitalWrite(_pin_LED, HIGH);
	}
}
