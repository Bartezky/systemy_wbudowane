#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include "EspMQTTClient.h"
#include <esp_task_wdt.h>

#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "Font4x7Fixed.h"
#include "Font4x5Fixed.h"
#include "Font5x5Fixed.h"
#include "Font5x7FixedMono.h"
#include "Font5x7Fixed.h"
#include "FreeSans6pt7b.h"
#include "FreeSansBold6pt7b.h"
#include "FreeSansBold8pt7b.h"
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

#define COLS PANEL_RES_X
#define ROWS PANEL_RES_Y


//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12 //18   //was 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 17
#define E_PIN -1   //21   // was -1 // required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 16


//3 seconds WDT
#define WDT_TIMEOUT 10

//#ifndef LED_BUILTIN
#define LED_BUILTIN   12
//#endif


extern EspMQTTClient client;


#define TOPIC_RX "disp/text"

#define MQTT_KEEP_CONN  0


EspMQTTClient client(
        "esp",
        "haslo8266",
        "192.168.0.66", /* // MQTT Broker server ip
  "mqttuser",   // Can be omitted if not needed
  "mqttpass",   // Can be omitted if not needed*/
        "LEDMTXCLK"      // Client name that uniquely identify your device

);


// const char *WIFI_ssid = "Urwany toster";
// const char *WIFI_password = "5q369G0?";


//todo: https://www.geeksforgeeks.org/convert-string-char-array-cpp/

void onMessageReceived(const String &topic, const String &message) {
    Serial.println("onMessageReceived / client.sub func: " + message);
}

static volatile bool sppStateUpd = false;
static String mqttRx = "Nasz projekt";

void onConnectionEstablished() {
    // Subscribe to "mytopic/test" and display received message to Serial
#if MQTT_KEEP_CONN == 0
    client.subscribe(TOPIC_RX, [](const String &payload) {
        //Serial.println("\nonConnectionEstablished / client.sub func: " + payload);
        mqttRx = payload;
        sppStateUpd = true;
    });
#endif
}


// const uint16_t color1 = dma_display->color565(64, 0, 64);
// const uint16_t color2 = dma_display->color565(128, 0, 0);
// const uint16_t color3 = dma_display->color565(0, 64, 64);
// const uint16_t color4 = dma_display->color565(0, 128, 0);
// const uint16_t color5 = dma_display->color565(64, 64, 0);
// const uint16_t color6 = dma_display->color565(128, 128, 128);
// const uint16_t color7 = dma_display->color565(255, 0, 0);
// const uint16_t color8 = dma_display->color565(0, 0, 255);

// const uint16_t colors[] = {color1, color2, color3, color4, color5, color6, color7, color8};

int sort_array[COLS];

void sort_initialize() {
    for(int i=0; i<COLS; ++i) {
        sort_array[i] = random(1, ROWS);
    }
}

void sort_display() {
    dma_display->clearScreen();
    for(int i=0; i<COLS; ++i) {
        dma_display->drawFastVLine(i, ROWS-sort_array[i], sort_array[i], dma_display->color565(0, 64, 64));
    }
}

void sort_display(int x1, int x2) {
    sort_display();
    dma_display->drawFastVLine(x1, ROWS-sort_array[x1], sort_array[x1], dma_display->color565(128, 0, 0));
    dma_display->drawFastVLine(x2, ROWS-sort_array[x2], sort_array[x2], dma_display->color565(128, 0, 0));
}

void sort_display(String name) {
    dma_display->clearScreen();
    dma_display->setCursor(3, 3);
    dma_display->print(name);
    dma_display->setCursor(3, 15);
    dma_display->print("sort");
    vTaskDelay(1000);
}

void bubble_sort() {
    sort_display("Bubble");
    sort_display();
    for(int i=0; i<COLS-1; ++i) {
        for(int j=0; j<COLS-i-1; ++j){
            sort_display(j, j+1);
            vTaskDelay(10);
            if (sort_array[j] > sort_array[j+1]) {
                int tmp = sort_array[j];
                sort_array[j] = sort_array[j+1];
                sort_array[j+1] = tmp;
                sort_display(j, j+1);
            }
            vTaskDelay(10);
        }
    }
    sort_display();
    vTaskDelay(1000);
}

void merge(int a, int m, int b) {
    int n1 = m - a + 1;
    int n2 = b - a;
    std::vector<int> L(n1);
    for (int i=0; i<n1; ++i) {
        L[i] = sort_array[a + i];
    }

    int i = 0, j = 0, k = a;
    while (i < n1 && j < n2) {
        // sort_display(k, m + j + 1);
        // vTaskDelay(100);
        if (L[i] <= sort_array[m + j + 1]) {
            sort_array[k] = L[i];
            ++i;
        } else {
            sort_array[k] = sort_array[m + j + 1];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        sort_array[k] = L[i];
        ++i;
        ++k;
    }
}


void merge_sort(int a, int b) {
    if (a == b) {
        return;
    }
    else {
        int m = a + (b - a) / 2;
        merge_sort(a, m);
        merge_sort(m+1, b);
        merge(a, m, b);
        sort_display();
        vTaskDelay(300);
    }
}

void m_sort() {
    sort_display("Merge");
    sort_display();
    merge_sort(0, COLS-1);
    sort_display();
    vTaskDelay(1000);
}

//33 32 35 34
const uint8_t button[4] = {34, 35, 32, 33};
#define BUTTON0   button[0]
#define BUTTON1   button[1]
#define BUTTON2   button[2]
#define BUTTON3   button[3]
#define BUTTONS_NB 4

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.print("Hello!\n");
    HUB75_I2S_CFG::i2s_pins _pins = {R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN,
                                     LAT_PIN, OE_PIN, CLK_PIN};

    HUB75_I2S_CFG mxconfig(
            PANEL_RES_X,   // module width
            PANEL_RES_Y,   // module height
            PANEL_CHAIN,    // Chain length
            _pins // pin mapping
    );
    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(90); //0-255
    dma_display->clearScreen();
    dma_display->fillScreen(myBLACK);


    //push buttons
    for (int i = 0; i < BUTTONS_NB; i++) {
        pinMode(button[i], INPUT_PULLUP);
    }

    Serial.printf("MQTT Client init...");
    client.enableLastWillMessage("spp/lastwill",
                                 "I am going offline");  // You can activate the retain flag by setting the third parameter to true
    client.subscribe(TOPIC_RX, onMessageReceived);
    client.enableDebuggingMessages(false);
    Serial.printf("MQTT Client done\n");


}


void loop() {
    // client.loop();
    sort_initialize();
    m_sort();
    // bubble_sort();
    vTaskDelay(1000);


    // if(sppStateUpd==true){
    //   sppStateUpd=false;
    //   Serial.printf("MQTT upd\n");
    //   dma_display->clearScreen();
    //   dma_display->setCursor(4,10);
    //   dma_display->setTextColor(dma_display->color444(0, 255, 255));
    //   dma_display->print(mqttRx);
    // }

}

