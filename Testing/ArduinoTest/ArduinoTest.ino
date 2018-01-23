int x = A0;
int y = A1;
int z = A2;

double xVal = 0;
double yVal = 0;
double zVal = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {

  xVal = (analogRead(A0)-236.0)*0.02174*9.8;
  yVal = (analogRead(A1)-236.5)*0.02151*9.8;
  zVal = (analogRead(A2)-243.25) * 0.02094 * 9.8;

  if(xVal>9.85)  {
  //  Serial.println("xOn");
  } else if(xVal < 9.75) {
    //    Serial.println("xOff");
  }

  if(yVal>9.85)  {
    //Serial.println("yOn");
  } else if(yVal < 9.75) {
     //   Serial.println("yOff");
  }

  if(zVal>9.85)  {
   // Serial.println("zOn");
  } else if(zVal < 9.75) {
   // Serial.println("zOff");
  }

 // Serial.println(xVal);
  Serial.println(yVal);
  //Serial.println(zVal);
  delay(10);
  
}
