#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <INA3221.h>

INA3221 ina3221(INA3221_ADDR40_GND);
LiquidCrystal_I2C lcd(0x27, 16, 4);

void fnvShowChannelInfo(int channel);
void fnvShowAllChannel(void);
void fnvReadPowerValues(int channel);

int timerReset = 0;
float current[3];
float current_compensated[3];
float voltage[3];
float power[3];

void setup()
{
   ina3221.begin();
   ina3221.reset();
   ina3221.setShuntRes(100, 100, 100);
   ina3221.setFilterRes(10, 10, 10);

   Serial.begin(115200);

   lcd.init();
   lcd.backlight();
   lcd.setCursor(0, 0);
   //  lcd.print("****************");
   lcd.setCursor(0, 1);
   lcd.print("  Measure Tool  ");
   lcd.setCursor(-4, 2);
   lcd.print(" Alec Solucoes");
   lcd.setCursor(-4, 3);
   //  lcd.print("****************");
   delay(2000);
   lcd.clear();
}

void loop()
{
   /*Exibe resumo dos 3 canais*/
   // fnvShowAllChannel();

   /*Exibe dados do canal escolhido*/
   // fnvShowChannelInfo(1);

   // delay(1000);

   for(int i = 1; i < 4; i++)
   {
      lcd.clear();
      fnvShowChannelInfo(i);
      delay(2000);
   }
}

/*
   Exibe os dados individuais de cada canal
   Tensao (V)
   Corrente (C)
   Potencia (P)
*/
void fnvShowChannelInfo(int channel)
{
   int correctChannel = channel - 1;

   fnvReadPowerValues(correctChannel);

   lcd.setCursor(0, 0);
   lcd.print("Channel ");
   lcd.print(channel);
   lcd.print(":");
   lcd.setCursor(0, 1);
   lcd.print("V: ");

   if (voltage[correctChannel] < 1)
   {
      lcd.print(voltage[correctChannel] * 1000, 0);
      lcd.print(" mV     ");
   }
   else
   {
      lcd.print(voltage[correctChannel], 2);
      lcd.print(" V     ");
   }

   lcd.setCursor(-4, 2);
   lcd.print("C: ");
   if (current_compensated[correctChannel] < 1)
   {
      lcd.print(current_compensated[correctChannel] * 1000, 0);
      lcd.print(" mA     ");
   }
   else
   {
      lcd.print(current_compensated[correctChannel], 2);
      lcd.print(" A     ");
   }

   lcd.setCursor(-4, 3);
   lcd.print("P: ");
   if (power[0] < 1)
   {
      lcd.print(power[correctChannel] * 1000, 0);
      lcd.print(" mW     ");
   }
   else
   {
      lcd.print(power[correctChannel], 2);
      lcd.print(" W     ");
   }
}

/*
  Tela geral - Exibe tensao e corrente dos 3 canais
*/
void fnvShowAllChannel(void)
{
   for(int i = 1; i < 4; i++)
   {
      fnvReadPowerValues(i);
   }

   lcd.setCursor(0, 0);
   lcd.print("Channels Info:");

   lcd.setCursor(0, 1);
   lcd.print("Ch1 " + String(voltage[0]) + "V" + String(current_compensated[0] * 1000 * 1.044, 0) + "mA");

   lcd.setCursor(-4, 2);
   lcd.print("Ch2 " + String(voltage[1]) + "V" + String(current_compensated[1] * 1000 * 1.044, 0) + "mA");

   lcd.setCursor(-4, 3);
   lcd.print("Ch3 " + String(voltage[2]) + "V" + String(current_compensated[2] * 1000 * 1.044, 0) + "mA");

}

void fnvReadPowerValues(int channel)
{
   int correctChannel = channel - 1;
   current[correctChannel] = ina3221.getCurrent((ina3221_ch_t) correctChannel);
   current_compensated[correctChannel] = ina3221.getCurrentCompensated((ina3221_ch_t) correctChannel)  * 1.044 ;
   voltage[correctChannel] = ina3221.getVoltage((ina3221_ch_t) correctChannel) * 0.9593;
   power[correctChannel] = current_compensated[correctChannel] * voltage[correctChannel];
}
