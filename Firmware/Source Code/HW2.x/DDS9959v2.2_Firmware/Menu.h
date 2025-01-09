#ifndef MENU_H
#define MENU_H

#define MAIN_MENU 0
#define CORE_CLOCK_MENU 1
uint8_t menuType = MAIN_MENU;

#define TCXO_OCXO 0
#define EXT_TCXO_OCXO 1
#define EXT_OSC 2

#define MIN_DDS_CORE_CLOCK_PLL 400000000
#define MAX_DDS_CORE_CLOCK_PLL 600000000
#define BASE_DDS_CORE_CLOCK 500000000UL //это частота на которую будет сбрасываться значение DDS_Core_Clock в любых "непонятных" случаях

#define MAX_DDS_CORE_CLOCK 600000000ULL //Hz (600 MHz) максимально допустимое значение DDS_CORE_CLOCK
#define MIN_DDS_CORE_CLOCK 400000000 //Hz (500 MHz) минимально допустимое значение DDS_CORE_CLOCK
#define BASE_EXT_DDS_CORE_CLOCK 500000000 //HZ (500 MHZ) //это частота на которую будет сбрасываться значение DDS_Core_Clock в режиме внешнего тактирования

#define MAX_OUT_FREQ 300 //MHZ

#define N_MIN_VALUE 4
#define N_MAX_VALUE 20

const uint16_t ExtMHzMax=MAX_DDS_CORE_CLOCK/1000000;
const uint16_t ExtMHzMin=MIN_DDS_CORE_CLOCK/1000000;

//**** EEPROM ADRESSES ****

#define CLOCK_SOURCE_ADR 0
#define CLOCK_TCXO_FREQ_INDEX_ADR 2
#define EXT_CLOCK_FREQ_MHZ_ADR 6
#define DDS_CORE_CLOCK_N_ADR 8

#define F0_MHZ_ADR 22
#define F0_KHZ_ADR 24
#define F0_HZ_ADR 28
#define F0_AMPLITUDE_ADR 32
#define F0_PHASE_ADR 36
#define F0_PHASE_FRACTION_ADR 38

#define F1_MHZ_ADR 40
#define F1_KHZ_ADR 42
#define F1_HZ_ADR 44
#define F1_AMPLITUDE_ADR 46
#define F1_PHASE_ADR 48
#define F1_PHASE_FRACTION_ADR 50

#define F2_MHZ_ADR 52
#define F2_KHZ_ADR 54
#define F2_HZ_ADR 56
#define F2_AMPLITUDE_ADR 58
#define F2_PHASE_ADR 60
#define F2_PHASE_FRACTION_ADR 62

#define F3_MHZ_ADR 64
#define F3_KHZ_ADR 66
#define F3_HZ_ADR 68
#define F3_AMPLITUDE_ADR 70
#define F3_PHASE_ADR 72
#define F3_PHASE_FRACTION_ADR 74

#define MAIN_SETTINGS_FLAG_ADR 100 // defualt settings 
#define CLOCK_SETTINGS_FLAG_ADR 101 // defualt clock settings flag address

//***********************

// **** INIT VALUES ******
#define INIT_MHZ 100
#define INIT_KHZ 0
#define INIT_HZ 0
#define INIT_AMPLITUDE 3
#define INIT_PHASE 0
#define INIT_PHASE_FRACTION 0

#define INIT_CLOCK_SOURCE_INDEX 0
#define INIT_CLOCK_FREQ_INDEX 2

#define INIT_EXT_CLOCK_FREQ_MHZ 500

#define INIT_DDS_CORE_CLOCK_N 10

class MenuItem
{
  public:
    //String ItemName = "Default Menu Item Name";
    MenuItem* nextItem = NULL;
    MenuItem* prevItem = NULL;
    MenuItem* childItem = NULL;
    MenuItem* parentItem = NULL;
    int16_t value = 0;
    int16_t maxValue = 0;
    int16_t minValue = 0;
    bool bBlink=false;
    MenuItem* moveToNextItem()
    {
      return nextItem;
    };
    MenuItem* moveToPrevItem()
    {
      return prevItem;
    };
    MenuItem* moveToChildItem()
    {
      return childItem;
    };
    MenuItem* moveToParentItem()
    {
      menuType = MAIN_MENU;
      return parentItem;
    };
    virtual void incValue(int addendum) = 0;
    virtual void decValue(int addendum) = 0;
    void incrementValue(int addendum)
    {
        value=value+abs(addendum);
        if (value > maxValue) value=minValue;
    }
    void decrementValue(int addendum)
    {
        value=value-abs(addendum);
        if (value < minValue) value=maxValue;
    }
    virtual bool goToEditMode(bool editMode)
    {
      //Blink=editMode;
      bBlink=!bBlink;
      return bBlink;
    }
    uint8_t GetColor()
    {
      static uint32_t lastTimeColorChangedTime=millis();
      static uint8_t color=1; //1 - white (normal color), 0 - black;
      if (bBlink==false) return 1;
      if ((millis()-lastTimeColorChangedTime)>333)
      {
        lastTimeColorChangedTime=millis();
        color=!color;
        /*if (color==1) color=0; 
          else color=1;*/
      }
      return color;
    }
};

MenuItem* curItem;

class FClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      incrementValue(addendum);
    };
    void decValue(int addendum) {
      decrementValue(addendum);
    };
    FClass()
    {
      parentItem=this;
    }
    bool goToEditMode(bool editMode)
    {
      curItem=moveToChildItem();
      return false;
    }
} F0, F1, F2, F3;

class FreqClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      incrementValue(addendum);
    };
    void decValue(int addendum) {
      decrementValue(addendum);
    };
} F0_MHz, F0_kHz, F0_Hz, 
  F1_MHz, F1_kHz, F1_Hz,
  F2_MHz, F2_kHz, F2_Hz,
  F3_MHz, F3_kHz, F3_Hz;

class AmplitudeClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      decrementValue(addendum);
    };
    void decValue(int addendum) {
      incrementValue(addendum);
    };
} F0_Amplitude, F1_Amplitude, F2_Amplitude, F3_Amplitude;

class PhaseClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      incrementValue(addendum);
      if ((value == 360) && (nextItem->value > 0)) nextItem->value = 0;
    };
    void decValue(int addendum) {
      decrementValue(addendum);
      if ((value == 360) && (nextItem->value > 0)) nextItem->value = 0;
    };
} F0_Phase, F1_Phase, F2_Phase, F3_Phase;

class PhaseFractionClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      incrementValue(addendum);
      if ((value != 0) && (prevItem->value == 360)) prevItem->value = 359;
    };
    void decValue(int addendum) {
      decrementValue(addendum);
      if ((value != 0) && (prevItem->value == 360)) prevItem->value = 359;
    };
} F0_PhaseFraction, F1_PhaseFraction, F2_PhaseFraction, F3_PhaseFraction;

// **************** CLOCK SOURCE SETUP MENU ************************

class ClockSrcClass: public MenuItem
{
  public:
    void incValue(int addendum);
    void decValue(int addendum);
    //String ClockSourceNames[2] = {"TCXO/OCXO", "Ext. Clock"};
    String ClockSourceNames[3] = {"TCXO/OCXO", "Ext. TCXO/OCXO", "Ext. OSC"};
} ClockSrc;

class ClockFreqClass: public MenuItem
{
  public:
    void incValue(int addendum);
    void decValue(int addendum);
    uint32_t Ref_Clk [5] = {25000000, 40000000, 50000000, 100000000, 125000000};
} ClockFreq;

class DDSCoreClockClass: public MenuItem
{
  public:
    uint32_t DDSCoreClock=0;
    void incValue(int addendum) 
    {
      value++;
      if (value > N_MAX_VALUE) value=FindMinimalN();
      //if (ClockFreq.Ref_Clk[ClockFreq.value]*(value)*2 > MAX_DDS_CORE_CLOCK_PLL) value=FindMinimalN();
      if (ClockFreq.Ref_Clk[ClockFreq.value]*value > MAX_DDS_CORE_CLOCK_PLL) value=FindMinimalN();
      //DDSCoreClock=ClockFreq.Ref_Clk[ClockFreq.value]*value*2;
      SetDDSCoreClock();
      #if DBG==1
      Serial.print("N=");
      Serial.println(value);
      #endif
    }
    
    void decValue(int addendum) 
    {
      value--;
      if (value < N_MIN_VALUE) value=FindMaximalN();
      //if (ClockFreq.Ref_Clk[ClockFreq.value]*(value)*2 < MIN_DDS_CORE_CLOCK_PLL) value=FindMaximalN();
      if (ClockFreq.Ref_Clk[ClockFreq.value]*value < MIN_DDS_CORE_CLOCK_PLL) value=FindMaximalN();
      //DDSCoreClock=ClockFreq.Ref_Clk[ClockFreq.value]*value*2;
      SetDDSCoreClock();
      #if DBG==1
      Serial.print("N=");
      Serial.println(value);
      #endif
    }
    
    uint16_t FindMinimalN()
    {
      //uint16_t tmpN=ceil(MIN_DDS_CORE_CLOCK_PLL/2/ClockFreq.Ref_Clk[ClockFreq.value]);
      uint16_t tmpN=ceil(MIN_DDS_CORE_CLOCK_PLL/ClockFreq.Ref_Clk[ClockFreq.value]);
      if (tmpN < N_MIN_VALUE) tmpN=N_MIN_VALUE;
      return tmpN;
    }
    
    uint16_t FindMaximalN()
    {
      //uint16_t tmpN=floor(MAX_DDS_CORE_CLOCK_PLL/2/ClockFreq.Ref_Clk[ClockFreq.value]);
      uint16_t tmpN=floor(MAX_DDS_CORE_CLOCK_PLL/ClockFreq.Ref_Clk[ClockFreq.value]);
      if (tmpN > N_MAX_VALUE) tmpN=N_MAX_VALUE;
      return tmpN;
    }
    
    void SetNForDDSCoreFreq(uint32_t ui32Freq) // Set N (value) to get nearest value of freq. 
    {
      //uint16_t tmpN=ui32Freq/2/ClockFreq.Ref_Clk[ClockFreq.value];
      uint16_t tmpN=ui32Freq/ClockFreq.Ref_Clk[ClockFreq.value];
      if (tmpN < N_MIN_VALUE) tmpN=N_MIN_VALUE;
      if (tmpN > N_MAX_VALUE) tmpN=N_MAX_VALUE;
      //return tmpN;
      value=tmpN;
    }
    
    void SetDDSCoreClock()
    {
      //DDSCoreClock=ClockFreq.Ref_Clk[ClockFreq.value]*value*2;
      DDSCoreClock=ClockFreq.Ref_Clk[ClockFreq.value]*value;
    }

    bool IsFreqInRange()
    {
      if ((DDSCoreClock>MIN_DDS_CORE_CLOCK_PLL) && (DDSCoreClock<MAX_DDS_CORE_CLOCK_PLL)) return true;
      else return false;
    }
    
    uint64_t GetDDSCoreClock();

    DDSCoreClockClass()
    {
      SetNForDDSCoreFreq(BASE_DDS_CORE_CLOCK);
      SetDDSCoreClock();
    }

    bool ExtClockFreqInRange();
    
} DDSCoreClock;

class CoreClockSaveClass: public MenuItem
{
  public:
    void incValue(int addendum) {
      
    };
    
    void decValue(int addendum) {
      
    };
    
    bool goToEditMode (bool editMode);
} CoreClockSave;

extern void DrawBackground();
class CoreClockExitClass: public MenuItem
{
  public:
    void incValue(int addendum) {
     
    };
    void decValue(int addendum) {
      ;
    };
    bool goToEditMode (bool editMode)
    {
      //Serial.println("Exit!!!");
      curItem = &F0;
      menuType = MAIN_MENU;
      DrawBackground();
     // DisplayMenu(menuType);
      return false;
    }
} CoreClockExit;

class ExtClockFreqMHzClass: public MenuItem
{
  public:
    void incValue(int addendum);
    void decValue(int addendum);
   
} ExtClockFreqMHz;

void MenuLinking();
void MenuInitValues();

void LoadMainSettings();
void SaveMainSettings();

void LoadClockSettings();
void SaveClockSettings();

void DisplayMessage(String Title, String Message);
void DisplaySaved(void);

void PrintValues();

#endif
