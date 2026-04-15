const int CLOCK = 11;
const int FQ_UD = 10;
const int DATA = 9;
const int RST = 8;

const unsigned long DDSClock = 125000000;

const unsigned long targetFrequency = 10000000;
const uint8_t targetPhase = 0;

void DDSReset()
{
  digitalWrite(DATA, LOW);
  digitalWrite(FQ_UD, LOW);
  digitalWrite(CLOCK, LOW);

  pulseHigh(RST);
  pulseHigh(CLOCK);
  pulseHigh(FQ_UD);
}

void pulseHigh (int pin)
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(2);
  digitalWrite(pin, LOW);
}

unsigned long calculateDataWord(unsigned long freq)
{
  unsigned long result = 0;

  result = (freq * pow(2,32)) / DDSClock;

  return result;
}

void setFrequency (unsigned long targetFrequency, uint8_t targetPhase)
{
  unsigned long dataWord;
  dataWord = calculateDataWord(targetFrequency * 1UL);

  // send 32 bit frequency tuning data word
  for (int i=0; i<32; i++)
  {
    digitalWrite(DATA, (dataWord & 0x01));
    pulseHigh(CLOCK);

    dataWord >>= 1;
  }

  //now send phase value
  unsigned long lowerByte = 0;
  lowerByte = (targetPhase << 3);
  lowerByte = lowerByte + 0; // no clock multiplier on 9850, add 1 for AD9851

  for (int i=0; i<8; i++)
  {
    
    digitalWrite (DATA, lowerByte & 0x01);
    pulseHigh(CLOCK);

    lowerByte = lowerByte << 1;
  }
  pulseHigh(FQ_UD);
}

void setup() {
  // put your setup code here, to run once:
 
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(FQ_UD, OUTPUT);
  pinMode(RST, OUTPUT);

  DDSReset();
}

void loop() {
  // put your main code here, to run repeatedly:
   setFrequency (targetFrequency, targetPhase);
   delay(1000);
}
