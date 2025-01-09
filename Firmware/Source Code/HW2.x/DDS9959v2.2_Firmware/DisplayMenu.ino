#include "DisplayMenu.h"

void DisplayMenu(uint8_t menuType) {
  //do   {
  switch (menuType) {
    case MAIN_MENU:
      u8g2.setCursor(58, 9);
      if (isPWR_DWN) {
        u8g2.print(F("OFF"));
      } else {
        u8g2.print(F("   "));
      }

      // *********** First ROW Start ***********
      u8g2.setCursor(2, 20);
      if (curItem == &F0) u8g2.setDrawColor(0);
      else u8g2.setDrawColor(1);
      u8g2.print(F("F0"));
      u8g2.setDrawColor(1);
      u8g2.setCursor(17, 20);
      if (curItem == &F0_MHz) {
        if (F0_MHz.bBlink == true) u8g2.setDrawColor(F0_MHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      //u8g2.print("299");
      u8g2.print(PreZero(F0_MHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F0_kHz) {
        if (F0_kHz.bBlink == true) u8g2.setDrawColor(F0_kHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F0_kHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F0_Hz) {
        if (F0_Hz.bBlink == true) u8g2.setDrawColor(F0_Hz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F0_Hz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.setCursor(77, 20);
      if (curItem == &F0_Amplitude) {
        if (F0_Amplitude.bBlink == true) u8g2.setDrawColor(F0_Amplitude.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F("-"));
      u8g2.print(PreZero(F0_Amplitude.value, 2));
      u8g2.setDrawColor(1);
      u8g2.setCursor(97, 20);
      if (curItem == &F0_Phase) {
        if (F0_Phase.bBlink == true) u8g2.setDrawColor(F0_Phase.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F0_Phase.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F0_PhaseFraction) {
        if (F0_PhaseFraction.bBlink == true) u8g2.setDrawColor(F0_PhaseFraction.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F0_PhaseFraction.value);
      u8g2.setDrawColor(1);
      u8g2.print(F("{"));
      // *********** First ROW End ***********

      // *********** 2nd ROW Start ***********
      u8g2.setCursor(2, 30);
      if (curItem == &F1) u8g2.setDrawColor(0);
      else u8g2.setDrawColor(1);
      u8g2.print(F("F1"));
      u8g2.setDrawColor(1);
      u8g2.setCursor(17, 30);
      if (curItem == &F1_MHz) {
        if (F1_MHz.bBlink == true) u8g2.setDrawColor(F1_MHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F1_MHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F1_kHz) {
        if (F1_kHz.bBlink == true) u8g2.setDrawColor(F1_kHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F1_kHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F1_Hz) {
        if (F1_Hz.bBlink == true) u8g2.setDrawColor(F1_Hz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F1_Hz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.setCursor(77, 30);
      if (curItem == &F1_Amplitude) {
        if (F1_Amplitude.bBlink == true) u8g2.setDrawColor(F1_Amplitude.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F("-"));
      u8g2.print(PreZero(F1_Amplitude.value, 2));
      u8g2.setDrawColor(1);
      u8g2.setCursor(97, 30);
      if (curItem == &F1_Phase) {
        if (F1_Phase.bBlink == true) u8g2.setDrawColor(F1_Phase.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F1_Phase.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F1_PhaseFraction) {
        if (F1_PhaseFraction.bBlink == true) u8g2.setDrawColor(F1_PhaseFraction.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F1_PhaseFraction.value);
      u8g2.setDrawColor(1);
      u8g2.print(F("{"));
      // *********** 2nd ROW End ***********

      // *********** 3nd ROW Start ***********
      u8g2.setCursor(2, 40);
      if (curItem == &F2) u8g2.setDrawColor(0);
      else u8g2.setDrawColor(1);
      u8g2.print(F("F2"));
      u8g2.setDrawColor(1);
      u8g2.setCursor(17, 40);
      if (curItem == &F2_MHz) {
        if (F2_MHz.bBlink == true) u8g2.setDrawColor(F2_MHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F2_MHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F2_kHz) {
        if (F2_kHz.bBlink == true) u8g2.setDrawColor(F2_kHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F2_kHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F2_Hz) {
        if (F2_Hz.bBlink == true) u8g2.setDrawColor(F2_Hz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F2_Hz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.setCursor(77, 40);
      if (curItem == &F2_Amplitude) {
        if (F2_Amplitude.bBlink == true) u8g2.setDrawColor(F2_Amplitude.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F("-"));
      u8g2.print(PreZero(F2_Amplitude.value, 2));
      u8g2.setDrawColor(1);
      u8g2.setCursor(97, 40);
      if (curItem == &F2_Phase) {
        if (F2_Phase.bBlink == true) u8g2.setDrawColor(F2_Phase.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F2_Phase.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F2_PhaseFraction) {
        if (F2_PhaseFraction.bBlink == true) u8g2.setDrawColor(F2_PhaseFraction.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F2_PhaseFraction.value);
      u8g2.setDrawColor(1);
      u8g2.print(F("{"));
      // *********** 3nd ROW End ***********

      // *********** 4th ROW Start ***********
      u8g2.setCursor(2, 50);
      if (curItem == &F3) u8g2.setDrawColor(0);
      else u8g2.setDrawColor(1);
      u8g2.print(F("F3"));
      u8g2.setDrawColor(1);
      u8g2.setCursor(17, 50);
      if (curItem == &F3_MHz) {
        if (F3_MHz.bBlink == true) u8g2.setDrawColor(F3_MHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F3_MHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F3_kHz) {
        if (F3_kHz.bBlink == true) u8g2.setDrawColor(F3_kHz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F3_kHz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F3_Hz) {
        if (F3_Hz.bBlink == true) u8g2.setDrawColor(F3_Hz.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F3_Hz.value, 3));
      u8g2.setDrawColor(1);
      u8g2.setCursor(77, 50);
      if (curItem == &F3_Amplitude) {
        if (F3_Amplitude.bBlink == true) u8g2.setDrawColor(F3_Amplitude.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F("-"));
      u8g2.print(PreZero(F3_Amplitude.value, 2));
      u8g2.setDrawColor(1);
      u8g2.setCursor(97, 50);
      if (curItem == &F3_Phase) {
        if (F3_Phase.bBlink == true) u8g2.setDrawColor(F3_Phase.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(PreZero(F3_Phase.value, 3));
      u8g2.setDrawColor(1);
      u8g2.print(F("."));
      if (curItem == &F3_PhaseFraction) {
        if (F3_PhaseFraction.bBlink == true) u8g2.setDrawColor(F3_PhaseFraction.GetColor());
        else u8g2.setDrawColor(0);
      } else u8g2.setDrawColor(1);
      u8g2.print(F3_PhaseFraction.value);
      u8g2.setDrawColor(1);
      u8g2.print(F("{"));
      // *********** 4th ROW End ***********

      /*int y=0;
  u8g2.setDrawColor(0);
  for (int i=0; i<22; i++)
  {
    u8g2.drawHLine(0, y, 128);
    y=y+2;
  }*/
      // ***** ERROR MESSAGES **********
      u8g2.setCursor(0, 61);
      switch (FreqInRange()) {
        case 0:
          u8g2.print(F("                "));
          break;
        case 1:
          u8g2.print(F("F0 Too HIGH Freq"));  // F0 Too HIGH FREQ
          break;
        case 2:
          u8g2.print(F("F1 Too HIGH Freq"));
          break;
        case 3:
          u8g2.print(F("F2 Too HIGH Freq"));
          break;
        case 4:
          u8g2.print(F("F3 Too HIGH Freq"));
          break;
        case -1:
          u8g2.print(F("F0 Too LOW Freq "));  // F0 Too LOW FREQ
          break;
        case -2:
          u8g2.print(F("F1 Too LOW Freq"));
          break;
        case -3:
          u8g2.print(F("F2 Too LOW Freq"));
          break;
        case -4:
          u8g2.print(F("F3 Too LOW Freq"));
          break;
      }
      //********ERROR MESSAGES END******

      u8g2.sendBuffer();
      //u8g2.setClipWindow(2, 12, 17, 52); //?
      break;
    case CORE_CLOCK_MENU:
      u8g2.setCursor(51, 8);
      u8g2.print(F("SETUP"));
      u8g2.setCursor(0, 26);
      u8g2.print(F("Clock Src:               "));  //Ext. TCXO/OCXO
 
      if (curItem == &ClockSrc) {
        if (ClockSrc.bBlink == true) u8g2.setDrawColor(ClockSrc.GetColor());
        else u8g2.setDrawColor(0);
      }
      u8g2.setCursor(55, 26);
      u8g2.print(ClockSrc.ClockSourceNames[ClockSrc.value]);

      u8g2.setDrawColor(1);
      //u8g2.print(" "); //////////
      u8g2.setCursor(0, 34);
      u8g2.print(F("Clock Freq: "));
      if ((ClockSrc.value == TCXO_OCXO) || (ClockSrc.value == EXT_TCXO_OCXO)) {
        if (curItem == &ClockFreq) {
          if (ClockFreq.bBlink == true) u8g2.setDrawColor(ClockFreq.GetColor());
          else u8g2.setDrawColor(0);
        }
        u8g2.print(ClockFreq.Ref_Clk[ClockFreq.value] / 1000000);
      } else {
        /*if (curItem == &ExtClockFreqGHz) {if (ExtClockFreqGHz.bBlink==true) u8g2.setDrawColor(ExtClockFreqGHz.GetColor()); else u8g2.setDrawColor(0);}
        u8g2.print(ExtClockFreqGHz.value);
        u8g2.setDrawColor(1);*/
        if (curItem == &ExtClockFreqMHz) {
          if (ExtClockFreqMHz.bBlink == true) u8g2.setDrawColor(ExtClockFreqMHz.GetColor());
          else u8g2.setDrawColor(0);
        }
        u8g2.print(PreZero(ExtClockFreqMHz.value));
      }
      u8g2.setDrawColor(1);
      u8g2.print(F(" MHz "));

      //*********
      u8g2.setCursor(0, 42);
      u8g2.print(F("Core Clock: "));
      u8g2.setDrawColor(1);
      //u8g2.print(" ");
      if (curItem == &DDSCoreClock) {
        if (DDSCoreClock.bBlink == true) u8g2.setDrawColor(DDSCoreClock.GetColor());
        else u8g2.setDrawColor(0);
      }
      //display.print(DDSCoreClock.DDSCoreClock / 1000000);
      u8g2.print((uint32_t)(DDSCoreClock.GetDDSCoreClock() / 1000000));
      /*Serial.print("DDSCoreClock.GetDDSCoreClock()"); //4del
      Serial.println((uint32_t)DDSCoreClock.GetDDSCoreClock()); //4del*/
      u8g2.setDrawColor(1);
      u8g2.print(F(" MHz "));
      //***********

      u8g2.setCursor(0, 63);
      if (curItem == &CoreClockSave) u8g2.setDrawColor(0);
      u8g2.println(F("SAVE"));

      u8g2.setDrawColor(1);
      u8g2.setCursor(103, 63);
      if (curItem == &CoreClockExit) u8g2.setDrawColor(0);
      u8g2.println(F("EXIT"));
      u8g2.setDrawColor(1);
      u8g2.sendBuffer();
      break;
  }
  //} while ( u8g2.nextPage() );
}

void DrawBackground() {
  u8g2.clearBuffer();
  u8g2.setCursor(2, 9);
  u8g2.print(F("Ch Freq,Hz     dBm Phase}"));

  u8g2.drawHLine(0, 0, 128);
  u8g2.drawHLine(0, 11, 128);
  u8g2.drawVLine(14, 0, 52);
  u8g2.drawVLine(74, 0, 52);
  u8g2.drawVLine(94, 0, 52);
  u8g2.drawHLine(0, 52, 128);
  u8g2.drawVLine(0, 0, 52);
  u8g2.drawVLine(127, 0, 52);
  u8g2.setCursor(0, 61);
  u8g2.setCursor(88, 61);
  u8g2.print(F("GRA&AFCH"));
  u8g2.sendBuffer();
}

String PreZero(int Digit, uint8_t Qty = 3) {
  switch (Qty) {
    case 3:
      if ((Digit < 100) && (Digit >= 10)) return "0" + String(Digit);
      if (Digit < 10) return "00" + String(Digit);
      return String(Digit);
    case 2:
      if (Digit < 10) return "0" + String(Digit);
      return String(Digit);
  }
}

void DisplayHello() {
  u8g2.setDrawColor(1);
  u8g2.setCursor(51, 8);
  u8g2.print(F("Hint:"));

  u8g2.setCursor(15, 25);
  u8g2.println(F("Push and hold Encoder"));
  u8g2.setCursor(15, 33);
  u8g2.println(F("   to enter Setup.   "));

  u8g2.setCursor(0, 63);
  u8g2.print(F("Firmware ver.: "));
  u8g2.print(FIRMWAREVERSION);

  u8g2.sendBuffer();
}
