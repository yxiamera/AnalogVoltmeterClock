#include <iarduino_RTC.h>

#define PIN_METER_HOUR 3
#define PIN_METER_MIN 5
#define PIN_METER_SEC 6
#define HOUR_BUTTON 4
#define MIN_BUTTON 7

iarduino_RTC time(RTC_DS1302, 10, 8, 9);

int addMin = 0;
int addHour = 0;
int hourButtonState;
int minButtonState;
int hourFlag = 0;
int minFlag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(HOUR_BUTTON, INPUT_PULLUP); 
  pinMode(MIN_BUTTON, INPUT_PULLUP);
  time.begin();
}

void loop() {
  //смотрим состояние кнопок, если необходимо - регулируем время
  setTimeWithButtons();

  if (millis()%1000==0)
  { // если прошла 1 секунда
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      analogWrite(PIN_METER_HOUR, time.hours*(12.75));
      analogWrite(PIN_METER_MIN, time.minutes*(2.55));
      analogWrite(PIN_METER_SEC, time.seconds*(2.55));
  }
}

void setTimeWithButtons() {
  //считываем показания кнопок
  if ((digitalRead(HOUR_BUTTON) == LOW)&&(hourFlag == 0))
  {
    addHour++;
    Serial.println("Hour");
    
    hourFlag = 1;
  }

  if ((digitalRead(HOUR_BUTTON) == HIGH)&&(hourFlag == 1))
  {
    hourFlag = 0;
    Serial.println("Hour HIGH");
  }
  
  if ((digitalRead(MIN_BUTTON) == LOW)&&(minFlag == 0))
  {
    addMin++;
    Serial.println("Min");
    
    minFlag = 1;
    minButtonState = digitalRead(MIN_BUTTON);
  }

  if ((digitalRead(MIN_BUTTON) == HIGH)&&(minFlag == 1))
  {
    minFlag = 0;
    Serial.println("Min HIGH");
  }

  if ((addHour > 0) || (addMin > 0))
  {
    //проверяем на превышение значений
    if ((time.minutes + addMin) == 60)
    {
      time.settime(time.seconds, 0, time.Hours, time.day, time.month, time.year, time.weekday);
      addMin = 0;
    }
    if ((time.Hours + addHour) == 24)
    {
      time.settime(time.seconds, time.minutes, 0, time.day, time.month, time.year, time.weekday);
      addHour = 0;
    }

    //устанавливаем время
    time.settime(time.seconds, (time.minutes + addMin), (time.Hours + addHour), time.day, time.month, time.year, time.weekday);
    Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
    analogWrite(PIN_METER_HOUR, time.hours*(12.75));
    analogWrite(PIN_METER_MIN, time.minutes*(2.55));
    analogWrite(PIN_METER_SEC, time.seconds*(2.55));
    addHour = 0;
    addMin = 0;
  }
}
