#ifndef DISPLAY_MENU_H
#define DISPLAY_MENU_H

void DisplayMenu(uint8_t menuType);
void DisplayHello();
extern String PreZero(int Digit, uint8_t Qty=3);

class ScrollingText
{
  public:
  ScrollingText(String text, uint8_t qty, uint16_t scrollDelay)
  {
    String sTempString;

    ui8NumberOfVisibleCharacters=qty;
    for (int i=0; i<ui8NumberOfVisibleCharacters; i++)
    {
      sTempString=sTempString+" ";
    }

    sText=sTempString+text;
    ui8TextLength=sText.length();
  
    //SetText(text);
    ui16ScrollingDelayMS=scrollDelay;
    //ui8TextLength=sText.length();
  }
  
  String GetText()
  {
    ui32NowMS=millis();
    if (ui32NowMS-ui32LastTimeScrolled>ui16ScrollingDelayMS*3) ui8_CurrentPos=0;
    if (ui32NowMS-ui32LastTimeScrolled>ui16ScrollingDelayMS) 
    {
      ui32LastTimeScrolled=ui32NowMS;
      ui8_CurrentPos++;
      if (ui8_CurrentPos>ui8TextLength) ui8_CurrentPos=0;
      sCurrentString=sText.substring(ui8_CurrentPos, ui8_CurrentPos+ui8NumberOfVisibleCharacters);
    }
    return sCurrentString;
  }

  void SetText(String sNewText)
  {
    sCurrentString="";
    ui8_CurrentPos=0;
    String sTempString;
    for (int i=0; i<ui8NumberOfVisibleCharacters; i++)
    {
      sTempString=sTempString+" ";
    }

    sText=sTempString+sNewText;
    ui8TextLength=sText.length();
  }
  
  private:
    String sText;
    uint8_t ui8NumberOfVisibleCharacters;
    uint16_t ui16ScrollingDelayMS;
    uint32_t ui32LastTimeScrolled=0;
    uint8_t ui8_CurrentPos=0;
    uint32_t ui32NowMS=0;
    uint8_t ui8TextLength=0;
    String sCurrentString="";
};

 void DrawBackground();

#endif
