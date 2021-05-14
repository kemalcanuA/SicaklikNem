#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <dht11.h>
#define RX 10
#define TX 11
#define dht_apin 2 // DHT11 analog pine baglı
LiquidCrystal_I2C lcd(0x27,16,2);
dht11 dhtObject;
String AP = "Superbox_WiFi_5200";       // İnternetin adı
String PASS = "HTLA0338FF1"; // internetin sifresi
String API = "E2EFZWW5FMS62GYP";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
int temp;
int humidity;
  
SoftwareSerial esp8266(RX,TX); 
  
void setup() {
  Serial.begin(9600);                                                   // seri haberleşme
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");                                 // At komutunu baslatan komut
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
    Serial.println("Temperature and Humidity Sensor Test");
    //dht.begin();
    lcd.begin(); //initialize the lcd
    lcd.backlight(); //open the backlight
}

void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue();        //sıcaklık ve nemin siteyi veri olarak çekilmesini sağlayan döngü
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 Show(temp,humidity);
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

}
void Show(int temp,int hum){
  lcd.clear();
  lcd.setCursor(0, 0);
    lcd.println(" Now Temperature ");
    
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(temp);
    lcd.print("C");

    lcd.setCursor(11, 1);
    lcd.print("H:");
    lcd.print(hum);
    lcd.print("%");
    
  delay(1000); //Delay 1 sec. 
  }

String getTemperatureValue(){

   dhtObject.read(dht_apin);                        // Sıcaklık değeri (C)
   Serial.print(" Temperature(C)= ");
   temp = dhtObject.temperature;
   Serial.println(temp); 
   delay(50);
   return String(temp); 
  
}


String getHumidityValue(){

   dhtObject.read(dht_apin);                // Nem değeri (%)
   Serial.print(" Humidity in %= ");
   humidity = dhtObject.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity); 
  
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);      //at+cipsend
    if(esp8266.find(readReplay))  //ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("(Basarılı)");      // Basarılı veri çekme işlemi için;
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("(Basarısız)");   // Basarısız veri çekme işlemi için;
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
 
