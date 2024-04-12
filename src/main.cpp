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

#include <vector>
#include <unordered_map>


#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

#define COLS PANEL_RES_X
#define ROWS PANEL_RES_Y
std::vector<std::vector<bool>> life_foreground;
std::vector<std::vector<bool>> life_background;
std::vector<std::vector<bool>> life_tmp;


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


int delay_time = 250;


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

void life_initialize() {
    life_foreground.resize(ROWS, std::vector<bool>(COLS, false));
    life_background.resize(ROWS, std::vector<bool>(COLS, false));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            life_foreground[i][j] = random() % 2 == 0;
        }
    }
}

void life_initialize_gun() {
    life_foreground.resize(ROWS, std::vector<bool>(COLS, false));
    life_background.resize(ROWS, std::vector<bool>(COLS, false));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            life_foreground[i][j] = false;
        }
    }

    life_foreground[0][25] = 1;
    life_foreground[1][23] = 1;
    life_foreground[1][25] = 1;
    life_foreground[2][13] = 1;
    life_foreground[2][14] = 1;
    life_foreground[2][21] = 1;
    life_foreground[2][22] = 1;
    life_foreground[2][35] = 1;
    life_foreground[2][36] = 1;

    life_foreground[3][12] = 1;
    life_foreground[3][16] = 1;
    life_foreground[3][21] = 1;
    life_foreground[3][22] = 1;
    life_foreground[3][35] = 1;
    life_foreground[3][36] = 1;

    life_foreground[4][1] = 1;
    life_foreground[4][2] = 1;
    life_foreground[4][11] = 1;
    life_foreground[4][17] = 1;
    life_foreground[4][21] = 1;
    life_foreground[4][22] = 1;

    life_foreground[5][1] = 1;
    life_foreground[5][2] = 1;
    life_foreground[5][11] = 1;
    life_foreground[5][15] = 1;
    life_foreground[5][17] = 1;
    life_foreground[5][18] = 1;
    life_foreground[5][23] = 1;
    life_foreground[5][25] = 1;

    life_foreground[6][11] = 1;
    life_foreground[6][17] = 1;
    life_foreground[6][25] = 1;

    life_foreground[7][12] = 1;
    life_foreground[7][16] = 1;

    life_foreground[8][13] = 1;
    life_foreground[8][14] = 1;

}

void life_step() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int aliveNeighbors = 0;

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;

                    int nx = i + dx;
                    int ny = j + dy;

                    // wersja ze zwykłymi granicami
                    // if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS && life_foreground[nx][ny]) {
                    //     ++aliveNeighbors;
                    // }
                    // wersja z "zawiniętymi krawędziami" - lewa kolumna graniczy z prawą, dolny wierszy graniczy  z górnym
                    if (life_foreground[(nx + ROWS) % ROWS][(ny + COLS) % COLS]) {
                        ++aliveNeighbors;
                    }
                }
            }

            if (life_foreground[i][j]) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    life_background[i][j] = false;
                } else {
                    life_background[i][j] = true;
                }
            } else {
                if (aliveNeighbors == 3) {
                    life_background[i][j] = true;
                } else {
                    life_background[i][j] = false;
                }
            }
        }
    }
    life_tmp = life_background;
    life_background = life_foreground;
    life_foreground = life_tmp;
}

void life_display() {
    dma_display->clearScreen();

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            if (life_foreground[y][x]) {
                dma_display->drawPixel(x, y, dma_display->color565(64, 0, 64));
            }
        }
    }
}

struct drop {
    int x;
    int y;
    int vx;
    int vy;
    int color;
};

std::vector<drop> drops;

const uint16_t color1 = dma_display->color565(64, 0, 64);
const uint16_t color2 = dma_display->color565(128, 0, 0);
const uint16_t color3 = dma_display->color565(0, 64, 64);
const uint16_t color4 = dma_display->color565(0, 128, 0);
const uint16_t color5 = dma_display->color565(64, 64, 0);
const uint16_t color6 = dma_display->color565(128, 128, 128);
const uint16_t color7 = dma_display->color565(255, 0, 0);
const uint16_t color8 = dma_display->color565(0, 0, 255);

const uint16_t colors[] = {color1, color2, color3, color4, color5, color6, color7, color8};

void drop_initialize() {
    drops.clear();
    for (int j = 0; j < COLS; ++j) {
        drops.push_back({j * 10, 0, ((int) random() % 7) - 3, ((int) random() % 7) - 3, random() % 8});
    }
}

void drop_down_initialize() {
    drops.clear();
    for (int j = 0; j < COLS; ++j) {
        drops.push_back({j * 10, 0, ((int) random() % 5) - 2, ((int) random() % 10), random() % 8});
    }
}

void drop_step() {
    for (auto &drop: drops) {
        drop.vx += ((int) random() % 5) - 2;
        if (drop.vx < -5) {
            drop.vx = -5;
        } else if (drop.vx > 5) {
            drop.vx = 5;
        }
        drop.x += drop.vx;
        drop.x = (drop.x + COLS * 10) % (COLS * 10);

        drop.vy += ((int) random() % 5) - 2;
        if (drop.vy < -5) {
            drop.vy = -5;
        } else if (drop.vy > 5) {
            drop.vy = 5;
        }
        drop.y += drop.vy;
        drop.y = (drop.y + ROWS * 10) % (ROWS * 10);
    }
}

void drop_down_step() {
    for (auto &drop: drops) {
        drop.vx += ((int) random() % 3) - 1;
        if (drop.vx < -2) {
            drop.vx = -2;
        } else if (drop.vx > 2) {
            drop.vx = 2;
        }
        drop.x += drop.vx;
        drop.x = (drop.x + COLS * 10) % (COLS * 10);

        drop.vy += ((int) random() % 5) - 2;
        if (drop.vy < 0) {
            drop.vy = 0;
        } else if (drop.vy > 10) {
            drop.vy = 10;
        }
        drop.y += drop.vy;
        drop.y = (drop.y + ROWS * 10) % (ROWS * 10);
    }
}

void drop_display() {
    dma_display->clearScreen();
    for (auto &drop: drops) {
        dma_display->drawPixel(drop.x / 10, drop.y / 10, colors[drop.color]);
    }
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

    // // fix the screen with green
    // dma_display->fillRect(3, 4, 10, 15, dma_display->color444(0, 8, 0));
    // delay(1000);
    // dma_display->clearScreen();
    // dma_display->setFont(&Font5x7Fixed);
    // dma_display->setTextColor(dma_display->color444(0, 255, 255));
    // dma_display->setCursor(4,10);
    // dma_display->print(mqttRx);
    
    life_initialize();

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

typedef void (*FunctionPointer)();

FunctionPointer step = &life_step;
FunctionPointer display = &life_display;

void buttons(void) {
    if (!digitalRead(button[0])) {
        life_initialize();
        delay_time = 250;
        step = &life_step;
        display = &life_display;
    } else if (!digitalRead(button[1])) {
        life_initialize_gun();
        delay_time = 100;
        step = &life_step;
        display = &life_display;
    } else if (!digitalRead(button[2])) {
        drop_initialize();
        delay_time = 100;
        step = &drop_step;
        display = &drop_display;
    } else if (!digitalRead(button[3])) {
        drop_down_initialize();
        delay_time = 100;
        step = &drop_down_step;
        display = &drop_display;
    }
}


void loop() {
    // client.loop();
    buttons();
    step();
    display();
    vTaskDelay(delay_time);


    // if(sppStateUpd==true){
    //   sppStateUpd=false;
    //   Serial.printf("MQTT upd\n");
    //   dma_display->clearScreen();
    //   dma_display->setCursor(4,10);
    //   dma_display->setTextColor(dma_display->color444(0, 255, 255));
    //   dma_display->print(mqttRx);
    // }

}

