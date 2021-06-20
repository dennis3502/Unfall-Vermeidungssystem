/*
* Wer: Dennis Sieber, HKA, side1011, SS21
* Wann: 20.06.2021
* 
* Was: Projektarbeit technische Informatik Labor, autonom fahrendes Legoauto, welches Hindernissen auf seinem Weg ausweicht 
* Bauteile: 1x Lego 8296, 1x ESP32 AZDelivery DevKit v4,1x Breadboard, 1x Servomotor, 1x DRV8833 Motortreiber, 1x Gleichstrom E-motor, 1x Ultraschallsensor, 1x Start/Stopp Knopf (Ky-004), 1x Powerbank
* 
*/

#include <Servo.h>                                                                // Bibliothek zur Steuerung des Servos einbinden

// Servomotor
Servo myservo;                                                                    // Ansteuerung des Servomotors über myservo 
int pos = 75;                                                                     // Ausgangsposition des Servos auf 75° setzen

// Ultraschallsensor
long laenge;                                                                      // Variable laenge zur Messung des Abstands
long cm;                                                                          // Variable cm zur Messung des Abstands

// DC - Motor
int motor1Pin1 = 25;                                                              // Motorpin 1 des DC - Motors dem Pin 25 des ESP32 zugeordnet
int motor1Pin2 = 26;                                                              // Motorpin 2 des DC - Motors dem Pin 26 des ESP32 zugeordnet

// Start/Stopp Knopf
int buttonPin = 16;                                                               // Der Pin 16 wird dem Knopf zugeordnet                                                      
int val;                                                                          // Variable "val" zum bestimmen des Knopfdrucks

void setup()                                                                      // put your setup code here, to run once:
{
  // Servo
  myservo.attach(13);                                                             // Servomotor dem Pin 13 des ESP32 zugeordnet
  myservo.write(pos);                                                             // Servomotor dreht auf die bestimmte pos = 75

  // DC - Motor
  pinMode(motor1Pin1, OUTPUT);                                                    // Motorpin 1 als Output
  pinMode(motor1Pin2, OUTPUT);                                                    // Motorpin 2 als Output

  // Ultraschallsensor
  pinMode(4, OUTPUT);                                                             // Trigger des Ultraschallsensors auf Pin 4 des ESP32 und als Output 
  pinMode(5, INPUT);                                                              // Echo des Ultraschallsensors auf Pin 5 des ESP32 und als Input

  // Start/Stopp Knopf
  pinMode(buttonPin, INPUT_PULLUP);                                               // Pin des Knopfes als INPUT_OUTPUT                                                     


  Serial.begin(9600);                                                             // Baudrate 9600 festgelegt
}

void loop()                                                                       // put your main code here, to run repeatedly: 
{
  val = digitalRead(buttonPin);                                                   // Variable "val" wird der Wert des Knopfes zugewiesen                         
  while(val == LOW)                                                               // Start/Stopp Knopf wartet auf Startsignal um das Programm zu starten               
  {
    Serial.println("Moving Forward");                                             // Serielle Ausgabe
    digitalWrite(motor1Pin1, LOW);                                                // DC Motor dreht vorwärts
    digitalWrite(motor1Pin2, HIGH);                                               // DC Motor dreht vorwärts
  
    // Ultraschallsensor misst Abstand zu Hindernis
    digitalWrite(4, LOW);                                                         // Abstand wird gemessen
    delayMicroseconds(0.5);                                                       // 0.5µs warten
    digitalWrite(4, HIGH);                                                        // Abstand wird gemessen
    delayMicroseconds(1);                                                         // 1µs warten
    digitalWrite(4, LOW);                                                         // Abstand wird gemessen

    laenge = pulseIn(5, HIGH);                                                    // Abstand zur Hindernis wird berechnet

    cm = (laenge * 34) /2000;                                                     // Berechnung des Abstandes zum Hindernis

    Serial.print(cm);                                                             // Serielle Ausgabe des Abstandes
    Serial.println("cm");                                                         // Serielle Ausgabe des Abstandes

    delay(50);                                                                    // 0.05s warten

    // Wenn der Abstand zum Hindernis < 15 cm und nicht 0 cm
    if(cm<15 && cm != 0)                                                          
    {
      // DC Motor stoppt
      Serial.println("Motor stopped");                                            // Serielle Ausgabe 
      digitalWrite(motor1Pin1, LOW);                                              // DC Motor stoppt
      digitalWrite(motor1Pin2, LOW);                                              // DC Motor stoppt
    
      // DC - Motor dreht rückwärts für eine kurze Zeit
      // für nötigen Abstand fürs sichere Umfahren
      Serial.println("Moving Backwards");                                         // Serielle Ausgabe
      digitalWrite(motor1Pin1, HIGH);                                             // DC Motor dreht rückwärts
      digitalWrite(motor1Pin2, LOW);                                              // DC Motor dreht rückwärts
      delay(2000);                                                                // DC Motor dreht 2s rückwärts

      // DC Motor stoppt
      Serial.println("Motor stopped");                                            // Serielle Ausgabe 
      digitalWrite(motor1Pin1, LOW);                                              // DC Motor stoppt
      digitalWrite(motor1Pin2, LOW);                                              // DC Motor stoppt
      delay(250);                                                                 // 0.25s warten

      // Lenkung nach rechts
      for(pos = 75; pos <= 150; pos += 5)                                         // Servo dreht um 75° nach rechts
      {
        myservo.write(pos);                                                       // Position des Servos wird verändert um +5°
        delay(15);                                                                // 0.015s warten
      }

      // DC Motor dreht vorwärts
      Serial.println("Moving Forward");                                           // Serielle Ausgabe
      digitalWrite(motor1Pin1, LOW);                                              // DC Motor dreht vorwärts
      digitalWrite(motor1Pin2, HIGH);                                             // DC Motor dreht vorwärts
      delay(1000);                                                                // DC Motor dreht 1s vorwärts
      
      // Lenkung dreht nach links
      for(pos = 150; pos >= 75; pos -= 5)                                         // Servo dreht Lenkung um 75° zurück auf "geradeausfahrt"
      {
        myservo.write(pos);                                                       // Position des Servos wird verändert um -5°
        delay(15);                                                                // 0.015s warten
      }
    
      // DC - Motor fährt für 0.5 Sekunden
      Serial.println("Moving Forward");                                           // Serielle Ausgabe
      digitalWrite(motor1Pin1, LOW);                                              // DC Motor dreht vorwärts
      digitalWrite(motor1Pin2, HIGH);                                             // DC Motor dreht vorwärts
      delay(500);                                                                 // DC Motor dreht 0.5s vorwärts
  
      // Lenkung dreht nach links
      for(pos = 75; pos >= 0; pos -= 5)                                           // Servo dreht Lenkung um 75° nach links
      {
        myservo.write(pos);                                                       // Position des Servos wird verändert um -5°
        delay(15);                                                                // 0.015s warten
      }
      delay(1800);                                                                // 1.8s fahren
   
      // Lenkung dreht nach rechts
      for(pos = 0; pos <= 120; pos += 5)                                          // Servo dreht 120° nach rechts
      {
        myservo.write(pos);                                                       // Position des Servos wird verändert um +5°
        delay (15);                                                               // 0.015s warten
      }
      delay(1500);                                                                // 1.5s warten

      // Lenkung nach links
      for(pos = 120; pos >= 75; pos -= 5)                                         // Servo dreht 45° nach links; Auto soll nun in die Anfangsrichtung fahren
      {
        myservo.write(pos);                                                       // Position des Servos wird verändert um -5°
        delay (15);                                                               // 0.015s warten
      }
      delay(1000);                                                                 // 1s warten

      // DC Motor stoppt
      Serial.println("Motor stopped");                                            // Serielle Ausgabe 
      digitalWrite(motor1Pin1, LOW);                                              // DC Motor stoppt
      digitalWrite(motor1Pin2, LOW);                                              // DC Motor stoppt
      delay(5000);                                                                // Auto stoppt für 5s (für Aufnahe),
    }                                                                             // Schleife endet hier, DC Motor dreht ab jetzt weiterhin vorwärts, bis der Ultraschallsensor ein Signal cm < 12 misst und somit wieder ein "Ausweichmanöver einleitet 
   }                                                                              
   val = 0;                                                                       // Variable val wird wieder auf Null gesetzt
  
  while(val != LOW)
  {
    Serial.println("Warte auf start");                                          // Knopf nicht gedrückt --> warten
    val = digitalRead(buttonPin);                                               // Checkt ob Knopf gedrückt
    delay(1000);                                                                // 1s warten
  }
}
