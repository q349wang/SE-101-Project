#include <Mouse.h>


int x = A0;
int y = A1;
int z = A2;

double xVal = 0;
double yVal = 0;
double zVal = 0;

double xyVal;

double lBound = 2.0;
double hBound = 10.0;

double init_x = 0;
double init_y = 0;

double scale(double iLow, double iHigh, double num, double fLow, double fHigh) { // Scales a number between an initial range to final range
  double iMid = (iLow + iHigh) / 2; //Initial midpoint

  double deviation = (num - iMid) / fabs(iMid - iLow); // Finds how far the number deviates from the midpoint as a ratio of this to the maximum deviation
  double fMid = (fLow + fHigh) / 2;

  return  (fMid + deviation * (fabs(fMid - fLow)));

}

void setup() {

  Serial.begin(9600);
  Mouse.begin();

}

void loop() {


  xVal = (analogRead(A0) - 359.0) * 0.0142857 * 9.8;
  yVal = (analogRead(A1) - 359.75) * 0.01413427 * 9.8;
  zVal = (analogRead(A2) - 371.5) * 0.013605 * 9.8;

  if (xVal > -hBound && xVal < -lBound)  {


    init_x = -scale(-hBound, -lBound, xVal, 1, 10);
  } else if (xVal > lBound && xVal < hBound) {
    init_x = scale(lBound, hBound, xVal, 1, 10);
  } else init_x = 0;

  if (yVal > -hBound && yVal < -lBound)  {

    init_y = scale(-hBound, -lBound, yVal, 1, 10);
  } else if (yVal > lBound && yVal < hBound) {

    init_y = -scale(lBound, hBound, yVal, 1, 10);
  } else init_y = 0;
  if ((zVal > 15.00)&&(xVal < lBound && xVal > -lBound && yVal < lBound && yVal > -lBound))   {

    Mouse.click();
    
  } else if (zVal < 9.75) {
    // Serial.println("zOff");
  }
  xyVal= sqrt(pow(init_x,2)+pow(init_y,2));
  init_x /= xyVal;
  init_x *= 2;
  init_y /= xyVal;
  init_y *= 2;

  Mouse.move(init_x, init_y, 0);

  delay(1);
}
