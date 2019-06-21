#define GREEN_PIN 5
#define BLUE_PIN  6

enum{
  GREEN=0,
  BLUE};
#define DELAY_DURATION 3000
int fade_val;
int SOUND_VOLUME_PIN = A0; 
int SOUND_THRESHOLD = 900;
int SOUND_MAX = 1500;
int soundValue;
int ledMaxValue;
int randomMaxColor;

#include <SPI.h>  // 引用 SPI 程式庫

byte symbol1[8] = {
  0x28, 0x4C, 0x2A, 0x9, 0x9, 0x2A,
  0x4C, 0x28};
byte symbol2[8] = {
  0x0, 0x42, 0x64, 0x8, 0x8, 0x64,
  0x42, 0x0};
  
// 定義 MAX7219 暫存器
const byte NOOP = 0x0;        // 不運作
const byte DECODEMODE = 0x9;  // 解碼模式
const byte INTENSITY = 0xA;   // 顯示強度
const byte SCANLIMIT = 0xB;   // 掃描限制
const byte SHUTDOWN = 0xC;    // 停機
const byte DISPLAYTEST = 0xF; // 顯示器檢測

// 設定 MAX7219 暫存器資料的自訂函數
void max7219(byte reg, byte data) {
  digitalWrite (SS, LOW);
  SPI.transfer (reg);
  SPI.transfer (data);
  digitalWrite (SS, HIGH);
}


void setup(){
  pinMode(SS, OUTPUT);     // 將預設的 SS 腳（數位 10）設成「輸出」
  digitalWrite(SS, HIGH);  // 先在 SS 腳輸出高電位
  //（代表「尚不選取周邊」）
  SPI.begin ();     // 啟動 SPI 連線
  
  max7219 (SCANLIMIT, 7);  // 設定掃描 8 行
  max7219 (DECODEMODE, 0); // 不使用 BCD 解碼
  max7219 (INTENSITY, 8);  // 設定成中等亮度
  max7219 (DISPLAYTEST, 0);// 關閉顯示器測試
  max7219 (SHUTDOWN, 1);   // 關閉停機模式（亦即，「開機」）

  // 清除顯示畫面（LED 矩陣中的八行都設定成 0）
  for (byte i=0; i < 8; i++) {
    max7219 (i + 1, 0);
  }

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
 
  randomSeed(analogRead(GREEN_PIN));
  randomSeed(analogRead(BLUE_PIN));
 Serial.begin(9600);
}

void loop()
{ 
  soundValue = analogRead(SOUND_VOLUME_PIN);
  Serial.println(soundValue);
  delay(500);
  
  if (soundValue > SOUND_THRESHOLD)
  {
    ledMaxValue = map(soundValue, SOUND_THRESHOLD, SOUND_MAX, 0, 255);
    analogWrite(GREEN_PIN, 1);
    analogWrite(BLUE_PIN, 0);
    
    for (byte i=0; i<8; i++) {
    max7219 (i + 1, symbol2[i]);  // 顯示自訂圖像
  }
    delay(DELAY_DURATION); 
  }
  else
  {    
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 1);
  
    for (byte i=0; i<8; i++) {
    max7219 (i + 1, symbol1[i]);  // 顯示自訂圖像
  }
  }
}


void OneColor(int _on, int _off1)
{
  analogWrite(_on, 0);
  analogWrite(_off1, 0);
  delay(300);
  
  for (fade_val=0; fade_val<256; fade_val=fade_val+5)
  {
    analogWrite(_on, fade_val);
    delay(DELAY_DURATION);
  }
}
