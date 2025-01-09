#define DBG 0

#include <EEPROM.h>
#include <U8g2lib.h>
#include "u8g2_font_5x8_custom_mr.h"
#include "Menu.h"
#include "DisplayMenu.h"
#include <ClickButton.h>
#include <AD9959.h>

#include <Encoder.h>
#ifndef GRA_AND_AFCH_ENCODER_MOD3
  #error The "Encoder" library modified by GRA and AFCH must be used!
#endif

#define FIRMWAREVERSION 2.04

//2.04 13.12.2024 закончено внедрения третьего параметра для Clock Src - ext TCXO/OCXO
//    Строковые константы завернуты в макрос F
//2.03 27.11.2024 начало внедрения третьего параметра для Clock Src - ext TCXO/OCXO
//2.02 23.09.2024 Переход на дисплеи с контроллером SSD1306 и SSD1309
//2.01 19.09.2024 Переход на плату версии 2.х
//    управление коммутатором тактирования
//    управелние светодиодом индикации источника тактирования
//    исправлена ошибка с поляронстью фазы и поэтому теперь не используется функция CorrectPhase
//    изменены значения по-умолчанию: TCXO - 50 МГц, N - 10, Amplitude - -3dBm
//    Увеличина мощность выходного сигнала с -7 до -3 dBm (чтобы увеличить мощность необходимо заменить резистор R11 c 1.91 kOhm на 1.2 kOhm, это обязательно нужно сделать иначе мощность на экране не будет совпадать с фактической)

//1.23 Добавлены комманды включения и выключения выходов (всех одновременно)
//1.22 ускорена обработка команд через последовтельный порт
//1.21 12.06.2023 добавлена поддержка управления через последовтельный порт
//1.2 15.07.2021 исправлен баг с уходом фазы при перестройке частоты
//1.1 06.11.2020 исправлена фаза на выходах F2 и F3
//0.17 Контроль граничных значений частоты и вывод сообщений об ошибках на экран
//0.16 изменение настроек в меню тактирования применятся только после принудительного сохранения
// библиотека AD9959 изменена таким образом чтобы можнобыло менять частоту через функцию SetClock reference_freq
//0.15 кнопка back выходит из меню настроек тактирования
//0.14 19.10.2020 исправление ошибок
//0.13 Добавлено сохранение настроек тактирования в EEPROM
//0.12 Добавляем сохранение основных настроек в EEPROM
//0.11 включаем в DisplayMenu отображение реальных значений, устраняем ошибку когда кнопка back не отключает режим редактирования

//AAAAAAA TCXO 50MHZ сделать по умолчанию!!!!
// RF Switcher
// REF_LED ON/OFF TCXO - pin 8
// V1 - pin 49
// V2 - pin 48

//#include <AsyncStream.h>
//AsyncStream<110> serialbuffer(&Serial, '\n');
#include <GParser.h>

#include  "AD9959.h"
#ifndef GRA_AND_AFCH_AD9959_MOD
  #error The "AD9959" library modified by GRA and AFCH must be used!
#endif

#define POWER_DOWN_CONTROL_PIN 13
#define SDIO_3_PIN 11
#define SDIO_1_PIN 9

#define P0_PIN 16
#define P1_PIN 15
#define P2_PIN 14
#define P3_PIN 4

#define CLOCK_SOURCE_TCXO_INDEX 0
#define CLOCK_SOURCE_EXT_TCXO_INDEX 1
#define CLOCK_SOURCE_EXT_OSC_INDEX 2
#define TCXO_POWER_PIN 8 //REF_LED
#define TCXO_PATH_PIN 48 //V2
#define EXTERANL_SRC_PATH_PIN 49 //V1

bool isPWR_DWN = false;

class MyAD9959 : public AD9959<
    12,              // Reset pin (active = high)
    6,              // Chip Enable (active = low)
    5,              // I/O_UPDATE: Apply config changes (pulse high)
    40000000        // 40MHz crystal (optional)
> 
{
  public:
    void AllChanAutoClearPhase()
    {
     //setChannels(MyAD9959::Channel0);
     write(MyAD9959::FR2, FR2_Bits::AllChanAutoClearPhase); 
    }
};

MyAD9959  dds;

//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

uint32_t EXT_OSC_Freq=BASE_DDS_CORE_CLOCK;
uint32_t DDS_Core_Clock=BASE_DDS_CORE_CLOCK;
#define LOW_FREQ_LIMIT  100000 //Hz (100 kHz)
#define HIGH_FREQ_LIMIT  225000000 //Hz (225 MHZ)
uint32_t ui32HIGH_FREQ_LIMIT=0;
uint32_t F0OutputFreq=0, F1OutputFreq=0, F2OutputFreq=0, F3OutputFreq=0;

#define MODE_PIN 2   
#define BACK_PIN 3

ClickButton modeButton (MODE_PIN, LOW, CLICKBTN_PULLUP);
ClickButton backButton (BACK_PIN, LOW, CLICKBTN_PULLUP);

//Encoder myEnc(18, 19); // Voron EC12PLGSDVF D25K24 (blue)
Encoder myEnc(19, 18); // Bourns PEC11R-4220K-S0024 (green)

bool MenuEditMode=false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10);
  Serial.println(F("DDS AD9959 by GRA & AFCH"));
  Serial.println(F("HW v2.x"));
  Serial.print(F("SW v"));
  Serial.println(FIRMWAREVERSION);
  
  u8g2.setBusClock(800000);
  //u8g2.setBusClock(1000000UL);
  u8g2.begin();
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_5x8_custom_mr);
  u8g2.clearBuffer();

  DisplayHello();
  delay(3000);
  u8g2.clearBuffer();
  
  DrawBackground();
  //u8g2.writeBufferPBM(Serial);
  MenuLinking();
  MenuInitValues();

  modeButton.Update();
  if (modeButton.depressed == true) //если при включении была зажата кнопка MODE, то затираем управляющие флаги в EEPROM, которые восстановят заводские значения всех параметров
  {
    EEPROM.write(CLOCK_SETTINGS_FLAG_ADR, 255); //flag that force save default clock settings to EEPROM 
    EEPROM.write(MAIN_SETTINGS_FLAG_ADR, 255); //flag that force save default main settings to EEPROM 
  }
  
  pinMode(POWER_DOWN_CONTROL_PIN, OUTPUT);
  digitalWrite(POWER_DOWN_CONTROL_PIN, LOW);

  pinMode(SDIO_1_PIN, INPUT);

  pinMode(SDIO_3_PIN, OUTPUT);
  digitalWrite(SDIO_3_PIN, LOW);
  
  pinMode(P0_PIN, OUTPUT);
  pinMode(P1_PIN, OUTPUT);
  pinMode(P2_PIN, OUTPUT);
  pinMode(P3_PIN, OUTPUT);
  digitalWrite(P0_PIN, LOW);
  digitalWrite(P1_PIN, LOW);
  digitalWrite(P2_PIN, LOW);
  digitalWrite(P3_PIN, LOW);

  pinMode(TCXO_POWER_PIN, OUTPUT);
  pinMode(TCXO_PATH_PIN, OUTPUT);
  pinMode(EXTERANL_SRC_PATH_PIN, OUTPUT);
  digitalWrite(TCXO_POWER_PIN, LOW);
  digitalWrite(TCXO_PATH_PIN, LOW);
  digitalWrite(EXTERANL_SRC_PATH_PIN, LOW);
  
  LoadClockSettings();
  LoadMainSettings();

  //debug
   /*SPI.beginTransaction(SPISettings(2000000, 1, SPI_MODE3));
   digitalWrite(6, LOW);

    SPI.transfer(0x1);
    SPI.transfer(0x80 | 10*0x04 | 0x03);
    SPI.transfer(0x00 | 0x00 | 0x00);
    SPI.transfer(0x20); // Don't output SYNC_CLK
    //SPI.transfer(0x00);

   digitalWrite(6, HIGH);
   SPI.endTransaction();

   SPI.beginTransaction(SPISettings(2000000, 1, SPI_MODE3));
   digitalWrite(6, LOW);

    SPI.transfer(0x2);
    SPI.transfer(B11101111);
    SPI.transfer(B11111111);

   digitalWrite(6, HIGH);
   SPI.endTransaction();

  dds.setFrequency(MyAD9959::Channel0, 100000000); 
  dds.setAmplitude(MyAD9959::Channel0, dBmToASF(10)); 

  dds.setFrequency(MyAD9959::Channel1, 100000000); 
  dds.setAmplitude(MyAD9959::Channel1, dBmToASF(10)); 

  dds.setFrequency(MyAD9959::Channel2, 100000000); 
  dds.setAmplitude(MyAD9959::Channel2, dBmToASF(10)); 

  dds.setFrequency(MyAD9959::Channel3, 100000000); 
  dds.setAmplitude(MyAD9959::Channel3, dBmToASF(10)); 

  dds.update();
   while(1) */
  //debug end

  dds.AllChanAutoClearPhase();
  ApplyChangesToDDS();

  curItem = &F0;

  modeButton.debounceTime   = 25;   // Debounce timer in ms
  modeButton.multiclickTime = 10;  // Time limit for multi clicks
  modeButton.longClickTime  = 1000; // time until "held-down clicks" register

  backButton.debounceTime   = 25;   // Debounce timer in ms
  backButton.multiclickTime = 10;  // Time limit for multi clicks
  backButton.longClickTime  = 1000; // time until "held-down clicks" register

  /*curItem = &ClockSrc; //4del
  menuType = CORE_CLOCK_MENU; //4del 
  u8g2.clearBuffer(); //4del*/

  DisplayMenu(menuType); 
  attachInterrupt(digitalPinToInterrupt(MODE_PIN), ModeButtonDown, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BACK_PIN), DownButtonDown, CHANGE);
}

void ModeButtonDown()
{
  volatile static uint32_t lastTimeButtonPressed=millis();
  volatile static uint8_t lastState=1;
  volatile static uint8_t currState=1;
  if (millis()-lastTimeButtonPressed < modeButton.debounceTime) return;
  currState=digitalRead(MODE_PIN);
  if ((lastState == HIGH) && (currState == LOW)) modeButton.Update();
  lastState=currState;
}

void DownButtonDown()
{
  volatile static uint32_t lastTimeButtonPressed=millis();
  volatile static uint8_t lastState=1;
  volatile static uint8_t currState=1;
  if (millis()-lastTimeButtonPressed < backButton.debounceTime) return;
  currState=digitalRead(BACK_PIN);
  if ((lastState == HIGH) && (currState == LOW)) backButton.Update();
  lastState=currState;
}

void loop() 
{ 
  while (1)
  {
    //u8g2.firstPage();
    ReadSerialCommands();
    int curPos=0;
    curPos=myEnc.read();
    
    modeButton.Update();
    backButton.Update();

    if (modeButton.clicks == 1) 
    {
      //MenuEditMode=!MenuEditMode;
      MenuEditMode=curItem->goToEditMode(MenuEditMode);
      DisplayMenu(menuType);
    }

    if (backButton.clicks > 0) 
    {
      curItem->bBlink=false;
      curItem=curItem->moveToParentItem();
      MenuEditMode=false;
      //Serial.println("Back btn pressed");
      LoadClockSettings();
      u8g2.clearBuffer();
      DrawBackground();
      DisplayMenu(menuType);
    }
    
    if ((modeButton.clicks == -1) && (modeButton.depressed == true)) 
    {
      curItem = &ClockSrc; 
      menuType = CORE_CLOCK_MENU; 
      u8g2.clearBuffer();
      DisplayMenu(menuType);
    } 

    if (curPos>0)
    {
      switch(MenuEditMode)
      {
        case true: 
          curItem->incValue(curPos); 
          ApplyChangesToDDS(); //4del
          /*ui32CurrentOutputFreq=GetFreq(); 
          DDS.setFreq(ui32CurrentOutputFreq,0); 
          DDS.setAmpdB(Amplitude.value * -1, 0); */ //uncomment
          if (menuType == MAIN_MENU) SaveMainSettings();
        break;
        case false: curItem=curItem->moveToNextItem(); break;
      }
      DisplayMenu(menuType);
    }
    
    if (curPos<0)
    {
      switch(MenuEditMode)
      {
        case true: 
          curItem->decValue(curPos);
          /*
          ui32CurrentOutputFreq=GetFreq();
          DDS.setFreq(ui32CurrentOutputFreq,0);
          DDS.setAmpdB(Amplitude.value * -1, 0);*/ //uncomment
          ApplyChangesToDDS(); //4del
          if (menuType == MAIN_MENU) SaveMainSettings(); 
        break;
        case false: curItem=curItem->moveToPrevItem(); break;
      }
      DisplayMenu(menuType);
    }
    /*uint32_t prev;
    prev=millis();*/
    DisplayMenu(menuType);  
    //Serial.println(millis()-prev);
  }
}

uint32_t DegToPOW(uint16_t deg) //функция принимает значение в градусах умноженное на 10
{
  #define TWO_POW_14 16384
  uint32_t POW=(deg/3600.0)*TWO_POW_14; //360.0 changed to 3600.0
  return POW;
}

uint16_t dBmToASF(uint8_t dBm)
{
  //return (uint16_t)powf(10,(-1*dBm+7+60.206)/20.0); //10(maxValue)+log(1024(2^10))*20=60,205999132796239042747778944899
  return (uint16_t)powf(10,(-1*dBm+3+60.206)/20.0);
}

void ApplyChangesToDDS()
{
  //dds.reference_freq=ClockFreq.Ref_Clk[ClockFreq.value];
  //dds.setClock(DDSCoreClock.value,0); //<-- set PLL multiplier

  if (FreqInRange() !=0) return;

  //dds.write(MyAD9959::FR1, FR1_Bits::SyncClkDisable); // Don't output SYNC_CLK
  //dds.AllChanAutoClearPhase();

  dds.setFrequency(MyAD9959::Channel0, F0OutputFreq); 
  dds.setAmplitude(MyAD9959::Channel0, dBmToASF(F0_Amplitude.value));   
  dds.setPhase(MyAD9959::Channel0, DegToPOW(F0_Phase.value * 10 + F0_PhaseFraction.value));    

  dds.setFrequency(MyAD9959::Channel1, F1OutputFreq);  
  dds.setAmplitude(MyAD9959::Channel1, dBmToASF(F1_Amplitude.value));    
  dds.setPhase(MyAD9959::Channel1, DegToPOW(F1_Phase.value * 10 + F1_PhaseFraction.value));    

  dds.setFrequency(MyAD9959::Channel2, F2OutputFreq);  
  dds.setAmplitude(MyAD9959::Channel2, dBmToASF(F2_Amplitude.value));   
  //dds.setPhase(MyAD9959::Channel2, DegToPOW(CorrectPhase(F2_Phase.value * 10 + F2_PhaseFraction.value))); // компенсация ошибки разводки на плате весрии 1.1
  dds.setPhase(MyAD9959::Channel2, DegToPOW(F2_Phase.value * 10 + F2_PhaseFraction.value));

  dds.setFrequency(MyAD9959::Channel3, F3OutputFreq);  
  dds.setAmplitude(MyAD9959::Channel3, dBmToASF(F3_Amplitude.value));    
  //dds.setPhase(MyAD9959::Channel3, DegToPOW(CorrectPhase(F3_Phase.value * 10 + F3_PhaseFraction.value))); // компенсация ошибки разводки на плате весрии 1.1  
  dds.setPhase(MyAD9959::Channel3, DegToPOW(F3_Phase.value * 10 + F3_PhaseFraction.value)); 
  
  dds.update();
}

int8_t FreqInRange() //1,2,3,4 - Higher, -1,-2,-3,-4 - lower, 0 - in range
{
  //ui32HIGH_FREQ_LIMIT=DDS_Core_Clock*0.45;
  F0OutputFreq=F0_MHz.value * 1000000UL + F0_kHz.value * 1000UL + F0_Hz.value;
  F1OutputFreq=F1_MHz.value * 1000000UL + F1_kHz.value * 1000UL + F1_Hz.value;
  F2OutputFreq=F2_MHz.value * 1000000UL + F2_kHz.value * 1000UL + F2_Hz.value;
  F3OutputFreq=F3_MHz.value * 1000000UL + F3_kHz.value * 1000UL + F3_Hz.value;

  #if DBG==1
  Serial.print("ui32HIGH_FREQ_LIMIT=");
  Serial.println(ui32HIGH_FREQ_LIMIT);
  #endif
  
  if ((F0OutputFreq > ui32HIGH_FREQ_LIMIT) || (F0OutputFreq > HIGH_FREQ_LIMIT)) return 1;
  if (F0OutputFreq < LOW_FREQ_LIMIT) return -1;

  if ((F1OutputFreq > ui32HIGH_FREQ_LIMIT) || (F1OutputFreq > HIGH_FREQ_LIMIT)) return 2;
  if (F1OutputFreq < LOW_FREQ_LIMIT) return -2;

  if ((F2OutputFreq > ui32HIGH_FREQ_LIMIT) || (F2OutputFreq > HIGH_FREQ_LIMIT)) return 3;
  if (F2OutputFreq < LOW_FREQ_LIMIT) return -3;

  if ((F3OutputFreq > ui32HIGH_FREQ_LIMIT) || (F3OutputFreq > HIGH_FREQ_LIMIT)) return 4;
  if (F3OutputFreq < LOW_FREQ_LIMIT) return -4;
  return 0;
}

uint16_t CorrectPhase(uint16_t phase) // компенсация ошибки разводки на плате весрии 1.1
{
  if (phase < 1800) phase = phase + 1800;
    else phase = phase - 1800;
    return phase;
}

void selectClockSrcPath(uint8_t path)
{
  switch (path)
  {
    case CLOCK_SOURCE_TCXO_INDEX:
      digitalWrite(TCXO_POWER_PIN, HIGH);
      digitalWrite(TCXO_PATH_PIN, HIGH);
      digitalWrite(EXTERANL_SRC_PATH_PIN, LOW);
    break;
    case CLOCK_SOURCE_EXT_TCXO_INDEX:  //////////// ЭТО ЭКСТЕРНАЛ TCXO!!!!!!!!!!  //
      digitalWrite(TCXO_POWER_PIN, LOW);
      digitalWrite(TCXO_PATH_PIN, LOW);
      digitalWrite(EXTERANL_SRC_PATH_PIN, HIGH);
    break;
    case CLOCK_SOURCE_EXT_OSC_INDEX:
      digitalWrite(TCXO_POWER_PIN, LOW);
      digitalWrite(TCXO_PATH_PIN, LOW);
      digitalWrite(EXTERANL_SRC_PATH_PIN, HIGH);
    break;    
  }
}