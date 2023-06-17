#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//----------- Enter you Wi-Fi Details---------//

char ssid[] = "Tort";    //SSID
char pass[] = "8587HFS97"; // Password

//-------------------------------------------//

unsigned long myChannelField =  1320888;    // Channel ID
const int vitesse_ChannelField = 1; //  channel 1 to write data
const int matricule_ChannelField = 2; //channel 2 to write data
const int front_distnc_ChannelField = 3; //channel 2 to write data
const int back_dist_ChannelField = 4; //channel 2 to write data
const int left_side_ChannelField = 5; //channel 2 to write data
const int right_side_ChannelField = 6; //channel 2 to write data

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
int dismin=1;
int dismax=10;
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

Temps_start_us=micros();


duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds


distance= duration / 58; 
//Calculating actual/real distance



if(distance > 10 )
{
  in1=distance;
  Serial.print(" Good ");
  digitalWrite(led, LOW);
  
}
else
{
  out1=distance;
  
  Serial.print("Be carful !! ");
  digitalWrite(led, HIGH);
}
Temps_stop_us=micros();
Duree_us=Temps_stop_us-Temps_start_us;
vitesse = (((in1-out1)*0.01)/(Duree_us*0.000001));

 // créer une requette HTTP  Client.print()
 
/*
const int vitesse_ChannelField = 1; //  channel 1 to write data
const int matricule_ChannelField = 2; //channel 2 to write data
const int front_distnc_ChannelField = 3; //channel 2 to write data
const int back_dist_ChannelField = 4; //channel 2 to write data
const int left_side_ChannelField = 5; //channel 2 to write data
const int right_side_ChannelField = 6; //channel 2 to write data
*/
  int  distfront=distance;
  int  distback=distance-2;
  int  distls=distance-4;
  int  distrs=distance -6;


   ThingSpeak.setField(1, vitesse);
   ThingSpeak.setField(2,matr);
   ThingSpeak.setField(3,distfront);
   ThingSpeak.setField(4,distback);
   ThingSpeak.setField(5,distls);
   ThingSpeak.setField(6,distrs);
   ThingSpeak.writeFields(myChannelField, myWriteAPIKey);


Serial.print("  vitesse = ");        //Output distance on arduino serial monitor 
Serial.println(vitesse);


digitalWrite(led, LOW);
delay(1000);

  

                     //Pause for 3 seconds and start measuring distance again
}
 
