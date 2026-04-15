//  ******************************************** HARDWARE IMPORTANT********************************************************
//  Tested Pico 22/3/26 - working
//  MOSI (pin D3 pin 5) to ADF4351 DATA
//  SCK (pin D2 pin 4) to ADF CLK
//  Select (PIN D5 pin 7) to ADF LE
//  ** on "new" board, CS must be pulled high by 10K to 3V3
//  ******************************************** HARDWARE IMPORTANT********************************************************


#include <SPI.h>

#define ADF4351_LE 5

SPISettings spisettings(1000000, MSBFIRST, SPI_MODE0);


//uint32_t registers[6] =  {0x4E0358, 0x80087D1, 0x4E42, 0x4B3, 0x8C803C, 0x580005} ; 
//uint32_t registers[6] =  {0xC80000, 0x8008011, 0x4E42, 0x4B3, 0xB503FC, 0x580005} ; 
//uint32_t registers[6] =  {0x960000, 0x8008011, 0x4E42, 0x4B3, 0x9503FC, 0x580005} ; // 1500MHz 10MHz ref clock
uint32_t registers[6] =  {0x300000, 0x8008011, 0x4E42, 0x4B3, 0x9503FC, 0x580005} ; // 1200MHz 25MHz ref clock

double RFout, REFin, PFDRFout, OutputChannelSpacing, FRACF;
unsigned int long RFint,RFintold, INTA, MOD, FRAC;
int modif = 0;
byte OutputDivider;

void WriteRegister32(const uint32_t value)   
{
  digitalWrite(ADF4351_LE, LOW);
  for (int i = 3; i >= 0; i--) 
  {            // loop round 4 x 8bits
    SPI.beginTransaction(spisettings);
    SPI.transfer((value >> 8 * i) & 0xFF); // offset, byte mask and send via SPI
  }
  digitalWrite(ADF4351_LE, HIGH);
  delayMicroseconds(2);
  digitalWrite(ADF4351_LE, LOW);
}

void SetADF4351()  // bung the data into the ADF4351
{ for (int i = 5; i >= 0; i--)
    WriteRegister32(registers[i]);
}

void SerialPrintRegisters()
{   
    for (int i = 0; i <=5; i++)
    {
      Serial.print("Register ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(registers[i],HEX);
      Serial.print(" ");
    }
  Serial.println();
}

void setup() 
{
  Serial.begin (9600);

  pinMode(ADF4351_LE, OUTPUT);          
  digitalWrite(ADF4351_LE, HIGH);
  SPI.setRX(0);
  SPI.setCS(1);
  SPI.setSCK(2);
  SPI.setTX(3);
  SPI.begin(true);

  delay(5000);
}
  

void loop() 
{
  delay (2000);

  SetADF4351();  
  SerialPrintRegisters();
}