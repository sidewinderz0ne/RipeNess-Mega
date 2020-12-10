#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int a1 = 2;
int a2 = 3;
int a3 = 4;
int raw1, raw2, raw3 = 0;
int Vin = 5;
long timer = 0;
long waktu = 10000;
float Vout1, Vout2, Vout3 = 0;
float R11 = 9830;
float R12 = 9830;
float R13 = 9830;
float R21, R22, R23 = 0;
float buffer1, buffer2, buffer3 = 0;
boolean lanjut = true;

const int rAC11 = 6;
const int rAC12 = 6;
const int rAC13 = 6;
const int rAC21 = 6;
const int rAC22 = 6;
const int rAC23 = 6;
float rAB11[rAC11];
float rAB21[rAC21];
float rAB12[rAC12];
float rAB22[rAC22];
float rAB13[rAC13];
float rAB23[rAC23];
int nRA11, nRA21, nRA12, nRA22, nRA13, nRA23;
float rAH11, rAH21, rAH12, rAH22, rAH13, rAH23;

int buzzerSound = 2345;

#define buzzer 7

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.begin();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.println(" SRS");
  display.display();
  tone(buzzer, buzzerSound);
  delay(2000);
  noTone(buzzer);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void buzz(){
    tone(buzzer, buzzerSound);
    delay(1000);
    noTone(buzzer);
    delay(1000);
}

void loop()
{
  raw1 = analogRead(a1);
  raw2 = analogRead(a2);
  raw3 = analogRead(a3);
  if (raw1 && raw2 && raw3)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    buffer1 = raw1 * Vin;
    Vout1 = (buffer1) / 1024.0;
    buffer1 = (Vin / Vout1) - 1;
    R21 = R11 * buffer1;

    buffer2 = raw2 * Vin;
    Vout2 = (buffer2) / 1024.0;
    buffer2 = (Vin / Vout2) - 1;
    R22 = R12 * buffer2;

    buffer3 = raw3 * Vin;
    Vout3 = (buffer3) / 1024.0;
    buffer3 = (Vin / Vout3) - 1;
    R23 = R13 * buffer3;
    //    Serial.print("Vout: ");
    //    Serial.println(Vout1);
    //    Serial.print("R2: ");
    //    Serial.println(R21);
    //    Serial.println(millis());
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (Vout1 >= 1 && Vout2 >= 1 && Vout3 >= 1 && lanjut) {
    timer = millis() + waktu;
    lanjut = false;
  }
  else if (Vout1 >= 1 && Vout2 >= 1 && Vout3 >= 1 && millis() >= timer) {
    digitalWrite(LED_BUILTIN, LOW);
    for (int i = 0; i < 10; ++i)
    {
      buzz();
    }
    noTone(buzzer);
    delay(2125000);
  } else if (Vout1 >= 1 && Vout2 >= 1 && Vout3 >= 1 &&  millis() <= timer ) {

    //A1
    rAB11[nRA11++] = Vout1;
    if (nRA11 >= rAC11)
    {
      nRA11 = 0;
    }
    rAH11 = 0;
    for (int i = 0; i < rAC11; ++i)
    {
      rAH11 += rAB11[i];
    }
    rAH11 /= rAC11;
    rAB21[nRA21++] = R21;
    if (nRA21 >= rAC21)
    {
      nRA21 = 0;
    }
    rAH21 = 0;
    for (int i = 0; i < rAC21; ++i)
    {
      rAH21 += rAB21[i];
    }
    rAH21 /= rAC21;

    //A2
    rAB12[nRA12++] = Vout2;
    if (nRA12 >= rAC12)
    {
      nRA12 = 0;
    }
    rAH12 = 0;
    for (int i = 0; i < rAC12; ++i)
    {
      rAH12 += rAB12[i];
    }
    rAH12 /= rAC12;
    rAB22[nRA22++] = R22;
    if (nRA22 >= rAC22)
    {
      nRA22 = 0;
    }
    rAH22 = 0;
    for (int i = 0; i < rAC22; ++i)
    {
      rAH22 += rAB22[i];
    }
    rAH22 /= rAC22;

    //A3
    rAB13[nRA13++] = Vout3;
    if (nRA13 >= rAC13)
    {
      nRA13 = 0;
    }
    rAH13 = 0;
    for (int i = 0; i < rAC13; ++i)
    {
      rAH13 += rAB13[i];
    }
    rAH13 /= rAC13;
    rAB23[nRA23++] = R23;
    if (nRA23 >= rAC23)
    {
      nRA23 = 0;
    }
    rAH23 = 0;
    for (int i = 0; i < rAC23; ++i)
    {
      rAH23 += rAB23[i];
    }
    rAH23 /= rAC23;
  }
  
  display.display();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.print("Run Time: ");
  display.println(millis() / 1000);
  
  display.print(Vout1);
  display.print("/");
  int r11 = R21;
  display.print(r11);
  display.print("|");
  int r21 = rAH21;
  display.print(rAH11);
  display.print("/");
  display.println(r21);

  display.print(Vout2);
  display.print("/");
  int r12 = R22;
  display.print(r12);
  display.print("|");
  int r22 = rAH22;
  display.print(rAH12);
  display.print("/");
  display.println(r22);

  display.print(Vout3);
  display.print("/");
  int r13 = R23;
  display.print(r13);
  display.print("|");
  int r23 = rAH23;
  display.print(rAH13);
  display.print("/");
  display.println(r23);

}
