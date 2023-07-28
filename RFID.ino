#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         48
#define SS_PIN          53

int UID[4], i;

int ID1[4] = {66, 9, 127, 76}; 
int ID2[4] = {209, 9, 21, 25}; 
int ID3[4] = {227, 145, 15, 150}; 
int ID4[4] = {243, 93, 249, 149};
MFRC522 mfrc522(SS_PIN, RST_PIN);
int relay1 = 41; //relay dieu khien quat
int led = 39;
int servo3 = 37;//servo mo cua so ben trai
int lamp = 35; //relay dieu khien den 220V
#include <Servo.h> 
int servo = 49; //sevro mo cua
int servo2 = 43;//servo mo cua so ben phai
Servo sv3;
Servo sv2;
int dem = 0; //bien dem cho quat
int dem2 = 0; //bien dem cho cua so
int dem3 = 0; //bien dem cho den 220V
int goc; //khai bao bien goc
Servo myServo;

#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
int coi = 47;
int mode = 0; 


#include <Keypad.h>
int so_dung = 0;
bool e;
char* password = "54321";
char password_nhap[] = {'0','0','0','0','0'};


const byte numRows= 4;       
const byte numCols= 4;          
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[numRows] = { 11,10,9,8};// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[numCols] = { 7,6,5,4};// Connect keypad CO COL2 to these Arduino pins.

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

int alarm = 0;
uint8_t alarmStat = 0;
uint8_t maxError  = 4;
 
void setup() 
{
    sv2.write(130);
    sv3.write(45);
    Serial.begin(9600);   
    myServo.attach(servo);
    sv2.attach(servo2);
    sv3.attach(servo3);    
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    
    SPI.begin();    
    mfrc522.PCD_Init();
    myServo.write(10);
    pinMode (relay1, OUTPUT);
    digitalWrite(relay1,LOW);
    pinMode (lamp, OUTPUT);
    digitalWrite(lamp,LOW);

    lcd.init(); 
    lcd.backlight(); 
    lcd.setCursor(0, 0); 
    lcd.print("    NGOI NHA    "); 
    lcd.setCursor (0,1);
    lcd.print ("    HIEN DAI   ");
    pinMode(coi, OUTPUT);
    tone(coi, 1200);
    delay(1000);
    noTone(coi);
    delay(3000); // chờ 3giay
    lcd.clear(); //Xóa LCD
    lcd.setCursor (0,0);
    lcd.print ("   DA SAN SANG   ");

   



}
void setLocked(int locked){ 
  if(locked){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  MAT KHAU DUNG ");
    digitalWrite(led,LOW);
    lcd.setCursor(0,1);
    lcd.print("   XIN MOI VAO ");
    myServo.write(90);// servo mo cua
    delay(2000);//2 giay

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("TU DONG DONG SAU");
    lcd.setCursor(0,1);
    lcd.print ("                ");
    for(int i=5; i>0; i--){ 
      lcd.setCursor (7,1);
      lcd.print(i);
      delay(1000);
    }
    myServo.write(10); //dong cua
    mode = 0;
    for(int i = 0; i < 5; i++) //gan mang led ve 00000 (nhap mat khau lai)
    { password_nhap[i] = '0';}
        lcd.setCursor(0, 0); 
    lcd.print("    NGOI NHA    "); 
    lcd.setCursor (0,1);
    lcd.print ("    HIEN DAI   ");
    delay(3000); // chờ 3giay
    lcd.clear(); //Xóa LCD
    lcd.setCursor (0,0);
    lcd.print ("   DA SAN SANG   ");
  }
  else{
    digitalWrite(led,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  MAT KHAU SAI");
    tone(coi,1500);
    delay(3000);
    noTone(coi);

    for (int i=0; i<5 ; i++) //gan mang led ve 0000 (nhap mat khau lai)
    { password_nhap[i] = '0'; }
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("MOI NHAP LAI MK");

  }}
void loop() 
{
    //nhap mat khau
  char whichKey = myKeypad.getKey();
  if (whichKey) //neu co phim nhan
  {
    tone(coi,1500);
    delay(100);
    noTone(coi);
    Serial.println(whichKey);
    delay(100);
    e = true;
        if(whichKey != '*' && whichKey != '#') //neu nhu phim * va phim # duoc nhan
        {
          //dich phim
          //vong lap chay tu 4 ve 0
          for (int i = 4; i>0; i--) 
          { password_nhap[i] = password_nhap[i-1];  }
          password_nhap[0] = whichKey;
          //lcd.clear();
          //lcd.setCursor (0,0);
          //lcd.print("                ");
          for(unsigned int i=0; i<5; i++)
          {
            lcd.setCursor(4-i+6, 1); //hang 1, neu i = 0 la cot thu 10
            lcd.print(password_nhap[i]);
          }
        }
        if (whichKey == '*'){ // nhan phim *: BUOC VAO CHE DO DAT MAT KHAU

          mode ++;
          mode %=2; //mode = 2 thi reset
          lcd.clear();
          lcd.setCursor(0,0); //cot 0, hang 0
          lcd.print("  NHAP MAT KHAU  ");
        }
        if (whichKey == 'A') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("     MO QUAT   ");
          digitalWrite(relay1,HIGH);
          Serial.println("MO QUAT BANG NUT NHAN");}

        if (whichKey == 'B') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("     TAT QUAT   ");
          digitalWrite(relay1,LOW);
          Serial.println("TAT QUAT BANG NUT NHAN");}

        if (whichKey == 'C') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("    MO CUA SO ");
          sv2.write(0);
          goc = sv2.read();
          Serial.print("Góc hiện tại: "); Serial.println(goc);
          sv3.write(170);
          goc = sv3.read();
          Serial.print("Góc hiện tại: "); Serial.println(goc);
      }


        if (whichKey == 'D') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  DONG CUA SO  ");
          sv2.write(130);
          goc = sv2.read();
          Serial.print("Góc hiện tại: "); Serial.println(goc);
          sv3.write(45);
          goc = sv3.read();
          Serial.print("Góc hiện tại: "); Serial.println(goc);
      }
          
        if (whichKey == '#') // nhan OK thi chot mat khau
       {
        if (e = true){
          //kiem tra password voi so nhap

          for(int i=0; i<5; i++)
          {
            if (password_nhap[i] == password [4-i]) {so_dung++;}
          }

          //neu nhap dung
          if(so_dung ==5)
          {
            e = false;
            setLocked (true);
            so_dung = 0;
          }
          else //neu nhap sai
          {

            e = false;
            setLocked (false);
            so_dung = 0;
          }
            }
          }
         }


  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 
    return; 
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {  
    return;  
  }
  
  Serial.print("UID của thẻ: ");   
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");   
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
  }

  Serial.println("   ");
  
  if (UID[i] == ID1[i])
  {
    tone(coi,1000);
    delay(100);
    noTone(coi);
    Serial.println("XIN CHAO BAN!");
    lcd.setCursor (0,0);
    lcd.print ("   THE HOP LE    ");
    lcd.setCursor (0,1); 
    lcd.print ("   MOI BAN VAO    ");
    myServo.write(90);
    goc = myServo.read();
    Serial.print("Góc hiện tại: "); Serial.println(goc);
    delay(5000);
    lcd.clear(); //Xóa LCD     
    lcd.setCursor (0,0);
    lcd.print ("TU DONG DONG SAU");
    lcd.setCursor (0,1);
    lcd.print ("                ");
    for(int i=5; i>0; i--){ //dem nguoc trong vong 5 giay
        lcd.setCursor (7,1); lcd.print(i);
        delay(1000);
        }
    myServo.write(10); //sau 5 giay se dong cua (servo quay lai 1 goc 25 do)
    goc = myServo.read();
    Serial.print("Góc hiện tại: "); Serial.println(goc);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    NGOI NHA    ");
    lcd.setCursor (0,1);
    lcd.print ("    HIEN DAI   ");
    delay(3000); // chờ 3giay
    lcd.clear(); //Xóa LCD
    lcd.setCursor (0,0);
    lcd.print ("   DA SAN SANG   ");
  }
  else  if (UID[i] == ID2[i]){
      tone(coi,1000);
      delay(100);
      noTone(coi);
      delay(100);
      dem2 ++;
      Serial.print("Biến Đếm: ");
      Serial.println(dem2);
    
      if ( (dem2 % 2) == 1) //Số lẻ MO CUA SO
      {
      Serial.println("MO CUA SO");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1); 
      lcd.print ("   MO CUA SO   ");
      sv2.write(0);
      goc = sv2.read();
      Serial.print("Góc hiện tại: "); Serial.println(goc); 
      sv3.write(170);
      goc = sv3.read();
      Serial.print("Góc hiện tại: "); Serial.println(goc); 
      }
      else
      {
      delay(100);
      Serial.println("DONG CUA SO");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1);
      lcd.print ("  DONG CUA SO   ");
      sv2.write(130);
      goc = sv2.read();
      Serial.print("Góc hiện tại: "); Serial.println(goc);
      sv3.write(45);
      goc = sv3.read();
      Serial.print("Góc hiện tại: "); Serial.println(goc);          
      }
      }
   else if (UID[i] == ID3[i]){
    tone(coi,1000);
    delay(100);
    noTone(coi);
    dem ++;
    Serial.print("Biến Đếm: ");
    Serial.println(dem);
    
      if ( (dem % 2) == 1) 
      {
      Serial.println("XIN CHAO BAN!");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1); 
      lcd.print("     MO QUAT   ");
      digitalWrite(relay1,HIGH);
      Serial.println("MO QUAT");

      }
      else
      {
      Serial.println("XIN CHAO BAN!");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1); 
      lcd.print("     TAT QUAT   ");
      digitalWrite(relay1,LOW);
      Serial.println("TAT QUAT");
      }
  }
    else  if (UID[i] == ID4[i]){
      tone(coi,1000);
      delay(100);
      noTone(coi);
      delay(100);
      dem3 ++;
      Serial.print("Biến Đếm: ");
      Serial.println(dem3);
    
      if ( (dem3 % 2) == 1) 
      {
      Serial.println("MO DEN 220V");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1); 
      lcd.print ("   MO DEN 220V   ");
      digitalWrite(lamp,HIGH);
      }
      else
      {
      delay(100);
      Serial.println("TAT DEN 220V");
      lcd.setCursor (0,0);
      lcd.print ("   THE HOP LE    ");
      lcd.setCursor (0,1);
      lcd.print ("  TAT DEN 220V   ");
      digitalWrite(lamp,LOW);  
      }
      }
  else
  {
    Serial.println("Sai thẻ");
    alarm = alarm + 1; //moi lan quet the sai bien "alarm" se tang len 1 GT
    delay(100);
    tone(coi,1200);
    delay(100);
    noTone(coi);
    lcd.clear();
    lcd.setCursor (0,1);
    lcd.print ("   MA THE SAI ");
    delay(1000);
    lcd.clear();
    if (alarm >=  maxError)
    { 
       alarmStat = 1;
    digitalWrite(led,HIGH);
    lcd.setCursor (0,0);
    lcd.print ("HE THONG BI KHOA");
    lcd.setCursor (0,1);
    lcd.print (" XIN DOI SAU ");
    tone(coi,1200);
    delay(4000);
    noTone(coi);
    for(int i=10; i>0; i--){ //dem nguoc trong vong 10s, trong 10s do buzzer keu lien tuc
        lcd.setCursor (13,1); lcd.print(i);
        lcd.print (F("  ")); delay (1000);

        }
    digitalWrite(led,LOW);
    lcd.setCursor(0, 0); 
    lcd.print("    NGOI NHA    ");
    lcd.setCursor (0,1);
    lcd.print ("    HIEN DAI   ");
    delay(3000); // chờ 3giay
    lcd.clear(); //Xóa LCD
    lcd.setCursor (0,0);
    lcd.print ("   DA SAN SANG   ");
    alarmStat = 0;
    alarm = 0;
  }}
  
  mfrc522.PICC_HaltA();  
  mfrc522.PCD_StopCrypto1(); 

  }



    
  
