int dataOut = 2;

void setup() {
  Serial.begin(9600);
  pinMode(dataOut, OUTPUT);
  // put your setup code here, to run once:

}

void out(int i){
  digitalWrite(dataOut, i);
}

void loop() {
  
  out(1);
  //out(0);
  // put your main code here, to run repeatedly:

}
