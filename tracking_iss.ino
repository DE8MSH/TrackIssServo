///////////////////////////////////////////////////////
// Project based on https://github.com/jamescoxon/Eurus
///////////////////////////////////////////////////////

#include <Plan13.h>
#include <Time.h>
#include <Servo.h>

#define ONEPPM 1.0e-6
#define DEBUG true
#define TEST true


Plan13 p13;
Servo myservo;

char * elements[2][3] = {
  {
    "METEOR-M 2",
    "1 40069U 14037A   16001.21267721  .00000273  00000-0  14748-3 0  9999",
    "2 40069  98.7309  58.7571 0004764 320.0871  39.9966 14.20623921 76883"
  }
};

void setup () {
  Serial.begin(38400);
  setTime((1451677583));

  p13.setFrequency(137100000, 137100000)
  p13.setLocation(8.8, 53.0, 8);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  myservo.attach(2);
  int pos = 90;
  myservo.write(pos);
  delay (6000);
  for (pos = 90; pos >= 15; pos -= 1) // goes from 0 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15ms for the servo to reach the position
  }
}
void loop() {
  time_t t = now();
  Serial.print(year(t)); Serial.print("."); Serial.print(month(t)); Serial.print("."); Serial.print(day(t)); Serial.print("   "); Serial.print(hour(t)); Serial.print(":"); Serial.print(minute(t)); Serial.print("."); Serial.println(second(t));
  p13.setTime(year(t), month(t), day(t), hour(t), minute(t), second(t));

  //ISS
  readElements(0);
  p13.calculate(); //crunch the numbers
  p13.printdata();

  Serial.println();

  if (p13.EL >= 15) {
    myservo.write((int)p13.EL);
    digitalWrite(A0, HIGH);
  }
  else
  {
    digitalWrite(A0, LOW);
  }

  Serial.println((int)p13.EL);
  delay(1000);
}

double getElement(char *gstr, int gstart, int gstop)
{
  double retval;
  int    k, glength;
  char   gestr[80];

  glength = gstop - gstart + 1;

  for (k = 0; k <= glength; k++)
  {
    gestr[k] = gstr[gstart + k - 1];
  }

  gestr[glength] = '\0';
  retval = atof(gestr);
  return (retval);
}

void readElements(int x)
{
  p13.setElements(getElement(elements[x][1], 19, 20) + 2000, getElement(elements[x][1], 21, 32), getElement(elements[x][2], 9, 16),
                  getElement(elements[x][2], 18, 25), getElement(elements[x][2], 27, 33) * 1.0e-7, getElement(elements[x][2], 35, 42), getElement(elements[x][2], 44, 51), getElement(elements[x][2], 53, 63),
                  getElement(elements[x][1], 34, 43), (getElement(elements[x][2], 64, 68) + ONEPPM), 0);
}


