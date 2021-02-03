#include <Arduino.h>
#define AMR_Pin 2
#define Pressure_Transmitter_Pin A0
#define bounce_time 1000
#define VFD_ON_OFF_Pin 4
#define Valve_ON_OFF_Pin 5
#define Extra_Relay_Connected_To_Gnd 6
#define Level_pin A1

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

void Print_Pressure_Transmitter_Value(int count)   // Printing average transmitter value, count is given by the operator
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
  Serial.println(avg_analog_val);
}

void Print_Level_Sensor_Value(int count)   // Printing Level value, count is given by the operator
{                                                    // Count is the number of values that will be averaged
  float analog_val = 0;
  for(int i = 0; i<count; i++)
  {
    analog_val += analogRead(Level_pin);
  }
  float avg_analog_val = analog_val/count;
  //Serial.println(avg_analog_val);
  float val_in_volts = (avg_analog_val*5.0/1023);
  // Serial.print("Volts: ");
  // Serial.print(val_in_volts);
  // Serial.print("V ");
  // Serial.print(" Level: ");
  float level = (((val_in_volts*1.218905053)-0.565117967-0.61));
  level=(level*10000)/3.946;
  if(level<=0)
    Serial.print(0);
  else
    Serial.println(level/100);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(AMR_Pin, INPUT_PULLUP);
  pinMode(VFD_ON_OFF_Pin,OUTPUT);
  pinMode(Valve_ON_OFF_Pin,OUTPUT);
  pinMode(Extra_Relay_Connected_To_Gnd,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(AMR_Pin), Counter, FALLING);
}

/*
*Keeping things simple*
Counter value command = 'c'
Time interval between two counts command = 't'
Reset counter value = 'R'
Pressure transmitter value command = 'P'
VFD OFF command = 'O'
VFD ON command = 'o'
Valve Open command = 'V'
Valve Close command = 'v'
Extra relay ON command = 'F'
Extra relay OFF command = 'f'

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

    else if(command == 'L')
    {
      Print_Level_Sensor_Value(1000);
    }

    else if (command == 'R')
    {
      Reset_Counter_and_Time();
    }

    else if (command == 'O')
    {
      digitalWrite(VFD_ON_OFF_Pin,HIGH);
    }

    else if (command == 'o')
    {
      digitalWrite(VFD_ON_OFF_Pin,LOW);
    }

    else if (command == 'V')
    {
      digitalWrite(Valve_ON_OFF_Pin,HIGH);
    }

    else if (command == 'v')
    {
      digitalWrite(Valve_ON_OFF_Pin,LOW);
    }

    else if (command == 'F')
    {
      digitalWrite(Extra_Relay_Connected_To_Gnd,HIGH);
    }

    else if (command == 'f')
    {
      digitalWrite(Extra_Relay_Connected_To_Gnd,LOW);
    }
    
    else
    {
      Serial.println(-1);
    }
    
  }
}