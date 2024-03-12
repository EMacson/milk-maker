#include <AceButton.h>
#include <Servo.h>

using namespace ace_button;

// pin definitions
const int SERVO1_PIN = 3;
const int SERVO2_PIN = 5;
const int LED_B_PIN = 7;
const int LED_R_PIN = 9;
const int LED_G_PIN = 10;
const int BUTTON_PIN = 13;

// string for reading from serial and converting to an int
String inString = "";

int soyFollowers = 0;
int oatFollowers = 0;
int almondFollowers = 0;
int coconutFollowers = 0;

bool inProcess = false;
bool buttonPressed = false;

Servo servo1;
Servo servo2;

AceButton button(BUTTON_PIN);

void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  Serial.begin(9600);

  // setup up servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // setup LEDs
  pinMode(LED_B_PIN,OUTPUT);
  pinMode(LED_R_PIN,OUTPUT);
  pinMode(LED_G_PIN,OUTPUT);

  // setup button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);

  digitalWrite(LED_G_PIN,HIGH);

  servo1.write(80);
  servo2.write(80);
}

void loop() {
  // check if arduino is currently in process of dispensing
  if(!inProcess) {
    button.check();
  }

  if(buttonPressed) {
    delay(1000);

    if(Serial.available() > 0) {
      delay(1000);

      // loop for reading data from serial given by python code
      while(1){
        int inChar = Serial.read();
        if(isDigit(inChar)) {
          inString += (char)inChar;
        }

        if(inChar == 'S') {
          soyFollowers = inString.toInt();
          inString = "";
        }
        else if(inChar == 'O') {
          oatFollowers = inString.toInt();
          inString = "";
        }
        else if(inChar == 'A') {
          almondFollowers = inString.toInt();
          inString = "";
        }
        // C is the final deliminator
        else if(inChar == 'C') {
          coconutFollowers = inString.toInt();
          inString = "";
          break;
        }
      }
    }

    // debugging check to make sure not follower counts were 0
    // I'm assuming each page has atleast one follower
    if(((soyFollowers != 0) && (oatFollowers != 0)) && ((almondFollowers != 0) && (coconutFollowers != 0))) {
      digitalWrite(7,HIGH);
      delay(1000);
      digitalWrite(7,LOW);
    }

    delay(500);

    // dispense soy and oat
    servo1.write(75);
    delay(soyFollowers*100+1000);
    servo1.write(85);
    delay(oatFollowers*100+1000);
    servo1.write(80);

    delay(500);

    // disoense almond and coconut
    servo2.write(75);
    delay(almondFollowers*100+1000);
    servo2.write(85);
    delay(coconutFollowers*100+1000);
    servo2.write(80);

    delay(500);

    // reset and end process
    digitalWrite(LED_R_PIN,LOW);
    digitalWrite(LED_G_PIN,HIGH);
    inProcess = false;
    buttonPressed = false;
  }
}

void handleEvent(AceButton* /*button*/, uint8_t eventType,
    uint8_t /*buttonState*/) {
  switch (eventType) {
    case AceButton::kEventPressed:
      inProcess = true;
      buttonPressed = true;
      Serial.println(1);
      digitalWrite(LED_G_PIN,LOW);
      digitalWrite(LED_R_PIN,HIGH);
      break;
    case AceButton::kEventReleased:
      Serial.println(0);
      break;
  }
}
