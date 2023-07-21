# 1 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino"
# 2 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 3 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 4 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 5 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 6 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 7 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 8 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2
# 9 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino" 2

String message = "";
bool messageReady = false;
# 28 "C:\\Users\\arge\\Documents\\dev\\leonardo-temperature-controller\\cpu\\cpu.ino"
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3, 2, 6);
DHT dht(14, DHT22);

int graphStartPixel = 25;
float temperatureStack[58];
float humidityStack[58];

void setup()
{
  Serial.begin(9600);
  display.setFont(&FreeSansBold9pt7b);
  display.begin();
  display.setContrast(30);
  display.clearDisplay();
  display.setCursor(1, 27);
  display.println("Starting...");
  display.display();
  dht.begin();
  for (int i = 0; i < 58; i++)
  {
    temperatureStack[i] = 0;
    humidityStack[i] = 0;
  }
}

void showValues(boolean isTemperature)
{
  display.clearDisplay();
  if (isTemperature)
  {
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(14, 13);
    display.print(temperatureStack[58 - 1]);
    display.print("C");
  }
  else
  {

    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(10, 13);
    display.print("%");
    display.println(humidityStack[58 - 1]);
  }
  display.display();
  for (int j = 0; j < 58; j++)
  {
    if (isTemperature)
    {
      display.writeLine(j + graphStartPixel, (58 - 10) - (temperatureStack[j]), j + graphStartPixel, 58 - 10, 1 /*|< Black pixel*/);
      findMaxMinValue(true);
    }
    else
    {
      display.writeLine(j + graphStartPixel, (58 - 10) - (humidityStack[j] * 0.3), j + graphStartPixel, 58 - 10, 1 /*|< Black pixel*/);
      findMaxMinValue(false);
    }
  }
  display.display();
}

/**
   Push data to stack
*/
void push(boolean isTemperature, float item)
{

  float tempStack[58 + 1];
  for (int i = 0; i < 58; i++)
  {
    if (isTemperature)
    {
      tempStack[i] = temperatureStack[i];
    }
    else
    {
      tempStack[i] = humidityStack[i];
    }
  }
  tempStack[58] = 0;
  tempStack[58] = item;
  for (int j = 0; j < 58; j++)
  {
    if (isTemperature)
    {
      temperatureStack[j] = tempStack[j + 1];
    }
    else
    {
      humidityStack[j] = tempStack[j + 1];
    }
  }
}

void findMaxMinValue(boolean isTemperature)
{
  float minimumValueTemperature = 0, maximumValueTemperature = 0, minimumValueHumidity = 0, maximumValueHumidity = 0;
  for (int k = 58 - 1; k > 0; k--)
  {
    if (k == 58 - 1)
    {
      minimumValueTemperature = temperatureStack[k];
      maximumValueTemperature = temperatureStack[k];
      minimumValueHumidity = humidityStack[k];
      maximumValueHumidity = humidityStack[k];
    }
    if (temperatureStack[k] < minimumValueTemperature && temperatureStack[k] != 0.00)
    {
      minimumValueTemperature = temperatureStack[k];
    }
    if (temperatureStack[k] > maximumValueTemperature)
    {
      maximumValueTemperature = temperatureStack[k];
    }
    if (humidityStack[k] < minimumValueHumidity && humidityStack[k] != 0.00)
    {
      minimumValueHumidity = humidityStack[k];
    }
    if (humidityStack[k] > maximumValueHumidity)
    {
      maximumValueHumidity = humidityStack[k];
    }
  }
  display.setFont();
  if (isTemperature)
  {
    display.setCursor(0, 15);
    display.println("MAX");
    int maximumValueTemperature2 = maximumValueTemperature;
    int minimumValueTemperature2 = minimumValueTemperature;
    float a = 0, b = 0;
    a = maximumValueTemperature - maximumValueTemperature2;
    b = minimumValueTemperature - minimumValueTemperature2;
    if (a >= 0.5)
    {
      maximumValueTemperature2 += 1;
    }
    if (b >= 0.5)
    {
      minimumValueTemperature2 += 1;
    }
    display.println(maximumValueTemperature2);
    display.setCursor(0, 33);
    display.println("MIN");
    display.println(minimumValueTemperature2);
  }
  else
  {
    display.setCursor(0, 15);
    display.println("MAX");
    int maximumValueHumidity2 = maximumValueHumidity;
    int minimumValueHumidity2 = minimumValueHumidity;
    float c = 0, d = 0;
    c = maximumValueHumidity - maximumValueHumidity2;
    d = minimumValueHumidity - minimumValueHumidity2;
    if (c >= 0.5)
    {
      maximumValueHumidity2 += 1;
    }
    if (d >= 0.5)
    {
      minimumValueHumidity2 += 1;
    }
    display.println(maximumValueHumidity2);
    display.setCursor(0, 33);
    display.println("MIN");
    display.println(minimumValueHumidity2);
    display.display();
  }
}

/**
   Reads Temprerature and Humidity from Sensor at dht

   return void
*/
void readTemperatureHumidity()
{
  float temperature, humidity;
  temperature = dht.readTemperature();
  delay(5000);
  humidity = dht.readHumidity();
  delay(5000);
  if (isnan(temperature) || isnan(humidity))
  {
    push(true, 0);
    push(false, 0);
    return;
  }
  push(true, temperature);
  push(false, humidity);
}

void loop()
{
  readTemperatureHumidity();
  showValues(true);
  delay(5000);
  showValues(false);
  delay(5000);
}
