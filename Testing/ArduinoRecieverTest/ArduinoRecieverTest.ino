#define rfReceivePin A0  //RF Receiver pin = Analog pin 0
 #define ledPin 13        //Onboard LED = digital pin 13

 unsigned int data = 0;   // variable used to store received data
 const unsigned int upperThreshold = 70;  //upper threshold value
 const unsigned int lowerThreshold = 50;  //lower threshold value

 void setup(){
   pinMode(rfReceivePin, INPUT);
   Serial.begin(9600);
 }

 void loop(){
   data = analogRead(rfReceivePin);    //listen for data on Analog pin 0
   int count = 1;
/*   for(int i = 0; i < 500; i++) {
      count++;
      data +=digitalRead(rfReceivePin);
   }*/

   //data= round(data/count);
   
    if(data>upperThreshold){
     //analogRead(rfReceivePin);   //If a LOW signal is received, turn LED OFF
     Serial.println("OFF");
   }else if(data<lowerThreshold){
     //analogRead(rfReceivePin);   //If a HIGH signal is received, turn LED ON
     Serial.println("ON");
   }
 }
