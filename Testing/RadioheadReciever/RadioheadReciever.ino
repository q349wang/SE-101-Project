#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include<Mouse.h>

//RH_ASK driver;
RH_ASK driver(2000, 4); // ESP8266: do not use pin 11

bool clicked = false; // Checks if clicked

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
double lBound = 2.75;
double hBound = 13.5;

//Scaled x and y values to use on mouse
double init_x = 0;
double init_y = 0;


void setup()
{
  Serial.begin(9600);  // Debugging only
  if (!driver.init()){}
   //Serial.println("init failed");
}

void loop()
{
  /**
     Convert string data into x,y and z values
  */
  String dataStr = getData();
  if (!dataStr.equals("NULL")) {
    x = dataStr.substring(0, 3).toInt();
    y = dataStr.substring(3, 6).toInt();
    z = dataStr.substring(6, 9).toInt();
  }
  /****************Value Conversions**********************/
    //Conversion to g
    xVal = (x - 244.0) * 0.021739 * 9.8;
    yVal = (y - 245.5) * 0.021053 * 9.8;
    zVal = (z - 253.5) * 0.020202 * 9.8;


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


    // Clicks mouse if zVal is above threshold of 15 and mouse is not moving
    if ((zVal > 13.50))   {

      if (!clicked) {
        Mouse.click();
        clicked = true;
      }

    } else if (zVal < 6.0) {

      clicked = false;
      // Serial.println("zOff");
    }

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

      xyVal = scale(0.0, 8.0, xyVal, 1, 6);

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
    Mouse.move(init_x, init_y, 0);
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

