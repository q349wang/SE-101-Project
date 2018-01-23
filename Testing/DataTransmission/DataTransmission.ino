 #define rfTransmitPin 4  //RF Transmitter pin = digital pin 4
 #define ledPin 13        //Onboard LED = digital pin 13

 void setup(){
  Serial.begin(9600);
   pinMode(rfTransmitPin, OUTPUT);     
   //pinMode(ledPin, OUTPUT);    
 }

 void loop(){
   for(int i=4000; i>5; i=i-(i/3)){
     digitalWrite(rfTransmitPin, HIGH);     //Transmit a HIGH signal
     Serial.println("1");           //Turn the LED on
     delay(2000);                           //Wait for 1 second
     
     digitalWrite(rfTransmitPin,LOW);      //Transmit a LOW signal
     Serial.println("0");    
    // digitalWrite(ledPin, LOW);            //Turn the LED off
     delay(i);                            //Variable delay
   }
 }

