#include <Servo.h> 
#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

Servo servoL;
Servo servoR;
Servo servoM;
Servo servoH;

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

int sL = 80;
int sR = 50;
int sM = 110;
int sH = 20;
bool var;


void setup() {

servoL.attach(27); 
servoL.write(sL);   

servoR.attach(26); 
servoR.write(sR);  

servoM.attach(25); 
servoM.write(sM);  

servoH.attach(24); 
servoH.write(sH); 

  Serial3.begin(9600);
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}
void loop() {

if (var == 1) {
   PS4.setLed(Red);
   rob1();
  }

if (var == 0) {
 PS4.setLed(Green);
 rob2();
}
}

  void rob1() {
  
  Usb.Task();

  if (PS4.connected()) {
    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS4.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));
      Serial.print(F("\tRightHatX: "));
      Serial.print(PS4.getAnalogHat(RightHatX));
      Serial.print(F("\tRightHatY: "));
      Serial.print(PS4.getAnalogHat(RightHatY));
    }

    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));
    }
    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
      
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
    else {
    if (PS4.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTraingle"));
      sR = sR + 5; 
        if (sR > 900) sR = 900;
        servoR.write(sR);
    }
    if (PS4.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle")); 
    }
    if (PS4.getButtonClick(CROSS)) {
      Serial.print(F("\r\nCross"));
      sR = sR - 5; 
        if (sR < 0) sR = 0;
        servoR.write(sR);
    }
    if (PS4.getButtonClick(SQUARE)) {
      Serial.print(F("\r\nSquare"));
    }
    if (PS4.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
      sL = sL + 5; 
        if (sL > 900) sL = 900;
       servoL.write(sL);
    } if (PS4.getButtonClick(RIGHT)) {
      Serial.print(F("\r\nRight"));
      sM = sM + 5; 
        if (sM > 900) sM = 900;
       servoM.write(sM);      
    } if (PS4.getButtonClick(DOWN)) {
      Serial.print(F("\r\nDown"));
      sL = sL - 5;
        if (sL < 0) sL = 0;
       servoL.write(sL);
    } if (PS4.getButtonClick(LEFT)) {
      Serial.print(F("\r\nLeft"));
      sM = sM - 5; 
        if (sM < 0) sM = 0;
       servoM.write(sM);      
    }

    if (PS4.getButtonClick(L1))  {
      Serial.print(F("\r\nL1"));
      sH=60;
      servoH.write(sH);
    }
    if (PS4.getButtonClick(L3))
      Serial.print(F("\r\nL3"));
    if (PS4.getButtonClick(R1))  {
      Serial.print(F("\r\nR1"));
      sH=10;
      servoH.write(sH);
    }
    if (PS4.getButtonClick(R3))
      Serial.print(F("\r\nR3"));

      if (PS4.getButtonClick(SHARE))
      var = 1;
        Serial.print(F("\r\nShare"));
      if (PS4.getButtonClick(OPTIONS)) {
        var = 0;
        Serial.print(F("\r\nOptions"));
        printAngle = !printAngle;
      }
      if (PS4.getButtonClick(TOUCHPAD)) {
        Serial.print(F("\r\nTouchpad"));
        printTouch = !printTouch;
      }

      if (printAngle) { // Print angle calculated using the accelerometer only
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS4.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS4.getAngle(Roll));
      }

      if (printTouch) { // Print the x, y coordinates of the touchpad
        if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
          Serial.print(F("\r\n"));
        for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
          if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
            Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getX(i));
            Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getY(i));
            Serial.print(F("\t"));
          }
        }
      }
    }
  }
}

 void rob2() {
  
  Usb.Task();

  if (PS4.connected()) {
    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS4.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));
      Serial.print(F("\tRightHatX: "));
      Serial.print(PS4.getAnalogHat(RightHatX));
      Serial.print(F("\tRightHatY: "));
      Serial.print(PS4.getAnalogHat(RightHatY));
    }

    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));
    }
    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
      
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
    else {
    if (PS4.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTraingle"));
      Serial3.print('3');
    }
    if (PS4.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle"));
            Serial3.print('S'); 
    }
    if (PS4.getButtonClick(CROSS)) {
      Serial.print(F("\r\nCross"));
      Serial3.print('1');
        servoR.write(sR);
    }
    if (PS4.getButtonClick(SQUARE)) {
      Serial.print(F("\r\nSquare"));
    }
    if (PS4.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
      Serial3.print('F');
    } if (PS4.getButtonClick(RIGHT)) {
      Serial.print(F("\r\nRight"));
      Serial3.print('R');    
    } if (PS4.getButtonClick(DOWN)) {
      Serial.print(F("\r\nDown"));
      Serial3.print('S');
    } if (PS4.getButtonClick(LEFT)) {
      Serial.print(F("\r\nLeft"));
      Serial3.print('L');  
    }
    if (PS4.getButtonClick(L1))  {
      Serial.print(F("\r\nL1"));
    }
    if (PS4.getButtonClick(L3))
      Serial.print(F("\r\nL3"));
    if (PS4.getButtonClick(R1))  {
      Serial.print(F("\r\nR1"));
    }
    if (PS4.getButtonClick(R3))
      Serial.print(F("\r\nR3"));

      if (PS4.getButtonClick(SHARE))
      var = 1;
        Serial.print(F("\r\nShare"));
      if (PS4.getButtonClick(OPTIONS)) {
        var = 0;
        Serial.print(F("\r\nOptions"));
        printAngle = !printAngle;
      }
      if (PS4.getButtonClick(TOUCHPAD)) {
        Serial.print(F("\r\nTouchpad"));
        printTouch = !printTouch;
      }

      if (printAngle) { // Print angle calculated using the accelerometer only
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS4.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS4.getAngle(Roll));
      }

      if (printTouch) { // Print the x, y coordinates of the touchpad
        if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
          Serial.print(F("\r\n"));
        for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
          if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
            Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getX(i));
            Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getY(i));
            Serial.print(F("\t"));
          }
        }
      }
    }
  }
}
