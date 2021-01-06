#include <Arduino.h>
#define AMR_Pin 2
#define Pressure_Transmitter_Pin A0
#define bounce_time 1000
#define ON_pin 4
#define OFF_pin 5
#define Valve_pin 13

unsigned long counter = 0;
unsigned long tic = millis();
unsigned long toc = 0;
unsigned int time = 0;

// float pressure_avg(float count)
// {
//   float analog_val = 0;
//  for(int i = 0; i<count; i++)
//  {
//   analog_val += analogRead(A0);
//  }
//  float avg_analog_val = analog_val/count;
//  return avg_analog_val;
// }


void Counter()
{
  static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    toc = interrupt_time;

    if(interrupt_time - last_interrupt_time > bounce_time)
    {
        counter++;
        time = int((toc - tic)/1000);
        tic = millis();
    }

    last_interrupt_time = interrupt_time;
}

void Reset_Counter_and_Time()
{
  counter = 0;
  time = 0;
}

void Print_Counter_Value()
{
  Serial.println(counter);
}

void Print_Time()
{
  Serial.println(time);
}

double Print_Pressure_Transmitter_Value(int count)   // Printing average transmitter value, count is given by the operator
{                                                    // Count is the number of values that will be averaged
  // int val = analogRead(Pressure_Transmitter_Pin);
  // Serial.println(val);
  float analog_val = 0;
  for(int i = 0; i<count; i++)
  {
    analog_val += analogRead(Pressure_Transmitter_Pin);
  }
  float avg_analog_val = analog_val/count;
  //Serial.println(avg_analog_val);
  return avg_analog_val;
}

void setup() 
{
  Serial.begin(9600);
  pinMode(AMR_Pin, INPUT_PULLUP);
  pinMode(ON_pin,OUTPUT);
  pinMode(OFF_pin,OUTPUT);
  pinMode(Valve_pin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(AMR_Pin), Counter, FALLING);
}

/*
*Keeping things simple*
Counter value command = 'c'
Reset counter value = 'R'
Pressure transmitter value command = 'L'
*/

void loop() 
{
  if(Serial.available() > 0)
  {
    char command = Serial.read();
    if(command == 'c')
    {
      Print_Counter_Value();
    }
    else if (command == 't')
    {
      Print_Time();
    }
    
    else if(command == 'P')
    {
      Print_Pressure_Transmitter_Value(1000);//prints the avg of 1000 analog_reads
    }

    else if (command == 'R')
    {
      Reset_Counter_and_Time();
    }

    else if (command == 'O')
    {
      digitalWrite(ON_pin,HIGH);
    }

    else if (command == 'o')
    {
      digitalWrite(ON_pin,LOW);
    }

    else if (command == 'F')
    {
      digitalWrite(OFF_pin,HIGH);
    }

    else if (command == 'f')
    {
      digitalWrite(OFF_pin,LOW);
    }

    else if (command == 'V')
    {
      digitalWrite(Valve_pin,HIGH);
    }

    else if (command == 'v')
    {
      digitalWrite(Valve_pin,LOW);
    }
    
    else
    {
      Serial.println(-1);
    }
    
  }
}