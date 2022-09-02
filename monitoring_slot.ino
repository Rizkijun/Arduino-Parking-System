#include <SPI.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define Trigpin 10
#define Echopin 11

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
Servo myServo2;
SoftwareSerial mySerial(8, 9);

int parking1_slot1_ir_s = 4;
int parking1_slot2_ir_s = 5;

int parking2_slot1_ir_s = 6;
int parking2_slot2_ir_s = 7;

int visitor = 0;
int statusslot = 0;
int maxslot = 4;
int statusrfid = 0;
int datastatus;

int con1 = 0;
int con2 = 0;
int con3 = 0;
int con4 = 0;

String sensor1;
String sensor2;
String sensor3;
String sensor4;

String cdata = "";


void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  mySerial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(Trigpin, OUTPUT);
  pinMode(Echopin, INPUT);
  myServo.attach(3);
  myServo.write(0);
  myServo2.attach(12);
  myServo2.write(0);
  pinMode(parking1_slot1_ir_s, INPUT);
  pinMode(parking1_slot2_ir_s, INPUT);
  pinMode(parking2_slot1_ir_s, INPUT);
  pinMode(parking2_slot2_ir_s, INPUT);
  lcd.setCursor (0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor (0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay (2000);
  lcd.clear();
}
void loop()
{
  p1slot1();
  p1slot2();
  p2slot1();
  p2slot2();
  entrance_gate();
  exitdoor();
  printdata();
}

void p1slot1()
{
  if (con1 == 0)
  {
    if (digitalRead(parking1_slot1_ir_s) == HIGH)
    {
      sensor1 = "p1s1off";
      Serial.println(sensor1);
      delay(200);
      con1 = 1;
      visitor -= 1;
      statusslot += 1;
    }
  }
  if (con1 == 1)
  {
    if (digitalRead(parking1_slot1_ir_s) == LOW)
    {
      sensor1 = "p1s1on";
      Serial.println(sensor1);
      delay(200);
      con1 = 0;
      visitor += 1;
      statusslot -= 1;
    }
  }
  Serial.println(sensor1);
}

void p1slot2()
{
  if (con2 == 0)
  {
    if (digitalRead(parking1_slot2_ir_s) == HIGH)
    {
      sensor2 = "p1s2off";
      delay(200);
      con2 = 1;
      visitor -= 1;
      statusslot += 1;
    }
  }
  if (con2 == 1)
  {
    if (digitalRead(parking1_slot2_ir_s) == LOW)
    {
      sensor2 = "p1s2on";
      delay(200);
      con2 = 0;
      visitor += 1;
      statusslot -= 1;
    }
  }
  Serial.println(sensor2);
}


void p2slot1()
{
  if (con3 == 0)
  {
    if (digitalRead(parking2_slot1_ir_s) == HIGH)
    {
      sensor3 = "p2s1off";
      delay(200);
      con3 = 1;
      visitor -= 1;
      statusslot += 1;
    }
  }
  if (con3 == 1)
  {
    if (digitalRead(parking2_slot1_ir_s) == LOW)
    {
      sensor3 = "p2s1on";
      delay(200);
      con3 = 0;
      visitor += 1;
      statusslot -= 1;
    }
  }
  Serial.println(sensor3);
}


void p2slot2()
{
  if (con4 == 0)
  {
    if (digitalRead(parking2_slot2_ir_s) == HIGH)
    {
      sensor4 = "p2s2off";
      delay(200);
      con4 = 1;
      visitor -= 1;
      statusslot += 1;
    }
  }
  if (con4 == 1)
  {
    if (digitalRead(parking2_slot2_ir_s) == LOW)
    {
      sensor4 = "p2s2on";
      delay(200);
      con4 = 0;
      visitor += 1;
      statusslot -= 1;
    }
  }
  Serial.println(sensor4);
}

void entrance_gate()
{
  if (mySerial.available() > 0)
  {
    String cmd = mySerial.readString();
    cmd.trim();
    if (statusslot > 0 && cmd == "/open") {
      myServo.write(90);
      delay(3000);
      myServo.write(0);
      delay(1500);
    } else {
      myServo.attach(3);
      myServo.write(0);
      lcd.setCursor (0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor (0, 1);
      lcd.print("  Parking Full  ");
      delay (3000);
      lcd.clear();
    }
  }
  lcd.setCursor (0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor (0, 1);
  lcd.print("Slot Left: ");
  lcd.print(statusslot);
}

void exitdoor()
{
  long duration, distance;
  digitalWrite(Trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigpin, LOW);
  duration = pulseIn(Echopin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance <= 5)
  {
    myServo2.write(90);
    delay(1500);
  }
  else
  {
    myServo2.write(0);
    delay(1500);
  }
}
void printdata()
{
  cdata = cdata + sensor1 + "," + sensor2 + "," + sensor3 + "," + sensor4 + ",";
  Serial.println(cdata);
  delay(2000);
  cdata = "";
}
