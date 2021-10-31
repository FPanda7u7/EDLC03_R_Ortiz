#include <Servo.h>

#define buzzer 8
Servo servoU;

unsigned long myTime;
unsigned long myTimeA = 0;
unsigned long myTimeB;
unsigned long myTimeBB = 0;
unsigned long myTimeX = 0;

unsigned long myTimeP;
unsigned long myTimeF;

String estado = "";
int estabilidad = 113;
bool simulador;
bool normal = true;
bool pausa = false;
bool alerta = false;
bool perdiste = false;
bool sumar = true;

int BarrilA = 100;
int BotonA = 5;
int LedA = 10;
bool flagA = false;

int BarrilB = 100;
int BotonB = 6;
int LedB = 11;
bool flagB = false;

int BarrilC = 100;
int BotonC = 7;
int LedC = 12;
bool flagC = false;

int Boton = 13;
bool flag = false;

void setup()
{
  Serial.begin(9600);
  servoU.attach(9);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(13, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);  
  pinMode(12, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

void loop()
{ 
  myTime = millis();
  myTimeB = millis();
  
  servoU.write(estabilidad);
  
  if (!pausa && !perdiste)
  {
    aumentar();
    clasificar();
    pulsadores();
    estabilizar();    
  }  
  
  //Pausa
  if (millis() - myTimeP >= 30000)
  {
    pausa = false;
  } 
  muted();
  lose();
  
  Serial.print(BarrilA); Serial.print(";");
  Serial.print(BarrilB); Serial.print(";");
  Serial.print(BarrilC); Serial.print(";");
  Serial.print(estabilidad); Serial.print(";");
  Serial.print(estado); Serial.print(";");
  Serial.print(pausa); Serial.print(";");
  Serial.print(alerta); Serial.print(";"); 
  Serial.print(perdiste); Serial.println(""); 
}

void aumentar()
{
  if (myTime - myTimeA >= 10000)
  {
    if (estabilidad >= 0 && estabilidad <= 135)
    {
      myTimeA = myTime;
      estabilidad -= 10;
    }
    else if (estabilidad >= 136 && estabilidad <= 180) 
    {
      myTimeA = myTime;
      estabilidad += 10;
    }   
  }
}

void clasificar()
{
  if (estabilidad <= 180 && estabilidad >= 136) //Bajo Nivel
  {
    estado = "Bajo Nivel";    
    if (myTimeB - myTimeBB >= 8000)
    {
      myTimeBB = myTimeB;
      digitalWrite(buzzer, LOW); 
    }
    normal = true;
  }
   if (estabilidad <= 135 && estabilidad >= 91 && normal) //Normal
  {
    estado = "Normal";  
    digitalWrite(buzzer, LOW);
    normal = false;
  }
   if (estabilidad <= 90 && estabilidad >= 46) //Precaución
  {
    estado = "Precaucion";
    if (myTimeB - myTimeBB >= 4000)
    {
      myTimeBB = myTimeB;
      digitalWrite(buzzer, LOW); 
    }
    normal = true;
  }
   if (estabilidad <= 45 && estabilidad >= 0) //Peligro
  {
    estado = "Peligro";
    if (myTimeB - myTimeBB >= 1000)
    {
      myTimeBB = myTimeB;
      digitalWrite(buzzer, LOW); 
    }
    normal = true;
  }
}

void pulsadores()
{ 
  if (digitalRead(BotonA) == LOW) //Pulsador A
  {
    digitalWrite(LedA, LOW);
    flagA = true;
  }else{
    digitalWrite(LedA, HIGH);
    if (flagA)
    {
      if (BarrilA > 0 && estabilidad < 180)
      {
        estabilidad += 9;
        BarrilA -= 5;
      }    
      flagA = false;
    } 
  }
  
  if (digitalRead(BotonB) == LOW) //Pulsador B
  {
    digitalWrite(LedB, LOW);
    flagB = true;
  }else{
    digitalWrite(LedB, HIGH);
    if (flagB)
    {
      if (BarrilB > 0 && estabilidad > 0)
      {
        estabilidad -= 18; 
        BarrilB -= 10;
      }      
      flagB = false;
    } 
  }
  
  if (digitalRead(BotonC) == LOW) //Pulsador C
  {
    digitalWrite(LedC, LOW);
    flagC = true;
  }else{
    digitalWrite(LedC, HIGH);
    if (flagC)
    {
      if (BarrilB > 0 && estabilidad > 0)
      {
        estabilidad -= 36;
        BarrilC -= 20;
      }     
      flagC = false;
    } 
  }

  if (digitalRead(Boton) == LOW) //Pulsador
  {
    flag = true;
  }else{
    if (flag)
    {
      pausa = true;
      myTimeP = millis();
      flag = false;
    }
  }
}

void lose()
{
  if (pausa)
  {
    alerta = false;
  }
  
  if ((estado == "Bajo Nivel" || estado == "Peligro") && !alerta)
  {
    myTimeF = millis();    
    alerta = true;
  }
  else if ((estado == "Normal" || estado == "Precaucion") && alerta)
  {
    alerta = false;
  }

  if (millis() - myTimeF >= 30000 && alerta)
  {
    perdiste = true;
  }

  if (perdiste && digitalRead(Boton) == HIGH)
  {   
    estabilidad = 123;
    BarrilA = 100;
    BarrilB = 100;
    BarrilC = 100;
    flag = false;
    alerta = false;
    perdiste = false;
  }
}

void estabilizar()
{
  if (estabilidad > 180)
  {
    estabilidad = 180;
  }
  if (estabilidad < 0)
  {
    estabilidad = 0;
  }
}

void muted()
{  
  if (digitalRead(buzzer) == LOW)
  {     
    myTimeX = myTimeBB;
    if (myTimeB - myTimeX >= 100)
    {  
      digitalWrite(buzzer, HIGH);    
    }
  }  
}
