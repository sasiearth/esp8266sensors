#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define SERVER_IP "192.168.10.232"
#define SERVER_PORT "1880"
#ifndef STASSID
#define STASSID "KKU-EE-IoT"
#define STAPSK "ee@kku1234"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WiFiMulti WiFiMulti;

void setup()
{
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Wait for Connect to WiFi... ");

    while (WiFiMulti.run() != WL_CONNECTED)
    {
        Serial.print("#.");
        delay(1000);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.println("Channel: ");
    Serial.println(WiFi.channel());
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Subnet Masks: ");
    Serial.println(WiFi.subnetMask());
    Serial.println("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.println("DNS address: ");
    Serial.println(WiFi.dnsIP());

    delay(1000);
}

void loop()
{
    // wait for WiFi connection
    if ((WiFi.status() == WL_CONNECTED))
    {

        WiFiClient client;
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        http.addHeader("Content-Type", "application/json");
        http.begin(client, "http://" SERVER_IP ":" SERVER_PORT "/http/"); // HTTP
        Serial.print("[HTTP] POST...\n");
        // start connection and send HTTP header and body
        int httpCode = http.POST("{\"hello\":\"world\"}");

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {
                const String &payload = http.getString();
                Serial.println("received payload:\n<<");
                Serial.println(payload);
                Serial.println(">>");
            }
        }
        else
        {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);
}
