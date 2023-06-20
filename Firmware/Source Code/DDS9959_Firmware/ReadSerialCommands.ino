/*
By default, the maximum length of one message is 64 bytes, you can change it in HardwareSerial.h, string #46: #define SERIAL_TX_BUFFER_SIZE 64
*/

int C=-1; //Номер канала(выхода) для управления, по умолчанию не задан (-1), допустимые значения: 0 - 3

#define SERIAL_PACKAGE_MAX_LENGTH 110
char Buff[SERIAL_PACKAGE_MAX_LENGTH];

const char HELP_STRING [] PROGMEM = "C — Set the current output Channel: (0 — 3)\n"
          "F — Sets Frequency in Hz (100000 — 225000000)\n"
          "A — Sets the power (Amplitude) level of the selected channel in dBm (-60 — -7)\n"
          "P — Sets the Phase of the selected channel in dBm (0 — 360)\n"
          "M — Gets Model\n"
          "V — Gets Firmware Version\n"
          "h — This Help\n"
          "; — Commands Separator"
          "\n"
          "Example:\n"
          "C0;F100000;A-10\n"
          "Sets the Frequency to 100 kHz, and Output Power (Amplitude) to -10 dBm on Channel 0 (RF OUT0).\n"
          "Any number of commands in any order is allowed, but the very first command must be \"C\".\n"
          "Note: by default, the maximum length of one message is 64 bytes";


bool inRange(int32_t val, int32_t minimum, int32_t maximum)
{
  return ((minimum <= val) && (val <= maximum));
}

void ReadSerialCommands()
{
  if (!Serial.available()) return;
  int RcvCounter=0;
  RcvCounter = Serial.readBytesUntil('\n', Buff, 110);
  if (RcvCounter == 0) return;
  Buff[RcvCounter]='\0';
  
  int32_t value=0;
  char command;

    GParser data(Buff, ';');
    int commandsCounter = data.split();

    for (int i=0; i < commandsCounter; i++)
    {
      sscanf(data[i], "%c%ld", &command, &value);
      switch (command)
      {

        case 'C': //Current Channel (0 - 3)
          if (inRange(value, 0, 3))
          {
            Serial.print(F("The Channel number is set to: "));
            Serial.println(value);
            C = value;
          } else Serial.println(F("The Channel number is OUT OF RANGE (0 — 3)"));
        break;

        case 'F': //RF Frequency
          if (C==-1) {Serial.println(F("The output Channel is not selected! Use \"C\" command to select the Channel.")); return;}
          if (inRange(value, LOW_FREQ_LIMIT, ui32HIGH_FREQ_LIMIT))
          {
            Serial.print(F("The Frequency of Channel "));
            Serial.print(C);
            Serial.print(F(" is set to: "));
            Serial.println(value);
            uint16_t H, K, M;
            H = value % 1000;
            K = (value / 1000) % 1000;
            M = value / 1000000;
            switch (C)
            {
              case 0:
                F0_Hz.value = H;
                F0_kHz.value = K;
                F0_MHz.value = M;
                F0OutputFreq = value;
              break;
              case 1:
                F1_Hz.value = H;
                F1_kHz.value = K;
                F1_MHz.value = M;
                F0OutputFreq = value;
              break;
              case 2:
                F2_Hz.value = H;
                F2_kHz.value = K;
                F2_MHz.value = M;
                F0OutputFreq = value;
              break;
              case 3:
                F3_Hz.value = H;
                F3_kHz.value = K;
                F3_MHz.value = M;
                F0OutputFreq = value;
              break;
            }
          } else Serial.println("Frequency is OUT OF RANGE (" + String(LOW_FREQ_LIMIT) + " - " + String(ui32HIGH_FREQ_LIMIT) + ")");
        break;

        case 'A': //Power(Amplitude), dBm -60 - -7
          if (C==-1) {Serial.println(F("The output Channel is not selected! Use \"C\" command to select the Channel.")); return;}
          if (inRange(value, -60, -7))
          {
            Serial.print(F("The Power (Amplitude) of Channel "));
            Serial.print(C);
            Serial.print(F(" is set to: "));
            Serial.println(value);
            switch (C)
            {
              case 0:
                F0_Amplitude.value = -1 * value;
              break;
              case 1:
                F1_Amplitude.value = -1 * value;
              break;
              case 2:
                F2_Amplitude.value = -1 * value;
              break;
              case 3:
                F3_Amplitude.value = -1 * value;
              break;
            }
          } else Serial.println("Power is OUT OF RANGE (-60 — -7)");
        break;

        case 'P': //Phase, 0.0 - 360.0
          if (C==-1) {Serial.println(F("The output Channel is not selected! Use \"C\" command to select the Channel.")); return;}
          if (inRange(value, 0, 360))
          {
            Serial.print(F("The Phase of Channel "));
            Serial.print(C);
            Serial.print(F(" is set to: "));
            Serial.println(value);
            switch (C)
            {
              case 0:
                F0_Phase.value = value;
              break;
              case 1:
                F1_Phase.value = value;
              break;
              case 2:
                F2_Phase.value = value;
              break;
              case 3:
                F3_Phase.value = value;
              break;
            }
          } else Serial.println("Phase is OUT OF RANGE (0 — 360)");
        break;

        case 'V': //Firmware Version request
          Serial.println(FIRMWAREVERSION);
          //Serial.println(value);
        break;

        case 'M': //Model request
          Serial.println(F("DDS9959 v1.1"));
          //Serial.println(value);
        break;

        case 'h': //Model request
          Serial.println((const __FlashStringHelper *) HELP_STRING);
        break;

        default:
        Serial.print(F("Unknown command:"));
        Serial.println(command);
        Serial.println((const __FlashStringHelper *) HELP_STRING);
      } //switch
    } //for

    DisplayMenu(menuType);
    ApplyChangesToDDS();
}