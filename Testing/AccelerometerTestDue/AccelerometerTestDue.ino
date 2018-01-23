#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

//RH_ASK driver;
 RH_ASK driver(2000, 4); // ESP8266: do not use pin 11

#include <Mouse.h>

// Input Pins
int x = A0;
int y = A1;
int z = A2;

bool clicked = false; // Checks if clicked

// Values of acceleration of x y z axis in m/s^2
double xVal = 0;
double yVal = 0;
double zVal = 0;

// Value of xy vector
double xyVal;

// Low and high thresholds of acceleration
double lBound = 2.5;
double hBound = 10.0;

//Scaled x and y values to use on mouse
double init_x = 0;
double init_y = 0;

double scale(double iLow, double iHigh, double num, double fLow, double fHigh) { // Scales a number between an initial range to final range
  double iMid = (iLow + iHigh) / 2; //Initial midpoint

  double deviation = (num - iMid) / fabs(iMid - iLow); // Finds how far the number deviates from the midpoint as a ratio of this to the maximum deviation
  double fMid = (fLow + fHigh) / 2; // Final Midpoint
if(iMid >=0)  {
  return  (fMid + deviation * (fabs(fMid - fLow))); // Returns the new number as the final midpoint times the deviation
} else return (fMid - deviation * (fabs(fMid - fLow))); // Returns the new number as the final midpoint times the deviation

}

void setup() {

  //Serial.begin(9600);
  driver.init();
  Mouse.begin();

}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    String str = "";
    
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      for(int i = 0; i<buflen; i++) {
        str += (char) buf[i];

    
      }
// Gets raw analog input and converts it into an accleration
  xVal = (analogRead(A0) - 359.0) * 0.0142857 * 9.8;
  yVal = (analogRead(A1) - 359.75) * 0.01413427 * 9.8;
  zVal = (analogRead(A2) - 371.5) * 0.013605 * 9.8;

  String data = "%03d", xVal;
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
  if ((zVal > 15.00) && (xVal < lBound && xVal > -lBound && yVal < lBound && yVal > -lBound))   {

    if (!clicked) {
      Mouse.click();
      clicked = true;
    }

  } else if (zVal < 10.0) {

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
      }
  delay(12);
}
