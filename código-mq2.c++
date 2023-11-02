# include <LiquidCrystal.h>

LiquidCrystal lcd(11,10,9,8,7,6,5);

byte led = 12;
byte som = 13;
int sensorGas = A5; 		 
int valorSensorGas = 0;

void setup() {
  Serial.begin(9600);	
  pinMode (sensorGas, INPUT);
  pinMode (led, OUTPUT);  
  lcd.begin(16,2);
  lcd.clear();
}
  void loop() 
{	
  valorSensorGas = analogRead(sensorGas);
    
  Serial.print("Valor medido: ");
  Serial.println(valorSensorGas);
               

  if (valorSensorGas >196) 					
  {				
    lcd.setCursor(0,0);
    lcd.print("Nivel Critico!");
    lcd.setCursor(0,1);
    lcd.print(valorSensorGas);
    lcd.print("g/km de co2");
    digitalWrite(led, HIGH);							
    tone(som, 1500);
    delay(500);
    noTone (som);
    delay(500);
    lcd.clear();
  }
  else 	
  {
    lcd.setCursor(0,0);
    lcd.print("Nivel ok");
    lcd.setCursor(0,1);
    lcd.print(valorSensorGas);
    lcd.print("g/km de co2");
    digitalWrite(led, LOW);
    delay(500);
    lcd.clear();
  }
}
