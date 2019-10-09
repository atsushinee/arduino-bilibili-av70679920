#include <IRremote.h>
#include <LiquidCrystal.h>

#define C0 -1
#define C1 262
#define C2 294
#define C3 330
#define C4 350
#define C5 393
#define C6 441
#define C7 495

#define L1 131
#define L2 147
#define L3 165
#define L4 175
#define L5 196
#define L6 221
#define L7 248

#define H1 525
#define H2 589
#define H3 661
#define H4 700
#define H5 786
#define H6 882
#define H7 990




int remotePIN = 13;
int tonePIN = 6;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
IRrecv irrecv(remotePIN);
decode_results results;



int songStart[] = {
  C5, C6, C5, C4, C3, C2, C1, L5,
  C1, C3, H1, C7, C6, C3, C5, C5,
  C6, C7, C6, C5, C4, C3, C2, L6,
  L7, L6, L5, C5, C1, C2, C3, C3,
  C5, C6, C5, C4, C3, C2, C1, L5,
  C1, C3, H1, C7, H2, H1, C6, C6,
  H1, C7, C6, C5, C6, C5, C4, C3,
  L7, L6, L5, C2, C1, C1,

  H1, H2, H3, H2, H1, C6, C7, C6, C3, C5, C5,
  H1, H2, H3, H2, H1, C6, C7, C5, C3, C6, C6,
  C5, C4, C3, C2, L7, L6, L6, L5, C3,
  C3, C2, C1, C1

};

int songEnd[] = {
  H1, H2, H3, H2, H1, C6, C7, C6, C3, C5, C5,
  H1, H2, H3, H2, H1, C6, C7, C5, C3, C6, C6,
  C5, C4, C3, C2, L7, L6, L5, C3,
  C5, H2, H1, H1, H1
};

float delayStart[] = {
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 1.5, 0.5, 0.5, 0.5, 1.5,
  1, 0.5, 1, 0.5, 1.5, 1.5,

  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 1.5, 0.5, 0.25, 0.25, 0.5, 1.5,
  1.5, 1, 0.5, 2.5

};

float delayEnd[] = {
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 1.5, 1.5,
  0.5, 0.5, 0.5, 1.5, 0.5, 0.5, 0.5, 1.5,
  1.5, 1, 0.5, 1.5, 1.5
};


void play(int code, float r)
{
  tone(tonePIN, code);
  show(code / 50);
  lightOn(code / 110);
  delay(500 * r);
  lightOFF();
  noTone(tonePIN);
}

int startLen;
int endLen;

String url = "LICHUNORZ.COM";

void show(int len)
{
  lcd.setCursor(0, 1);
  lcd.print("                ");
  for (int i = 0; i < len; i++)
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void lightOn(int len)
{
  lightOFF();
  lightControl(len, HIGH);
}

void lightControl(int len, int mode)
{
  if (len > 5)
  {
    len = 5;
  }
  for (int i = 14; i < len + 14; i++)
  {
    digitalWrite(i, mode);
  }
}

void lightOFF()
{
  lightControl(5, LOW);
}

void show(String s)
{
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(s);
}

uint8_t volume[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,}; //

//uint8_t b1[8] = {0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00,}; // (゜-゜)つロ
uint8_t b2[8] = {0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,}; // ゜
uint8_t b3[8] = {0x03, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,}; // -゜
uint8_t b4[8] = {0x10, 0x08, 0x09, 0x0a, 0x08, 0x08, 0x10, 0x00,}; // )つ
uint8_t b5[8] = {0x00, 0x00, 0x1c, 0x02, 0x02, 0x04, 0x00, 0x00,}; //
//uint8_t b6[8] = {0x00, 0x00, 0x1e, 0x12, 0x12, 0x1e, 0x00, 0x00,}; // ロ

uint8_t b7[8] = {0x00, 0x00, 0x0E, 0x0A, 0x0A, 0x0E, 0x00, 0x00,}; // ロ
uint8_t b9[8] = {0x1f, 0x15, 0x1f, 0x15, 0x1f, 0x4, 0xe, 0x1f,}; // 毕
uint8_t b8[8] = {0x12, 0x1b, 0x16, 0x1f, 0x4, 0x1f, 0x4, 0x4,};  // 里

int i = 0;
boolean flag = true;
int part = 1;

void lcdInit() {
  lcd.createChar(0, volume);


  lcd.createChar(4, b2);
  lcd.createChar(5, b3);
  lcd.createChar(6, b4);
  lcd.createChar(7, b5);

  lcd.createChar(1, b7);
  lcd.createChar(2, b8);
  lcd.createChar(3, b9);
  lcd.begin(16, 2);
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(1));
  lcd.write(byte(3));

  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(1));
  lcd.write(byte(3));
  lcd.print(' ');
  lcd.print('(');
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.write(byte(1));
}

void ledInit() {
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
}

void setup()
{
  ledInit();
  lcdInit();
  startLen = sizeof(songStart) / sizeof(songStart[0]);
  endLen = sizeof(songEnd) / sizeof(songEnd[0]);
  pinMode(tonePIN, OUTPUT);
  pinMode(remotePIN, INPUT);
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results))
  {
    if (results.value == 16761405)
    {
      flag = !flag;
      show("     PAUSE.     ");
    }
    irrecv.resume();
  }

  if (flag)
  {

    if (part == 1)
    {
      play(songStart[i], delayStart[i]);
    }
    else if (part == 2)
    {
      play(songStart[i], delayStart[i]);
    }
    else if (part == 3)
    {
      play(songEnd[i], delayEnd[i]);
    }

    i++;
    if (i == startLen && part == 1)
    {
      part = 2;
      i = 0;
    }
    if (i == startLen && part == 2)
    {
      part = 3;
      i = 0;
    }
    if (i == endLen && part == 3)
    {
      part = 1;
      i = 0;
      delay(2000);
    }
  }
}
