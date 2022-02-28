//Author : JohnSantos Ndiwayesu
//on : 20th Jan 2018
//if you encure syntax erros please replace the ' with "

//include first MyRealTimeClock myRTC Module
#include <MyRealTimeClock.h>
#include <avr/wdt.h>

//MyRealTimeClock myRTC(CLK, data, RST);
MyRealTimeClock myRTC(2, 1, 0);

const int bouton = 3;
const int temoin = 4;

// Selection et declaration des differentes broches sur arduino
const int vert_VK = 9;
const int rouge_VK = 8;
const int jaune_VK = 10;

const int vert_Cercle = 6;
const int rouge_Cercle = 5;
const int jaune_Cercle = 7;

const int vert_KM = 12;
const int rouge_KM = 11;
const int jaune_KM = 13;

/* tempA est la duree de priorite pour l'axe ville-katindo contre tempB
   Ces durees varient selon les etats
*/

int tempA = 0;
int tempB = 0;

// declaration de differents etats de la circulation routière
typedef enum etats  {
  
  etat_normal,
  etat_moyen,
  etat_grave,
  etat_eteint
  } etats;

etats etat;

void setup() {
  wdt_disable();

  digitalWrite(bouton,LOW);
  pinMode(bouton,INPUT);
  digitalWrite(temoin,LOW);
  pinMode(temoin,OUTPUT);
  
  // definition des broches en sortie pour differentes signalisations
  pinMode(vert_VK,OUTPUT);
  pinMode(rouge_VK,OUTPUT);
  pinMode(jaune_VK,OUTPUT);
  
  pinMode(vert_Cercle,OUTPUT);
  pinMode(rouge_Cercle,OUTPUT);
  pinMode(jaune_Cercle,OUTPUT);

  pinMode(vert_KM,OUTPUT);
  pinMode(rouge_KM,OUTPUT);
  pinMode(jaune_KM,OUTPUT);

  // Initialisation des nos sorties à l'etat haut
  digitalWrite(vert_VK,LOW);
  digitalWrite(rouge_VK,LOW);
  digitalWrite(jaune_VK,LOW);
  
  digitalWrite(vert_Cercle,LOW);
  digitalWrite(rouge_Cercle,LOW);
  digitalWrite(jaune_Cercle,LOW);

  digitalWrite(vert_KM,LOW);
  digitalWrite(rouge_KM,LOW);
  digitalWrite(jaune_KM,LOW);

  //wdt_enable(WDTO_4S);
  }

void loop() {
    wdt_reset();
  
    myRTC.updateTime();

    if(digitalRead(bouton) == HIGH){
        while(digitalRead(bouton) == HIGH)delay(10);

        myRTC.setDS1302Time(00, 00, 00, 00 , 00, 00, 2017);
        
        for(int i = 0; i< 5; i++){
           digitalWrite(temoin,LOW);
           delay(200);
           digitalWrite(temoin,HIGH);
           delay(200);
           wdt_reset();
         }
           digitalWrite(temoin,LOW);
    }

  // Condition temporelle  pour passer à l'etat etteint(00h à 4h59)
  if((myRTC.hours >= 0) &&(myRTC.hours < 5)){
      etat = etat_eteint;
     }
     
 // Condition temporelle  pour passer à l'etat normal(5h à 11h30)
  if((myRTC.hours >= 5) &&((myRTC.hours + myRTC.minutes) < 41)){ 
        etat = etat_normal;
     }
     
 // Condition temporelle  pour passer à l'etat moyen(11h31 à 16h40 et 20h01 à 23h59) 
 if((((myRTC.hours + myRTC.minutes) >= 42) &&((myRTC.hours + myRTC.minutes) < 56))||
    (((myRTC.hours + myRTC.minutes) >= 21) &&((myRTC.hours + myRTC.minutes) < 82))){
        etat = etat_moyen;
     }
     
  // Condition temporelle  pour passer à l'etat grave(16h41 à 20h00
  if(((myRTC.hours + myRTC.minutes >= 57))&&(myRTC.hours <= 20)){
        etat = etat_grave;
     }

etat = etat_grave;

switch(etat){
/*Les durees tempA et tempB sont en secondes */
  
case etat_normal :
//tempA = 1min-10sec
//tempB = 1min-10sec
tempA = 50; 
tempB = 50;
  Basculer();
  break;

case etat_moyen :
//tempA = 2min-10sec
//tempB = 1min-10sec
tempA = 110; 
tempB = 50;
  Basculer();
  break;

case etat_grave :
//tempA = 3min-10sec
//tempB = 1min-10sec
tempA = 170; 
tempB = 50;
  Basculer();
  break;

case etat_eteint :
  digitalWrite(vert_VK,LOW);
  digitalWrite(rouge_VK,LOW);
  digitalWrite(jaune_VK,LOW);

  digitalWrite(vert_Cercle,LOW);
  digitalWrite(rouge_Cercle,LOW);
  digitalWrite(jaune_Cercle,LOW);
    
  digitalWrite(vert_KM,LOW);
  digitalWrite(rouge_KM,LOW);
  digitalWrite(jaune_KM,LOW);
  break;

      }
   }
//fonction de clignotement de 10 sec du feu jaune

 void Allumerjaune (int jaune){
  digitalWrite(jaune_VK,LOW);
  digitalWrite(jaune_Cercle,LOW);
  digitalWrite(jaune_KM,LOW);
  
 for(int i = 0; i<10; i++){
  
  wdt_reset();
  digitalWrite(jaune,HIGH);
  delay(500);
  digitalWrite(jaune,LOW);
  delay(500);
     }
  }

 void Basculer(){
  
  digitalWrite(vert_VK,HIGH);
  digitalWrite(vert_Cercle,LOW);
  digitalWrite(vert_KM,LOW);
  
  digitalWrite(rouge_VK,LOW);
  digitalWrite(rouge_Cercle,HIGH);
  digitalWrite(rouge_KM,HIGH);
  
  tempoSec(tempA);
  
  Allumerjaune(jaune_VK);
  digitalWrite(vert_VK,LOW);
  digitalWrite(vert_Cercle,LOW);
  digitalWrite(vert_KM,HIGH);
  
  digitalWrite(rouge_VK,HIGH);
  digitalWrite(rouge_Cercle,HIGH);
  digitalWrite(rouge_KM,LOW);
  
  tempoSec(tempB);
  
  Allumerjaune(jaune_KM);
  digitalWrite(vert_VK,LOW);
  digitalWrite(vert_Cercle,HIGH);
  digitalWrite(vert_KM,LOW);
  
  digitalWrite(rouge_VK,HIGH);
  digitalWrite(rouge_Cercle,LOW);
  digitalWrite(rouge_KM,HIGH);
  
  tempoSec(tempB);
  
  Allumerjaune(jaune_Cercle); 
  }
//Fonction de temporisation pour des durees superieur ou egales a une seconde
 void tempoSec(int secondes){
   for(int i = 0; i < secondes; i++){
      delay(1000);
      wdt_reset();
     }
 }
