#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

int xVal = 0;
int yVal = 0;
int zVal = 0;

int leftClick = 8;
int rightClick = 7;

RH_ASK driver(2000, ' ', 4); // ESP8266: do not use pin 11

void setup()
{

    pinMode(leftClick, INPUT);
    pinMode(leftClick, INPUT);
    Serial.begin(9600);	  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    xVal = (int)(analogRead(A1));
    yVal = (int)(analogRead(A2));
    zVal = (int)(analogRead(A3));
    
    Serial.println("Start:");
    Serial.println(xVal);
    Serial.println(yVal);
    Serial.println(zVal);

    if(digitalRead(leftClick))  {

     driver.send("1", 1);
    driver.waitPacketSent();
      
    } else if(digitalRead(rightClick)) {
      
      driver.send("2", 1);
    driver.waitPacketSent();
      
    }
    
    char final_data[10];
    //char* msg [9];
  
    char *data1 = getData(xVal);
    strcpy(final_data, data1);
    free(data1);
    data1 = getData(yVal);
    strcpy(final_data + 3, data1);
    free(data1);
    data1 = getData(zVal);
    strcpy(final_data + 6, data1);
    free(data1);
    final_data[10] = '\0';

    driver.send((uint8_t *)final_data, strlen(final_data));
    driver.waitPacketSent();
    delay(1); 
}

char *getData(int n){
  char* tmp_str = (char *)malloc(4 * sizeof(int));
  sprintf(tmp_str, "%d", n);
  tmp_str = pad(tmp_str);
  tmp_str[3] = '\0';
  return tmp_str;
}

char *pad(char *s){
  char * n = (char *)malloc(4 * sizeof(char));
  if(strlen(s) == 3){
    s[3] = '\0';
    return s;
  }
  else if(strlen(s) == 2){
    strcpy(n + 1,s);
    n[0] = '0';
    n[3] = '\0';
  }
  else if(strlen(s) == 1){
    strcpy(n + 2,s);
    n[0] = '0';
    n[1] = '0';
    n[3] = '\0';
  }
  return n;
}

