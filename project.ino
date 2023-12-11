#include <SoftwareSerial.h>
// include the library code:
int m;

int n;
int tp;
char buf1[16];
char buf2[16];
char buf3[16];
char buf4[16];
int s=0;
int s1=0;
long t=0;
long t1=0;
long t3=0;
String strt1;
String strt2;
String strt3;
String strt4;
// replace with your channel's thingspeak API key
String apiKey = "PQVB3AN7RE8NKRQZ";
#include <LiquidCrystal.h>
long t2=0;
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


SoftwareSerial ser(5, 6); // RX, TX
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;   
int sensorPin1 = A1;
float volt;



// this runs once
void setup() {     
   pinMode(LED,OUTPUT);             
  // initialize the digital pin as an output.
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
   pinMode(A4, OUTPUT);
   digitalWrite(A4, LOW);
analogReference(DEFAULT);
  // enable debug serial
  Serial.begin(9600);
  // enable software serial
  ser.begin(115200);
  // reset ESP8266
  ser.println("AT+RST");
   delay(500);
  ser.println("AT+CWMODE=3");
   delay(500);
  ser.println("AT+CWJAP=\"project\",\"12345678\"");//to change wifi
  delay(500);
  
 pinMode(2, OUTPUT);
 digitalWrite(2, HIGH);
  
}


// the loop
void loop() {
  

static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 1.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
 // Serial.print("Voltage:");
      //  Serial.print(voltage,2);
     //   Serial.print("    pH value: ");
 // Serial.println(pHValue,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
  }

        int sensorValue = analogRead(sensorPin1);
        int turbidity = map(sensorValue, 0,740, 150, 0);
     
  tp=analogRead(A2)*0.49;
   lcd.clear();
  
   lcd.setCursor(0,0);
   lcd.print("T:");
 
   lcd.print(tp);
    lcd.print(" C ");
   lcd.setCursor(8,0);
   lcd.print("pH:");

   lcd.print(pHValue,2);
   lcd.print("    ");
     lcd.setCursor(0,1);
    lcd.print("Turb:");
    lcd.print(turbidity);
    lcd.print("mg/L ");
if(turbidity<25){
  lcd.print("Low");
  }else if (turbidity<100)
  {lcd.print("Med.");}
  else if(turbidity<150)
  {lcd.print("High");}
    delay(10);
   delay(200);
  //pHValue= random(7,8);
n = pHValue*100; //ph
if(pHValue>8 || pHValue<6 || turbidity>70 || tp>32||tp<25)
{
  digitalWrite(A4, HIGH);
  delay(500);
  digitalWrite(A4, LOW);
}
if(turbidity>70 || tp>32||tp<25)
{
  digitalWrite(2, LOW); //relay
}
else
{
  digitalWrite(2, HIGH);
}
 
strt2 = dtostrf(n, 4, 1, buf2);
strt3 = dtostrf(tp, 4, 1, buf3);
strt4 = dtostrf(turbidity*100, 4, 1, buf4);
// convert to string

  
  Serial.print(strt2);
  Serial.print(" ");
   Serial.print(strt3);
  Serial.print(" ");
  Serial.print(strt4);
  Serial.println(" ");
    
  
  if(millis() - t1>6000)
  {
    t1=millis();
  // TCP connection
  String cmd1 = "AT+CIPSTART=\"TCP\",\"";
  cmd1 += "184.106.153.149"; // api.thingspeak.com
  cmd1 += "\",80";
  ser.println(cmd1);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr1 = "GET /update?api_key=";
  getStr1 += apiKey;
  getStr1 +="&field1=";
  getStr1 += String(strt2);
  getStr1 += "\r\n\r\n";



  // send data length
  cmd1 = "AT+CIPSEND=";
  cmd1 += String(getStr1.length());
  ser.println(cmd1);

   

  if(ser.find(">")){
    ser.print(getStr1);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  // thingspeak needs 15 sec delay between updates
 // delay(6000);
  }

  if(millis() - t2>3000)
  {
    t2=millis();
  // TCP connection
  String cmd2 = "AT+CIPSTART=\"TCP\",\"";
  cmd2 += "184.106.153.149"; // api.thingspeak.com
  cmd2 += "\",80";
  ser.println(cmd2);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr2 = "GET /update?api_key=";
  getStr2 += apiKey;
  getStr2 +="&field2=";
  getStr2 += String(strt3);
  getStr2 += "\r\n\r\n";



  // send data length
  cmd2 = "AT+CIPSEND=";
  cmd2 += String(getStr2.length());
  ser.println(cmd2);

   

  if(ser.find(">")){
    ser.print(getStr2);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  // thingspeak needs 15 sec delay between updates
 // delay(6000);
  }
   if(millis() - t3>2000)
  {
    t3=millis();
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field3=";
  getStr += String(strt4);
  getStr += "\r\n\r\n";



  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

   

  if(ser.find(">")){
    ser.print(getStr);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  // thingspeak needs 15 sec delay between updates
 // delay(6000);
  }
}

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
    bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
    for (i = 0; i < iFilterLen - j - 1; i++) 
          {
      if (bTab[i] > bTab[i + 1]) 
            {
    bTemp = bTab[i];
          bTab[i] = bTab[i + 1];
    bTab[i + 1] = bTemp;
       }
    }
      }
      if ((iFilterLen & 1) > 0)
  bTemp = bTab[(iFilterLen - 1) / 2];
      else
  bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
