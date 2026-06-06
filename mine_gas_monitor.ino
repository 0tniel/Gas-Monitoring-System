#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define MQ7 A0
#define MQ135 A1

#define BUZZER 8
#define RED_LED 9
#define GREEN_LED 10

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);

    pinMode(BUZZER, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    digitalWrite(GREEN_LED, HIGH);

    dht.begin();

    Serial.println("Mine Gas Monitoring System Started");
}

void loop()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int mq7Raw = analogRead(MQ7);
    int mq135Raw = analogRead(MQ135);

    // Approximate conversions
    float co_ppm = map(mq7Raw, 0, 1023, 0, 200);
    float tgi = map(mq135Raw, 0, 1023, 0, 100);

    bool danger = false;

    if (co_ppm > 100 || tgi > 80)
    {
        danger = true;
    }

    if (danger)
    {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);

        tone(BUZZER, 2000);
    }
    else
    {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);

        noTone(BUZZER);
    }

    // ===== SERIAL FORMAT FOR STREAMLIT =====

    Serial.print("CO ppm = ");
    Serial.println(co_ppm, 2);

    Serial.print("Temp (°C): ");
    Serial.println(temperature, 2);

    Serial.print("Humidity (%): ");
    Serial.println(humidity, 2);

    Serial.print("TGI: ");
    Serial.println(tgi, 2);

    Serial.println("------------------------");

    delay(2000);
}
