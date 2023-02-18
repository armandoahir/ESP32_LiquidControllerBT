/*
 * Programma che gestisce, via bluetooth, il controllo di un sensore HC - SR04 (di profondità) mediante un modulo ESP32 
 * Progetto di Sistemi di elettronica digitale
 * Scritto e realizzato da: Ahir Armando
 */
 
#include<stdio.h>
#include<stdlib.h>
#include "BluetoothSerial.h"

#define SOUND_SPEED 0.034 //velocita' del suono approssimata, utilizzata nel sensore HC - SR04 per il calcolo della distanza 
#define MAX_VOL 20
#define SOGLIA 1

//definizione delle variabili
BluetoothSerial SerialBT; 
const int triggerPin = 18, echoPin = 19, buzzerPin = 33;
double distance, quantita;
unsigned long duration;
String input = "";
bool flag = true;

void setup() {
  SerialBT.begin("ESP32 AHIR");
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, OUTPUT);
  pinMode(buzzerPin,OUTPUT);
}

void loop() {
  bluetooth();
  
  if(input == "controllo_ON") {
     do {
      sensor();
     }while(!flag);
  }
  
  if(!flag) {
    return;
  }
}

//inizializzazione bluetooth

void bluetooth() {
  if(SerialBT.available()){  //Se c'è qualche informazione dalla linea seriale... 
    char incomingChar = SerialBT.read(); 
    
    if (incomingChar != '\n'){
      input += String(incomingChar); //cast da carattere a stringa, la variabile input diventa una sommatoria di caratteri.
    }else{
      input = "";
    }
    Serial.write(incomingChar);  //verifica: scrittura su seriale del comando dato in input 
  } 
}

//gestione allarme
void buzzAlarm() { 
  flag = false;
   for(int i = 0; i < 4; i++) { 
    digitalWrite(buzzerPin,HIGH);
    delay(50);
    
    digitalWrite(buzzerPin,LOW);
    delay(25);
  } 
}

//gestore sensore
void sensor() {
  flag = true;
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  
  duration = pulseIn(echoPin,HIGH); //conta il tempo nel quale il pin al primo parametro si trova nello stato nel secondo parametro
  distance = duration * SOUND_SPEED / 2; 
  quantita = MAX_VOL - distance; // quantita' = differenza tra distanza misurata e il totale del volume del serbatoio
    
  SerialBT.print("RUNNING:  "); 
  SerialBT.print(quantita);
  SerialBT.println(" lt.");
        
  Serial.print("RUNNING:  "); 
  Serial.print(quantita); 
  Serial.println(" lt."); 
  
  if(distance >= MAX_VOL) { //se DISTANZA raggiunge il valore massimo impostato, allora il serbatoio è PIENO.
    SerialBT.println("WARNING: SERBATOIO PIENO");
    Serial.println("WARNING: SERBATOIO PIENO");
    buzzAlarm();
    } else if(distance <= SOGLIA){ //se DISTANZA oltrepassa una SOGLIA minima impostata, allora il serbatoio è IN ESAURIMENTO
      
      SerialBT.println("WARNING: LIQUIDO IN ESAURIMENTO");
      Serial.println("WARNING: LIQUIDO IN ESAURIMENTO");
      buzzAlarm();
    }
    
   delay(500);
}
