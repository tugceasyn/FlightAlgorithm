

//------------------------------TANIMLAMA------------------------------
  #include <Wire.h> 
  #include <SPI.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BMP280.h>
  #include <TinyGPS++.h>
  #include <SoftwareSerial.h>
  
//*********************************************************************

  static const int RXPin = 13, TXPin = 10;

  
  static const uint32_t GPSBaud = 9600;
  TinyGPSPlus gps;
 SoftwareSerial ss(RXPin, TXPin);//SERİ HABERLEŞME ATAMASI
 
//------------------------------ATAMALAR------------------------------

  Adafruit_BMP280 bme;
  int rakim ;
  int ADXL345 = 0x53;
  float X_out, Y_out, Z_out;
  bool total=false;
  int in1 = 3; //burun
  int in2 = 5;//gövde
  int in3 = 6; // kutu
  #define buzzer A2
  unsigned long zaman = 0 ;
  int syc =0;
  int syc2 = 0;
 
  float a,b,c,veri1,veri2,veri3,veri4,veri5,veri6,
  veri7,veri8,veri9,veri10,yukseklik;
  
//************************************************************************
 
  
void setup() {
 
  Serial.begin(9600);  
  bme.begin(); 
  Wire.begin(); 
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); 
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
  ss.begin(GPSBaud);  
  pinMode (in1, OUTPUT);        
  pinMode (in2, OUTPUT);
  pinMode (in3, OUTPUT);
  pinMode(buzzer,OUTPUT);
   
  rakim =   (bme.readAltitude (1013.25));  
  
}

void loop() {
  
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
  
 //---------BUZZER------------
  //digitalWrite(buzzer,HIGH);
  //delay(300);
  //digitalWrite(buzzer,LOW);
  //delay(300);
 //****************************


              
  //--------------EKSEN------------------------
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); 
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); 
  X_out = ( Wire.read()| Wire.read() << 8); 
  X_out = X_out/2.56; 
  Y_out = ( Wire.read()| Wire.read() << 8); 
  Y_out = Y_out/2.56;
  Z_out = ( Wire.read()| Wire.read() << 8); 
  Z_out = Z_out/2.56;
 //**********************************************


 //------------------------------TANIMLAMA BÖLÜMÜ------------------------------
  veri1  = (X_out);                        // veri1 = X EKSENİ 
  veri2  = (Y_out);                        // veri2 = Y EKSENİ
  veri3  = (Z_out);                        // veri3 = Z EKSENİ
  veri4  = (bme.readTemperature ());       // veri4 = SICAKLIK
  veri5  = (bme.readPressure ());          // veri5 = BASINÇ
  veri6  = (bme.readAltitude (1013.25));   // veri6 = YÜKSEKLİK
  veri8  = (gps.location.lat());           // veri8 = ENLEM
  veri9  = (gps.location.lng());           // veri9 = BOYLAM
  veri10 = (gps.speed.kmph());             // veri10 = GPS HIZ KM/S 
 //*****************************************************************************


 //------------------------------EKRANA YAZDIRMA BÖLÜMÜ------------------------------
  //Serial.print("SICAKLIK        : ");
  //Serial.println(veri4);
  //Serial.print("BASINÇ          : ");
  //Serial.println(veri5);
  Serial.print("");
  Serial.print("*"); 
  Serial.print((veri6)-rakim); 
  Serial.print("*"); 
  Serial.print(veri10); 
  Serial.print("*"); 
  Serial.print((int)X_out-108); 
  Serial.print("*");
  Serial.print((int)Y_out+3);
  Serial.print("*");
  Serial.print((int)Z_out+24);
  Serial.print("*");  
  Serial.print((veri8), 6);
  Serial.print("*");
  Serial.print((veri9), 6);
  Serial.print("*");
  Serial.println();
  delay(1000);
 //***********************************************************************************                            
                              
 
                                  
//---------------------------------KOŞUL BÖLÜMÜ--------------------------------------                                 
if((veri6-rakim>50))

  {
  yukseklik = (bme.readAltitude (1013.25)); 
  a=yukseklik-rakim;
  syc=syc+1;
  delay(300);
  }
 
  if(syc==1)
  
    {
    yukseklik = (bme.readAltitude (1013.25)); 
    b=yukseklik-rakim;
    syc=syc+1;
    delay(300);
    }
    
    if(syc==2)
      
    {
    yukseklik = (bme.readAltitude (1013.25)); 
    c=yukseklik-rakim;
    syc=0;
    delay(300);
    }
//**********************************************************************************

//------------------------------İKİNCİ AYRILMA VE KUTU------------------------------
if(a>c)
{
   syc2 = syc2 + 1;
}
else{
   syc2 = 0;
}
if(syc2 >= 3){
   total=true;       
   digitalWrite(in1, HIGH);
   Serial.println("BURUN KONISI ACILDI");
}
       
if(total==true && yukseklik-rakim<100)
      
{
   digitalWrite(in2,HIGH);
   Serial.println("IKINCI AYRILMA GERCEKLESTI"); 
}
       
if(total==true && yukseklik-rakim<70)
      
{
   digitalWrite(in3,HIGH);
   Serial.println("ANA PARASUT ACILDI GERCEKLESTI"); 
}    
//**********************************************************************************


}
