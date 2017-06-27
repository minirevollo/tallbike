
/*
 Tallbike - Andreas Reheis 26.06.2017 für Martin Nothhelf
 Arduino steuert über zwei Bottons zwei MP3 Module, einen Stepper und ein Relais.

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 8, 9, 11, 12); // initialize the stepper library 
int schritte = 1000;            //Anzahl der Schritte des Steppmotors, 200 sind eine Umdrehung
int drehgeschwindigkeit = 200;  //Anzahl der Umdrehungen des Schrittmotors pro Minute
#define MUTE_BT 18              // Button zum Umschalten der Stimmung
#define EF_BT 19                // Button zum starten der Effekte
boolean mute_bt_status = false; // speichert ob der button gedrückt ist
long mute_bt_zeit = 0;
long mute_bt_zeitmax = 2000;    //Zeit die der button gedrückt werden muss
boolean mute_status = true;     // ob gute stimmung (true) oder schlechte (false)
boolean musik_status = true;    // ob gute musik (true) oder schlechte (false)
boolean effekt_bt_status = false;   // ob Effekt button gedrückt ist
boolean effekt_status = false;   //ob Effekt aktiviert ist
#define MUTE_PIN 14             //A0 start/stop auf dem MP3 Stimmung
#define MUTE_PIN_SKIP 15        //A1 skip auf dem MP3 STimmung
#define MUTE_PIN_REPEAT 16      //repeat auf MP3 Stimmung einschalten
#define EF_PIN 6                // start/stop auf dem MP3 Effekt
#define EF_PIN_SKIP 5           //skip auf dem MP3 Effekt
#define EF_PIN_REPEAT 4         //repeat auf MP3 Effekte einschalten
#define RAUCH_PIN 7             //Relay für Rauch

// ----------------------------  setup --------------------------------------------
void setup() {

myStepper.setSpeed(drehgeschwindigkeit);        // set the speed in rpm:
pinMode(MUTE_BT, INPUT);        // Botton um die Stimmung von gut zu schlecht zu ändern
digitalWrite(MUTE_BT, HIGH);    //internal pullup
pinMode(EF_BT, INPUT);          //Botton um einen Effekt zu aktivieren
digitalWrite(EF_BT, HIGH);      //internal pullup
pinMode(MUTE_PIN, OUTPUT);      //start stop  auf dem MP3 Stimmung
digitalWrite(MUTE_PIN, HIGH);  
pinMode(MUTE_PIN_SKIP, OUTPUT); // skip MP3 Stimmung
digitalWrite(MUTE_PIN_SKIP, OUTPUT);
pinMode(RAUCH_PIN, OUTPUT);     // zur Relaysteuerung für die Nebelmaschine
pinMode(MUTE_PIN_REPEAT, OUTPUT);// repeat mp3 Stimmung
pinMode(EF_PIN_REPEAT, OUTPUT);  // repeat für mp3 Effekte
digitalWrite(MUTE_PIN_REPEAT, LOW); //repeatfunktion für Stimmung aktiviert
delay(50);
digitalWrite(MUTE_PIN_REPEAT, HIGH);
digitalWrite(EF_PIN_REPEAT, LOW);   //repeatfunktion für Effekte aktiviert
delay(50);
digitalWrite(EF_PIN_REPEAT, HIGH);  

}
// ----------------------------- setup end ------------------------------------------
// ----------------------------- loop -----------------------------------------------
void loop() {

  mutebutton();
  effektbutton();
  musik();
  effekt();
  
}
// ----------------------------------- loop end -----------------------------------------
// ----------------------------------- mutebutton  -------------------------------------------
void mutebutton(){
  if(digitalRead(MUTE_BT) == LOW && mute_bt_status == false){
    mute_bt_status = true;
    mute_bt_zeit = millis();
  }
  if(digitalRead(MUTE_BT) == LOW && millis() - mute_bt_zeit > mute_bt_zeitmax){
    digitalWrite(MUTE_PIN, LOW);        //startet/bendet  Musik
    delay(100);
    digitalWrite(MUTE_PIN, HIGH);
    mute_bt_zeit = millis();
  }
  if(digitalRead(MUTE_BT) == HIGH && mute_bt_status == true){
    mute_bt_status = false;                
    !mute_status;                        //ändert die Stimmung
  }
}
// ------------------------------------ mutebutton end -------------------------------------
// ------------------------------------effektbutton --------------------------------------
void effektbutton(){
   if(digitalRead(EF_BT) == LOW && effekt_bt_status == false){
    effekt_bt_status = true;
   }
   if(digitalRead(EF_BT) == HIGH && effekt_bt_status == true){
    effekt_bt_status = false;             
    effekt_status = true;                      //aktiviert Effekte
   }
}
// ---------------------------------- effektbutton end ----------------------------------
// ------------------------------------- musik --------------------------------------------
void musik(){
  if(musik_status || mute_status){           //passt die Musik der Stimmung an
    digitalWrite(MUTE_PIN_SKIP, LOW);
    delay(100);
    digitalWrite(MUTE_PIN_SKIP, HIGH);
    !musik_status;
  }
}
// ------------------------------------- musik end ---------------------------------------
// --------------------------------------- effekt ----------------------------------------
void effekt(){
  if(effekt_status == true){
    digitalWrite(EF_BT, LOW);   //starte Effekt MP3
    delay(100);
    digitalWrite(EF_BT, HIGH);
    
    if(mute_status == true){
      stepper();
    }
    if(mute_status == false){
      rauch();
    }
    
    digitalWrite(EF_BT, LOW);   //stopt Effekt MP3
    delay(100);
    digitalWrite(EF_BT, HIGH);
  
    effekt_status == false;       //effekte werden deaktiviert
  }  
}
// ---------------------------------------effekt end -------------------------------------
// -------------------------------------- stepper ------------------------------------
void stepper(){
  
  myStepper.step(schritte);
  schritte = schritte * -1;
  delay(2000);
  myStepper.step(schritte);
  schritte = schritte * -1;

}
// -------------------------------------- stepper end -------------------------------------
// ------------------------------------ rauch -----------------------------------------
void rauch(){
  digitalWrite(RAUCH_PIN, HIGH);
  delay(2000);
  digitalWrite(RAUCH_PIN, LOW);
}
// -------------------------------------- rauch ende ------------------------------------
