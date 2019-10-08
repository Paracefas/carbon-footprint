#include <LiquidCrystal.h>
#define RAW_MQ135 45

LiquidCrystal lcd(12, 11, 5,  4, 3, 2);

class MQSensor 
{
public:
  MQSensor(unsigned port, double(*getPPM)(double), double RL, double R0) 
    : m_port{ port }, m_getPPM{ getPPM }, m_RL{ RL*1000 }, m_R0{ R0 } {}
  double read()
  {
    int adc_MQ = analogRead(m_port); 
    float voltaje = adc_MQ * (5.0 / 1023.0);
    float Rs = m_RL*((5-voltaje)/voltaje);  
    return m_getPPM(Rs/m_R0);
  }
  ~MQSensor() {}
private:
  unsigned m_port;
  double m_RL, m_R0;
  double (*m_getPPM)(double);  
};

double CH4(double x)
{
  return 4133 * pow(x, -2.56); 
}

double O3(double x)
{
  return 36.4 * pow(x, -1.09);
}

double CO(double x)
{
  return 0.371 * pow(x, -1.98);
}

MQSensor mq7{ A1, CO, 4.7/*Kohm*/, 32150.0 };
MQSensor mq9{ A2, CH4, 10/*Kohm*/, 1228.5 };
MQSensor mq131{ A3, O3, 20/*Kohm*/, 2083.33 };

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
}
 
void loop()
{
  float raw = 0.f;
  for(int i = 0; i < 10; ++i) 
  {
    raw += analogRead(A0);
    delay(200);
  }
  float avg = raw/10;
  
  float co2comp = avg - RAW_MQ135;
  float CO2 = map(co2comp, 0, 1023, 400, 5000); 
  
  lcd.print("CO2 level: ");
  lcd.print(CO2);
  Serial.print("CO2 level: ");
  Serial.println(CO2);
  delay(1000);
  lcd.clear();

  lcd.print("CO level: ");
  lcd.print(mq7.read());
  Serial.print("CO level: ");
  Serial.println(mq7.read());
  delay(1000);
  lcd.clear();
  
  lcd.print("CH4 level: ");
  lcd.print(mq9.read());
  Serial.print("CH4 level: ");
  Serial.println(mq9.read());
  delay(1000);
  lcd.clear();
  
  lcd.print("O3 level: ");
  lcd.print(mq131.read());
  Serial.print("O3 level: ");
  Serial.println(mq131.read());
  delay(1000);
  lcd.clear();
}
