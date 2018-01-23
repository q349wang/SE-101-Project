// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include<Mouse.h>

//RH_ASK driver;
RH_ASK driver(2000, 4); // ESP8266: do not use pin 11

bool lClicked = false; // Checks if left clicked
bool rClicked = false; // Checks if right clicked

int x = 0;
int y = 0;
int z = 0;

// Values of acceleration of x y z axis in m/s^2
double xVal = 0;
double yVal = 0;
double zVal = 0;

// Value of xy vector
double xyVal;

// Low and high thresholds of acceleration
double lBound = 1.7;
double hBound = 13.5;

// Speed Scaler
double lowSpeed = 0.0;
double hiSpeed = 9.0;

//Scaled x and y values to use on mouse
double init_x = 0;
double init_y = 0;

//const bool leftHanded = false;
void setup()
{
  //Serial.begin(9600);  // Debugging only
  if (!driver.init()) {}
  //Serial.println("init failed");
}

void loop()
{
  /**
     Convert string data into x,y and z values
  */
  String dataStr = getData();
  if (!dataStr.equals("NULL")) {

    if (dataStr.equals("1") && !lClicked)  {
      lClicked = true;
      Mouse.press(MOUSE_LEFT);

    } else if (dataStr.equals("2") && !rClicked) {
      rClicked = true;
      Mouse.press(MOUSE_RIGHT);

    } else {

      if (lClicked)  {
        lClicked = false;
        Mouse.release(MOUSE_LEFT);
      }

      if (rClicked)  {
        rClicked = false;
        Mouse.release(MOUSE_RIGHT);

      }
      x = dataStr.substring(0, 3).toInt();
      y = dataStr.substring(3, 6).toInt();
      z = dataStr.substring(6, 9).toInt();
    }
  }
  /****************Value Conversions**********************/
  //Conversion to g
  xVal = -(x - 350.0) * 0.014286 * 9.8;
  yVal = -(y - 350.0) * 0.014286 * 9.8;
  zVal = (z - 360.0) * 0.014286 * 9.8;
/*
  Serial.println(x);
  Serial.println(y);
*/
  /* Serial.println(xyVal);

    Serial.print("hBound: ");
    Serial.println(hBound);
    Serial.print("lBound: ");
    Serial.println(lBound);*/



  // Scales xVal and yVal to init_x and init_y respectively if they are between the bounds, else sets init_x and init_y to 0
  if (xVal > -hBound && xVal < -lBound)  {

    init_x = -scale(-hBound, -lBound, xVal, 1.0, 10);
  } else if (xVal > lBound && xVal < hBound) {

    init_x = scale(lBound, hBound, xVal, 1.0, 10);
  } else init_x = 0;

  if (yVal > -hBound && yVal < -lBound)  {

    init_y = scale(-hBound, -lBound, yVal, 1.0, 10);
  } else if (yVal > lBound && yVal < hBound) {

    init_y = -scale(lBound, hBound, yVal, 1.0, 10);
  } else init_y = 0;

  // Adds init_x, int_y together to make xyVal
  xyVal = sqrt(pow(init_x, 2) + pow(init_y, 2));

  /*Serial.print("x: ");
    Serial.println(init_x);
    Serial.print("y: ");
    Serial.println(init_y);

    Serial.print("xyVal: ");
    Serial.println(xyVal);*/

  // Converts init_x, init_y into a unit vector then scales it to desired value
  if (fabs(xyVal) >= 0.000001) {
    init_x /= xyVal;
    init_y /= xyVal;

    xyVal = scale(0.0, 8.0, xyVal, lowSpeed, hiSpeed);

    init_x *= xyVal;
    init_y *= xyVal;
  }

  /*Serial.print("xyVal: ");
    Serial.println(xyVal);

    Serial.print("x: ");
    Serial.println(init_x);
    Serial.print("y: ");
    Serial.println(init_y);*/

  // Moves mouse
  Mouse.move(init_y, init_x, 0);
  /*
      Serial.print("xVal: ");
      Serial.println(xVal);
      Serial.print("yVal: ");
      Serial.println(yVal);
      Serial.print("zVal: ");
      Serial.println(zVal);
  */
  delay(12);
}

double scale(double iLow, double iHigh, double num, double fLow, double fHigh) { // Scales a number between an initial range to final range
  double iMid = (iLow + iHigh) / 2; //Initial midpoint

  double deviation = (num - iMid) / fabs(iMid - iLow); // Finds how far the number deviates from the midpoint as a ratio of this to the maximum deviation
  double fMid = (fLow + fHigh) / 2; // Final Midpoint
  if (iMid >= 0)  {
    return  (fMid + deviation * (fabs(fMid - fLow))); // Returns the new number as the final midpoint times the deviation
  } else return (fMid - deviation * (fabs(fMid - fLow))); // Returns the new number as the final midpoint times the deviation

}

String getData() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    String str = "";
    for (int i = 0; i < buflen; i++) {
      str += (char) buf[i];
    }
    return str;
    // Message with a good checksum received, dump it.
    //  driver.printBuffer("Got:", buf, buflen);
  }
  return "NULL";
}

