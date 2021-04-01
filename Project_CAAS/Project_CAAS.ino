#include <LiquidCrystal_I2C.h>
#include <Servo.h>  
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//wifi ssid dan password
const char* ssid =  "Sergio"; 
const char* pass  = "12345678nz";

//host atau server yang menampung aplikasi web dan database

const char* host = "192.168.43.186";

Servo servo;       
#define trigger1      D5  //trigger1  = trigger untuk portal masuk di pin D5       
#define echo1         D6  //echo1     = echo untuk portal masuk di pin D6
#define trigger2      D4  //trigger2  = trigger untuk parkir kendaraan di pin D4
#define echo2         D8  //echo2     = echo untuk parkir kendaraan di pin D8
#define PIR           D7  //PIR berada di Pin D7

LiquidCrystal_I2C lcd(0x27,D2,D1); //0x27 adalah alamat untuk LCD, D2 pin SDA, D1 pin SCL

void setup() 
{
  
  pinMode(trigger1, OUTPUT);  
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  servo.attach(D0);
  delay(2000);
  servo.write(0);
  pinMode(PIR, INPUT);
  Serial.begin(115200); 
  lcd.begin();
  lcd.backlight();
  
  WiFi.begin(ssid, pass);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  

// Mengatur posisi kursor LCD  (kolom, baris)
  lcd.setCursor(0, 0);
// Menampilkan Text pada LCD
  lcd.print("SMART PARKING");
  lcd.setCursor(0, 1);                       
  lcd.print("Selamat Datang");             
  delay(2000);           
// Menghapus Text pada LCD  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kelompok 2");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Sergio");
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Arief");
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Azmi");
  delay(1000);
  lcd.clear();
}

void loop() 
{

//=============  ULTRASONIC MASUK  =================??

  int durasi, jarak1, jarak2;
  int kondisiPIR = digitalRead(PIR);
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger1, LOW);
  durasi = pulseIn(echo1, HIGH);
  jarak1 = (durasi/2) / 29.1; 
  
  if(jarak1 >= 0 && jarak1 <=7  )
    {
      if(kondisiPIR == HIGH)
      {
       lcd.clear();
      lcd.print("Selamat Datang ");
      lcd.setCursor(0, 1);
      lcd.print("Parkiran Telkom"); 
      servo.write(90);
      Serial.println("pintu terbuka"); 
      Serial.println(jarak1);
      Serial.println(kondisiPIR);
      }
      else
      {
       servo.write(90); 
      }
    }   
  else
    {
      servo.write(0);
    }
  delay(1000);

//==============  ULTRASONIC PARKIR1   ===============??

  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  durasi =pulseIn(echo2, HIGH);
  jarak2= (durasi/2) / 29.1;
  
  if(jarak2 >=0 && jarak2 <=7)
  { 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parkiran Penuh");  
      delay(150);
      Serial.println("Parkiran Penuh");
      
  }
  else 
  {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parkiran Kosong");
      delay(150);
      Serial.println("Parkiran kosong");
  }

  //kirim data ke server

 WiFiClient client ;
 // inisialiasai port web server 80

const int httpPort = 80;
 if(!client.connect(host, httpPort) )
 {
  Serial.println("Connection Failed");
  return;
  }

  //kondisi ketika terkoneksi, krim data sensor ke database atau  web

  String Link ;
  HTTPClient http;

  Link = "http://" + String(host) + "/multisensor/kirimdata.php?jarak1=" + String(jarak1) + "&kondisiPIR=" + String(kondisiPIR) + "&jarak2=" + String(jarak2);
  
  //eksekusi alamat link
  http.begin(Link);
  http.GET();

  //baca respon setelah berhasi kirim nilai sensor

  String respon = http.getString();
  Serial.println(respon);
  http.end();
  
  delay(1000); 
}
