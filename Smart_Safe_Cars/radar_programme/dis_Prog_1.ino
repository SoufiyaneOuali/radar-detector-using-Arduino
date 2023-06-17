#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//----------- Enter you Wi-Fi Details---------//

char ssid[] = "Tort";    //SSID
char pass[] = "8587HFS97"; // Password

//-------------------------------------------//

unsigned long myChannelField =  1320888;    // Channel ID
const int ChannelField = 1; // Which channel to write data
const int ChannelField_2 = 2; // Which channel to write data
const char * myWriteAPIKey = "ZRIBGYQQ8R0HV5TF"; // Your write API Key

//-------------------------------------------//
WiFiClient  client;

 
const int echoP = 16;  //D0 Or GPIO-0 of nodemcu
const int trigP = 5;  //D1 Or GPIO-2 of nodemcu

const int led=0; //D3 
unsigned long Temps_start_us,Temps_stop_us;
unsigned long  Duree_us =0;

int duration;
int distance;
int dismin=20;
int dismax=42;
float vitesse =0.0 ;
 int in1 ;
 int out1;

int  matr = 1 ; ;//matricule Fake Number
 
void setup() {
pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
pinMode(echoP, INPUT);   // Sets the echoPin as an Input
pinMode(led, OUTPUT);
ThingSpeak.begin(client);
Serial.begin(9600);      // Open serial channel at 9600 baud rate
}

void loop() 
{

 // création  d'une connexion TCP 
 
if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }


digitalWrite(trigP, LOW);   // Makes trigPin low
delayMicroseconds(2);       // 2 micro second delay 

digitalWrite(trigP, HIGH);  // tigPin high
delayMicroseconds(10);      // trigPin high for 10 micro seconds
digitalWrite(trigP, LOW);   // trigPin low

duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
distance= duration / 58; 

//Calculating actual/real distance


if(distance <= dismax && distance > 15 )
{
  in1 = distance;
  digitalWrite(led, HIGH);  
  Temps_start_us=micros();
}
else if(distance <= dismin && distance > 9)
{
  out1 = distance;
  digitalWrite(led, LOW);
  Temps_stop_us=micros(); 
  Duree_us=Temps_stop_us-Temps_start_us;
  if(Duree_us != 0)
{
vitesse = (((in1-out1)*0.01)/(Duree_us*0.000001));
}

if(vitesse > 9 )
{
  Serial.print(" \n !!!!!! Haut vitesse ! Fait attention !!!!! \n ");

  
   
  // String matricule="matricule_"+String(i) ; 
   ThingSpeak.setField(1, vitesse);
   ThingSpeak.setField(2,matr);
   ThingSpeak.writeFields(myChannelField, myWriteAPIKey);
   matr++;
}

Serial.print("  vitesse = ");        //Output distance on arduino serial monitor 
Serial.println(vitesse);

}









//  digitalWrite(led, HIGH);  




                     //Pause for 3 seconds and start measuring distance again
}
 
