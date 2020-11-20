#include "Menu.h"

void ClockFreqClass::incValue(int addendum)
{
    incrementValue(addendum);
    DDSCoreClock.SetNForDDSCoreFreq(BASE_DDS_CORE_CLOCK);
    DDSCoreClock.SetDDSCoreClock();
}

void ClockFreqClass::decValue(int addendum)
{
    decrementValue(addendum);
    DDSCoreClock.SetNForDDSCoreFreq(BASE_DDS_CORE_CLOCK);
    DDSCoreClock.SetDDSCoreClock();
}

void ClockSrcClass::incValue(int addendum)
{
  incrementValue(1);
  if (value==TCXO_OCXO)
  {
    nextItem=&ClockFreq;
    CoreClockSave.prevItem=&DDSCoreClock;
  } else
    {
      nextItem=&ExtClockFreqMHz;
      CoreClockSave.prevItem=&ExtClockFreqMHz;
    }
}

void ClockSrcClass::decValue(int addendum)
{
  decrementValue(1);
  if (value==TCXO_OCXO)
  {
    nextItem=&ClockFreq;
    CoreClockSave.prevItem=&DDSCoreClock;
  } else
    {
      nextItem=&ExtClockFreqMHz;
      CoreClockSave.prevItem=&ExtClockFreqMHz;
    }
}

void ExtClockFreqMHzClass::incValue(int addendum)
{
  incrementValue(addendum);
  DDSCoreClock.DDSCoreClock=value*1000000;
  /*if ((value>ExtMHzMin) && (ExtClockFreqGHz.value==ExtGHzMax)) ExtClockFreqGHz.value=ExtGHzMax-1;
  if ((value<ExtMHzMin) && (ExtClockFreqGHz.value==ExtGHzMin)) ExtClockFreqGHz.value=ExtGHzMin+1;*/
}

void ExtClockFreqMHzClass::decValue(int addendum)
{
  decrementValue(addendum);
  DDSCoreClock.DDSCoreClock=value*1000000;
  /*if ((value>ExtMHzMin) && (ExtClockFreqGHz.value==ExtGHzMax)) ExtClockFreqGHz.value=ExtGHzMax-1;
  if ((value<ExtMHzMin) && (ExtClockFreqGHz.value==ExtGHzMin)) ExtClockFreqGHz.value=ExtGHzMin+1;*/
}

bool DDSCoreClockClass::ExtClockFreqInRange()
{
  if (((ExtClockFreqMHz.value*1000000) < MIN_DDS_CORE_CLOCK) || ((ExtClockFreqMHz.value*1000000) > MAX_DDS_CORE_CLOCK)) return false;
    else return true;
}

uint64_t DDSCoreClockClass::GetDDSCoreClock()
{
  if (ClockSrc.value==TCXO_OCXO)
  {
    if (!IsFreqInRange()) SetDDSCoreClock();
    return DDSCoreClock;
  } else 
    {
      if (ExtClockFreqInRange())
      {
        return ExtClockFreqMHz.value*1000000;
      }
      else return BASE_EXT_DDS_CORE_CLOCK;
    }
}

bool CoreClockSaveClass::goToEditMode (bool editMode)
    {
      if (ClockSrc.value==TCXO_OCXO) dds.setClock(DDSCoreClock.value, ClockFreq.Ref_Clk[ClockFreq.value], 0);
        else dds.setClock(0, DDSCoreClock.GetDDSCoreClock(), 0);

      DDS_Core_Clock = DDSCoreClock.GetDDSCoreClock();
      ui32HIGH_FREQ_LIMIT = DDS_Core_Clock * 0.45;

      #if DBG==1
      Serial.print("ui32HIGH_FREQ_LIMIT=");
      Serial.print(ui32HIGH_FREQ_LIMIT);
      #endif
      
      ApplyChangesToDDS();
      SaveClockSettings();
      curItem=&CoreClockExit;
      return false;
    }

void MenuLinking()
{
  // ** F Next Items**
  F0.nextItem = &F1;
  F1.nextItem = &F2;
  F2.nextItem = &F3;
  F3.nextItem = &F0; //loop back to F0 item
  // **F Prev Items**
  F3.prevItem = &F2;
  F2.prevItem = &F1;
  F1.prevItem = &F0;
  F0.prevItem = &F3; //loop back to F3 item

  // ** Freq0 Next Items**
  F0_MHz.nextItem = &F0_kHz;
  F0_kHz.nextItem = &F0_Hz;
  F0_Hz.nextItem = &F0_Amplitude;
  F0_Amplitude.nextItem = &F0_Phase; 
  F0_Phase.nextItem = &F0_PhaseFraction;
  F0_PhaseFraction.nextItem = &F0_MHz; //loop back to MHz item
  // **Freq0 Prev Items**
  F0_PhaseFraction.prevItem = &F0_Phase;
  F0_Phase.prevItem = &F0_Amplitude;
  F0_Amplitude.prevItem = &F0_Hz;
  F0_Hz.prevItem = &F0_kHz;
  F0_kHz.prevItem = &F0_MHz;
  F0_MHz.prevItem = &F0_PhaseFraction; //loop back to PhaseFractio item

    // ** Freq1 Next Items**
  F1_MHz.nextItem = &F1_kHz;
  F1_kHz.nextItem = &F1_Hz;
  F1_Hz.nextItem = &F1_Amplitude;
  F1_Amplitude.nextItem = &F1_Phase; 
  F1_Phase.nextItem = &F1_PhaseFraction;
  F1_PhaseFraction.nextItem = &F1_MHz; //loop back to MHz item
  // **Freq1 Prev Items**
  F1_PhaseFraction.prevItem = &F1_Phase;
  F1_Phase.prevItem = &F1_Amplitude;
  F1_Amplitude.prevItem = &F1_Hz;
  F1_Hz.prevItem = &F1_kHz;
  F1_kHz.prevItem = &F1_MHz;
  F1_MHz.prevItem = &F1_PhaseFraction; //loop back to PhaseFractio item

    // ** Freq2 Next Items**
  F2_MHz.nextItem = &F2_kHz;
  F2_kHz.nextItem = &F2_Hz;
  F2_Hz.nextItem = &F2_Amplitude;
  F2_Amplitude.nextItem = &F2_Phase; 
  F2_Phase.nextItem = &F2_PhaseFraction;
  F2_PhaseFraction.nextItem = &F2_MHz; //loop back to MHz item
  // **Freq2 Prev Items**
  F2_PhaseFraction.prevItem = &F2_Phase;
  F2_Phase.prevItem = &F2_Amplitude;
  F2_Amplitude.prevItem = &F2_Hz;
  F2_Hz.prevItem = &F2_kHz;
  F2_kHz.prevItem = &F2_MHz;
  F2_MHz.prevItem = &F2_PhaseFraction; //loop back to PhaseFractio item

    // ** Freq3 Next Items**
  F3_MHz.nextItem = &F3_kHz;
  F3_kHz.nextItem = &F3_Hz;
  F3_Hz.nextItem = &F3_Amplitude;
  F3_Amplitude.nextItem = &F3_Phase; 
  F3_Phase.nextItem = &F3_PhaseFraction;
  F3_PhaseFraction.nextItem = &F3_MHz; //loop back to MHz item
  // **Freq3 Prev Items**
  F3_PhaseFraction.prevItem = &F3_Phase;
  F3_Phase.prevItem = &F3_Amplitude;
  F3_Amplitude.prevItem = &F3_Hz;
  F3_Hz.prevItem = &F3_kHz;
  F3_kHz.prevItem = &F3_MHz;
  F3_MHz.prevItem = &F3_PhaseFraction; //loop back to PhaseFractio item

  // **** Parent/Child ****
  F0.childItem=&F0_MHz;
  F1.childItem=&F1_MHz;
  F2.childItem=&F2_MHz;
  F3.childItem=&F3_MHz;
  
  F0_MHz.parentItem=&F0;
  F0_kHz.parentItem=&F0;
  F0_Hz.parentItem=&F0;
  F0_Amplitude.parentItem=&F0;
  F0_Phase.parentItem=&F0;
  F0_PhaseFraction.parentItem=&F0;

  F1_MHz.parentItem=&F1;
  F1_kHz.parentItem=&F1;
  F1_Hz.parentItem=&F1;
  F1_Amplitude.parentItem=&F1;
  F1_Phase.parentItem=&F1;
  F1_PhaseFraction.parentItem=&F1;

  F2_MHz.parentItem=&F2;
  F2_kHz.parentItem=&F2;
  F2_Hz.parentItem=&F2;
  F2_Amplitude.parentItem=&F2;
  F2_Phase.parentItem=&F2;
  F2_PhaseFraction.parentItem=&F2;

  F3_MHz.parentItem=&F3;
  F3_kHz.parentItem=&F3;
  F3_Hz.parentItem=&F3;
  F3_Amplitude.parentItem=&F3;
  F3_Phase.parentItem=&F3;
  F3_PhaseFraction.parentItem=&F3;

  // ** Core clock menu items linkning **
  // ** Next Items**
  ClockSrc.nextItem=&ClockFreq;
  ClockFreq.nextItem=&DDSCoreClock;
  DDSCoreClock.nextItem=&CoreClockSave;
  CoreClockSave.nextItem=&CoreClockExit;
  CoreClockExit.nextItem=&ClockSrc; //loop back to ClockSrc item

  ExtClockFreqMHz.nextItem=&CoreClockSave;
  
  // ** Prev Items**
  CoreClockExit.prevItem=&CoreClockSave;
  CoreClockSave.prevItem=&DDSCoreClock;
  DDSCoreClock.prevItem=&ClockFreq;
  ClockFreq.prevItem=&ClockSrc;
  ClockSrc.prevItem=&CoreClockExit; //loop back to CoreClockExit item

  ExtClockFreqMHz.prevItem=&ClockSrc;

  //********parents**************
  ClockSrc.parentItem=&F0;
  ClockFreq.parentItem=&F0;
  DDSCoreClock.parentItem=&F0;
  CoreClockSave.parentItem=&F0;
  CoreClockExit.parentItem=&F0;
}

void MenuInitValues()
{
  F0_MHz.maxValue=MAX_OUT_FREQ;
  F0_kHz.maxValue=999;
  F0_Hz.maxValue=999;
  F0_Amplitude.minValue=7;
  F0_Amplitude.maxValue=60;
  F0_Phase.maxValue=360;
  F0_PhaseFraction.maxValue=9;

  F1_MHz.maxValue=MAX_OUT_FREQ;
  F1_kHz.maxValue=999;
  F1_Hz.maxValue=999;
  F1_Amplitude.minValue=7;
  F1_Amplitude.maxValue=60;
  F1_Phase.maxValue=360;
  F1_PhaseFraction.maxValue=9;

  F2_MHz.maxValue=MAX_OUT_FREQ;
  F2_kHz.maxValue=999;
  F2_Hz.maxValue=999;
  F2_Amplitude.minValue=7;
  F2_Amplitude.maxValue=60;
  F2_Phase.maxValue=360;
  F2_PhaseFraction.maxValue=9;

  F3_MHz.maxValue=MAX_OUT_FREQ;
  F3_kHz.maxValue=999;
  F3_Hz.maxValue=999;
  F3_Amplitude.minValue=7;
  F3_Amplitude.maxValue=60;
  F3_Phase.maxValue=360;
  F3_PhaseFraction.maxValue=9;

  ClockSrc.maxValue=1;
  ClockSrc.minValue=0;
  
  ClockFreq.maxValue=4;
  ClockFreq.minValue=0;

  //ui32CurrentOutputFreq=GetFreq();
  
  DDSCoreClock.DDSCoreClock=DDS_Core_Clock;
  ExtClockFreqMHz.minValue=ExtMHzMin;
  ExtClockFreqMHz.maxValue=ExtMHzMax;
  DDSCoreClock.minValue=N_MIN_VALUE;
  DDSCoreClock.maxValue=N_MAX_VALUE;

  //***********
  if (DDSCoreClock.value > N_MAX_VALUE) DDSCoreClock.value=DDSCoreClock.FindMinimalN();
  if (ClockFreq.Ref_Clk[ClockFreq.value]*(DDSCoreClock.value)*2 > MAX_DDS_CORE_CLOCK_PLL) DDSCoreClock.value=DDSCoreClock.FindMinimalN();

  if (DDSCoreClock.value < N_MIN_VALUE) DDSCoreClock.value=DDSCoreClock.FindMaximalN();
  if (ClockFreq.Ref_Clk[ClockFreq.value]*(DDSCoreClock.value)*2 < MIN_DDS_CORE_CLOCK_PLL) DDSCoreClock.value=DDSCoreClock.FindMaximalN();
  //**********
  DDSCoreClock.SetDDSCoreClock();

  //BASE_EXT_DDS_CORE_CLOCK
  ExtClockFreqMHz.value=BASE_EXT_DDS_CORE_CLOCK/1000000; //4del
}

void LoadMainSettings()
{
  if (EEPROM.read(MAIN_SETTINGS_FLAG_ADR)!=55)
  {  
    F0_MHz.value=INIT_MHZ;
    F0_kHz.value=INIT_KHZ;
    F0_Hz.value=INIT_HZ;
    F0_Amplitude.value=INIT_AMPLITUDE;
    F0_Phase.value=INIT_PHASE;
    F0_PhaseFraction.value=INIT_PHASE_FRACTION;

    F1_MHz.value=INIT_MHZ;
    F1_kHz.value=INIT_KHZ;
    F1_Hz.value=INIT_HZ;
    F1_Amplitude.value=INIT_AMPLITUDE;
    F1_Phase.value=INIT_PHASE;
    F1_PhaseFraction.value=INIT_PHASE_FRACTION;

    F2_MHz.value=INIT_MHZ;
    F2_kHz.value=INIT_KHZ;
    F2_Hz.value=INIT_HZ;
    F2_Amplitude.value=INIT_AMPLITUDE;
    F2_Phase.value=INIT_PHASE;
    F2_PhaseFraction.value=INIT_PHASE_FRACTION;

    F3_MHz.value=INIT_MHZ;
    F3_kHz.value=INIT_KHZ;
    F3_Hz.value=INIT_HZ;
    F3_Amplitude.value=INIT_AMPLITUDE;
    F3_Phase.value=INIT_PHASE;
    F3_PhaseFraction.value=INIT_PHASE_FRACTION;
    SaveMainSettings();
    
    #if DBG==1
    PrintValues();
    #endif
  }
  else
  {
    EEPROM.get(F0_MHZ_ADR, F0_MHz.value);
    EEPROM.get(F0_KHZ_ADR, F0_kHz.value);
    EEPROM.get(F0_HZ_ADR, F0_Hz.value);
    EEPROM.get(F0_AMPLITUDE_ADR, F0_Amplitude.value);
    EEPROM.get(F0_PHASE_ADR, F0_Phase.value);
    EEPROM.get(F0_PHASE_FRACTION_ADR, F0_PhaseFraction.value);

    EEPROM.get(F1_MHZ_ADR, F1_MHz.value);
    EEPROM.get(F1_KHZ_ADR, F1_kHz.value);
    EEPROM.get(F1_HZ_ADR, F1_Hz.value);
    EEPROM.get(F1_AMPLITUDE_ADR, F1_Amplitude.value);
    EEPROM.get(F1_PHASE_ADR, F1_Phase.value);
    EEPROM.get(F1_PHASE_FRACTION_ADR, F1_PhaseFraction.value);

    EEPROM.get(F2_MHZ_ADR, F2_MHz.value);
    EEPROM.get(F2_KHZ_ADR, F2_kHz.value);
    EEPROM.get(F2_HZ_ADR, F2_Hz.value);
    EEPROM.get(F2_AMPLITUDE_ADR, F2_Amplitude.value);
    EEPROM.get(F2_PHASE_ADR, F2_Phase.value);
    EEPROM.get(F2_PHASE_FRACTION_ADR, F2_PhaseFraction.value);
    
    EEPROM.get(F3_MHZ_ADR, F3_MHz.value);
    EEPROM.get(F3_KHZ_ADR, F3_kHz.value);
    EEPROM.get(F3_HZ_ADR, F3_Hz.value);
    EEPROM.get(F3_AMPLITUDE_ADR, F3_Amplitude.value);
    EEPROM.get(F3_PHASE_ADR, F3_Phase.value);
    EEPROM.get(F3_PHASE_FRACTION_ADR, F3_PhaseFraction.value);
    
    #if DBG==1
    Serial.println(F("Value from EEPROM"));
    PrintValues();
    #endif
  }
}

void SaveMainSettings()
{
  EEPROM.put(F0_MHZ_ADR, F0_MHz.value);
  EEPROM.put(F0_KHZ_ADR, F0_kHz.value);
  EEPROM.put(F0_HZ_ADR, F0_Hz.value);
  EEPROM.put(F0_AMPLITUDE_ADR, F0_Amplitude.value);
  EEPROM.put(F0_PHASE_ADR, F0_Phase.value);
  EEPROM.put(F0_PHASE_FRACTION_ADR, F0_PhaseFraction.value);

  EEPROM.put(F1_MHZ_ADR, F1_MHz.value);
  EEPROM.put(F1_KHZ_ADR, F1_kHz.value);
  EEPROM.put(F1_HZ_ADR, F1_Hz.value);
  EEPROM.put(F1_AMPLITUDE_ADR, F1_Amplitude.value);
  EEPROM.put(F1_PHASE_ADR, F1_Phase.value);
  EEPROM.put(F1_PHASE_FRACTION_ADR, F1_PhaseFraction.value);

  EEPROM.put(F2_MHZ_ADR, F2_MHz.value);
  EEPROM.put(F2_KHZ_ADR, F2_kHz.value);
  EEPROM.put(F2_HZ_ADR, F2_Hz.value);
  EEPROM.put(F2_AMPLITUDE_ADR, F2_Amplitude.value);
  EEPROM.put(F2_PHASE_ADR, F2_Phase.value);
  EEPROM.put(F2_PHASE_FRACTION_ADR, F2_PhaseFraction.value);
  
  EEPROM.put(F3_MHZ_ADR, F3_MHz.value);
  EEPROM.put(F3_KHZ_ADR, F3_kHz.value);
  EEPROM.put(F3_HZ_ADR, F3_Hz.value);
  EEPROM.put(F3_AMPLITUDE_ADR, F3_Amplitude.value);
  EEPROM.put(F3_PHASE_ADR, F3_Phase.value);
  EEPROM.put(F3_PHASE_FRACTION_ADR, F3_PhaseFraction.value);
  
  EEPROM.write(MAIN_SETTINGS_FLAG_ADR,55);
}

#if DBG==1
void PrintValues()
{
    Serial.print(F("F0_MHz="));
    Serial.println(F0_MHz.value);
    Serial.print(F("F0_kHz="));
    Serial.println(F0_kHz.value);
    Serial.print(F("F0_Hz="));
    Serial.println(F0_Hz.value);
    Serial.print(F("F0_Amplitude="));
    Serial.println(F0_Amplitude.value);
    Serial.print(F("F0_Phase="));
    Serial.println(F0_Phase.value);
    Serial.print(F("F0_PhaseFraction="));
    Serial.println(F0_PhaseFraction.value);
    
    Serial.print(F("F1_MHz="));
    Serial.println(F1_MHz.value);
    Serial.print(F("F1_kHz="));
    Serial.println(F1_kHz.value);
    Serial.print(F("F1_Hz="));
    Serial.println(F1_Hz.value);
    Serial.print(F("F1_Amplitude="));
    Serial.println(F1_Amplitude.value);
    Serial.print(F("F1_Phase="));
    Serial.println(F1_Phase.value);
    Serial.print(F("F1_PhaseFraction="));
    Serial.println(F1_PhaseFraction.value);

    Serial.print(F("F2_MHz="));
    Serial.println(F2_MHz.value);
    Serial.print(F("F2_kHz="));
    Serial.println(F2_kHz.value);
    Serial.print(F("F2_Hz="));
    Serial.println(F2_Hz.value);
    Serial.print(F("F2_Amplitude="));
    Serial.println(F2_Amplitude.value);
    Serial.print(F("F2_Phase="));
    Serial.println(F2_Phase.value);
    Serial.print(F("F2_PhaseFraction="));
    Serial.println(F2_PhaseFraction.value);
    
    Serial.print(F("F3_MHz="));
    Serial.println(F3_MHz.value);
    Serial.print(F("F3_kHz="));
    Serial.println(F3_kHz.value);
    Serial.print(F("F3_Hz="));
    Serial.println(F3_Hz.value);
    Serial.print(F("F3_Amplitude="));
    Serial.println(F3_Amplitude.value);
    Serial.print(F("F3_Phase="));
    Serial.println(F3_Phase.value);
    Serial.print(F("F3_PhaseFraction="));
    Serial.println(F3_PhaseFraction.value);
}
#endif

void LoadClockSettings()
{
  if (EEPROM.read(CLOCK_SETTINGS_FLAG_ADR)!=55)
  {
    ClockSrc.value=INIT_CLOCK_SOURCE_INDEX;
    ClockFreq.value=INIT_CLOCK_FREQ_INDEX;
    ExtClockFreqMHz.value=INIT_EXT_CLOCK_FREQ_MHZ;
    DDSCoreClock.value=INIT_DDS_CORE_CLOCK_N;
    #if DBG==1
    Serial.println(F("Set default clock's settings"));
    Serial.print(F("ClockSrc.value="));
    Serial.println(ClockSrc.value);
    Serial.print(F("ClockFreq.value="));
    Serial.println(ClockFreq.value);
    Serial.print(F("ExtClockFreqMHz.value="));
    Serial.println(ExtClockFreqMHz.value);
    Serial.print(F("DDSCoreClock.value="));
    Serial.println(DDSCoreClock.value);
    #endif

    SaveClockSettings();
  } else
  {
    EEPROM.get(CLOCK_SOURCE_ADR, ClockSrc.value);
    EEPROM.get(CLOCK_TCXO_FREQ_INDEX_ADR, ClockFreq.value);
    EEPROM.get(EXT_CLOCK_FREQ_MHZ_ADR, ExtClockFreqMHz.value);
    EEPROM.get(DDS_CORE_CLOCK_N_ADR, DDSCoreClock.value);

    #if DBG==1
    Serial.println(F("Reading clock's settings from EEPROM"));
    Serial.print(F("ClockSrc.value="));
    Serial.println(ClockSrc.value);
    Serial.print(F("ClockFreq.value="));
    Serial.println(ClockFreq.value);
    Serial.print(F("ExtClockFreqMHz.value="));
    Serial.println(ExtClockFreqMHz.value);
    Serial.print(F("DDSCoreClock.value="));
    Serial.println(DDSCoreClock.value);
    #endif
  }

  //***********
  if (DDSCoreClock.value > N_MAX_VALUE) DDSCoreClock.value=DDSCoreClock.FindMinimalN();
  if (ClockFreq.Ref_Clk[ClockFreq.value]*(DDSCoreClock.value) > MAX_DDS_CORE_CLOCK_PLL) DDSCoreClock.value=DDSCoreClock.FindMinimalN();

  if (DDSCoreClock.value < N_MIN_VALUE) DDSCoreClock.value=DDSCoreClock.FindMaximalN();
  if (ClockFreq.Ref_Clk[ClockFreq.value]*(DDSCoreClock.value) < MIN_DDS_CORE_CLOCK_PLL) DDSCoreClock.value=DDSCoreClock.FindMaximalN();
  //**********
  DDSCoreClock.SetDDSCoreClock();

  if (ClockSrc.value==TCXO_OCXO) dds.setClock(DDSCoreClock.value, ClockFreq.Ref_Clk[ClockFreq.value], 0);
        else dds.setClock(0, DDSCoreClock.GetDDSCoreClock(), 0);

  #if DBG==1
  Serial.print("DDSCoreClock.value=");
  Serial.println(DDSCoreClock.value);

  Serial.print("ClockFreq.Ref_Clk[ClockFreq.value]=");
  Serial.println(ClockFreq.Ref_Clk[ClockFreq.value]);
  #endif

  if (ClockSrc.value==TCXO_OCXO)
  {
    ClockSrc.nextItem=&ClockFreq;
    CoreClockSave.prevItem=&DDSCoreClock;
  } else
    {
      ClockSrc.nextItem=&ExtClockFreqMHz;
      CoreClockSave.prevItem=&ExtClockFreqMHz;
    }

  DDS_Core_Clock = DDSCoreClock.GetDDSCoreClock();
  ui32HIGH_FREQ_LIMIT = DDS_Core_Clock * 0.45;
}

void SaveClockSettings()
{
    EEPROM.put(CLOCK_SOURCE_ADR, ClockSrc.value);
    EEPROM.put(CLOCK_TCXO_FREQ_INDEX_ADR, ClockFreq.value);
    EEPROM.put(EXT_CLOCK_FREQ_MHZ_ADR, ExtClockFreqMHz.value);
    EEPROM.put(DDS_CORE_CLOCK_N_ADR, DDSCoreClock.value);
    
    EEPROM.write(CLOCK_SETTINGS_FLAG_ADR, 55);

    
    //DisplayMessage("SETUP", "SAVED");
    DisplaySaved();
    delay(1000);
}

void DisplaySaved(void)
{
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setCursor(51, 8);
  u8g2.println("SETUP");
  u8g2.setCursor(51, 40);
  u8g2.print("SAVED");
  
  u8g2.sendBuffer();
}
