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

#include <unordered_map>

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

bool button_clicked[] = {false, false, false, false};
int menu_cursor = 0;
bool in_menu = false;
static unsigned long last_click_time = 0;
#define CLICK_DELAY 250
int speed = 2;

int delays[] = {500, 200, 100, 50, 20, 10, 5, 2, 1, 0};

void unclick_buttons() {
    for (int i = 0; i < 4; ++i) button_clicked[i] = false;
}

void IRAM_ATTR button0_clicked(void) {
    if (millis() - last_click_time > CLICK_DELAY) {
        last_click_time = millis();
        Serial.println("Button 0 clicked");
        if (!in_menu) {
            in_menu = true;
            menu_cursor = 0;
            speed = 9;
            unclick_buttons();
        } else {
            button_clicked[0] = true;
        }
    }
}

void IRAM_ATTR button1_clicked(void) {
    if (millis() - last_click_time > CLICK_DELAY) {
        last_click_time = millis();
        Serial.println("Button 1 clicked");
        if (!in_menu) {
            speed++;
            if (speed > 8) speed = 8;
        } else {
            button_clicked[1] = true;
        }
    }
}

void IRAM_ATTR button2_clicked(void) {
    if (millis() - last_click_time > CLICK_DELAY) {
        last_click_time = millis();
        Serial.println("Button 2 clicked");
        if (!in_menu) {
            speed--;
            if (speed < 0) speed = 0;
        } else {
            button_clicked[2] = true;
        }
    }
}

void IRAM_ATTR button3_clicked(void) {
    if (millis() - last_click_time > CLICK_DELAY) {
        last_click_time = millis();
        Serial.println("Button 3 clicked");
        button_clicked[3] = true;
    }
}


int sort_array[COLS];

void sort_initialize_random() {
    for (int i = 0; i < COLS; ++i) {
        sort_array[i] = random(1, ROWS);
    }
}

void sort_initialize_sorted() {
    for (int i = 0; i < COLS; ++i) {
        sort_array[i] = i / 2;
    }
}

void sort_initialize_asorted() {
    for (int i = 0; i < COLS; ++i) {
        sort_array[COLS - i] = i / 2;
    }
}

void sort_display() {
    if (in_menu) return;
    dma_display->clearScreen();
    for (int i = 0; i < COLS; ++i) {
        dma_display->drawFastVLine(i, ROWS - sort_array[i], sort_array[i], dma_display->color565(0, 64, 64));
    }
}

void sort_display(int x1, int x2) {
    if (in_menu) return;
    sort_display();
    dma_display->drawFastVLine(x1, ROWS - sort_array[x1], sort_array[x1], dma_display->color565(128, 0, 0));
    dma_display->drawFastVLine(x2, ROWS - sort_array[x2], sort_array[x2], dma_display->color565(128, 0, 0));
}

void sort_display(String name) {
    dma_display->clearScreen();
    dma_display->setCursor(2, 10);
    dma_display->print(name);
    dma_display->setCursor(2, 20);
    dma_display->print("sort");
    vTaskDelay(1000);
}

void bubble_sort() {
    sort_display("Bubble");
    sort_display();
    for (int i = 0; i < COLS - 1; ++i) {
        for (int j = 0; j < COLS - i - 1; ++j) {
            sort_display(j, j + 1);
            vTaskDelay(delays[speed]);
            if (sort_array[j] > sort_array[j + 1]) {
                int tmp = sort_array[j];
                sort_array[j] = sort_array[j + 1];
                sort_array[j + 1] = tmp;
                sort_display(j, j + 1);
            }
            vTaskDelay(delays[speed]);
        }
    }
    sort_display();
    vTaskDelay(1000);
}

void merge(int a, int m, int b) {
    int n1 = m - a + 1;
    int n2 = b - a;
    std::vector<int> L(n1);
    for (int i = 0; i < n1; ++i) {
        L[i] = sort_array[a + i];
    }

    int i = 0, j = 0, k = a;
    while (i < n1 && j < n2) {
        sort_display(k, m + j + 1);
        vTaskDelay(delays[speed]);
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
    } else {
        int m = a + (b - a) / 2;
        merge_sort(a, m);
        merge_sort(m + 1, b);
        merge(a, m, b);
        sort_display();
        vTaskDelay(delays[speed]);
    }
}

void m_sort() {
    sort_display("Merge");
    sort_display();
    merge_sort(0, COLS - 1);
    sort_display();
    vTaskDelay(1000);
}


int partition(int start, int end) {
    int id = random(start, end + 1);
    int x = sort_array[id];
    sort_array[id] = sort_array[end];
    sort_array[end] = x;

    int i = start - 1;
    for (int j = start; j < end; j++) {
        if (sort_array[j] <= x) {
            i++;
            sort_display(j, end);
            vTaskDelay(delays[speed]);
            int tmp = sort_array[i];
            sort_array[i] = sort_array[j];
            sort_array[j] = tmp;
        }

    }
    i++;
    int tmp = sort_array[i];
    sort_array[i] = sort_array[end];
    sort_array[end] = tmp;
    sort_display(i, end);
    vTaskDelay(delays[speed]);
    return i;
}

void quick_sort(int start, int end) {
    if (start < end) {
        int pivot = partition(start, end);
        quick_sort(start, pivot - 1);
        quick_sort(pivot + 1, end);
    }
}

void q_sort() {
    sort_display("Quick");
    sort_display();
    quick_sort(0, COLS - 1);
    sort_display();
    vTaskDelay(1000);
}


void selection_sort() {
    for (int j = 0; j < COLS; j++) {
        int min = sort_array[j];
        int min_idx = j;
        for (int i = j; i < COLS; i++) {
            if (sort_array[i] < min) {
                min = sort_array[i];
                min_idx = i;
            }
            sort_display(i, min_idx);
            vTaskDelay(delays[speed]);
        }
        int tmp = sort_array[min_idx];
        sort_array[min_idx] = sort_array[j];
        sort_array[j] = tmp;
        sort_display(j, min_idx);
        vTaskDelay(delays[speed]);
    }
}

void s_sort() {
    sort_display("Selection");
    sort_display();
    selection_sort();
    sort_display();
    vTaskDelay(1000);
}


void insertion_sort() {
    for (int i = 1; i < COLS; i++) {
        int key = sort_array[i];
        int j = i - 1;

        while (j >= 0 && sort_array[j] > key) {
            sort_array[j + 1] = sort_array[j];
            sort_display(j + 1, j);
            j--;
            vTaskDelay(delays[speed]);
        }
        sort_array[j + 1] = key;
        sort_display(j + 1, i);
        vTaskDelay(delays[speed]);
    }
}


void i_sort() {
    sort_display("Insertion");
    sort_display();
    insertion_sort();
    sort_display();
    vTaskDelay(1000);
}

void shells() {
    for (int gap = COLS / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < COLS; i++) {
            int temp = sort_array[i];
            int j;

            for (j = i; j >= gap && sort_array[j - gap] > temp; j -= gap) {
                sort_array[j] = sort_array[j - gap];
                sort_display(j, j - gap);
                vTaskDelay(delays[speed]);
            }

            sort_array[j] = temp;
            sort_display(j, i);
            vTaskDelay(delays[speed]);
        }
    }
}


void shell_sort() {
    sort_display("Shell");
    sort_display();
    shells();
    sort_display();
    vTaskDelay(1000);
}

struct Menu_element {
    String name;
    String display;
    int parent_id;
    int prev;
    int next;
    int child;
    std::vector<String> options;
    int value;
};

Menu_element menu[] = {
        {"Menu",           "Menu",           0, 0,  0,  1},
        {"Algorithms",     "Algorithms",     0, 8,  8,  2},
        {"Bubble sort",    "Bubble",         1, 7,  3,  2,  {"On", "Off"}, 0},
        {"Insertion sort", "Insertion",      1, 2,  4,  3,  {"On", "Off"}, 0},
        {"Selection sort", "Selection",      1, 3,  5,  4,  {"On", "Off"}, 0},
        {"Shell sort",     "Shell",          1, 4,  6,  5,  {"On", "Off"}, 0},
        {"Merge sort",     "Merge",          1, 5,  7,  6,  {"On", "Off"}, 0},
        {"Quick sort",     "Quick",          1, 6,  2,  7,  {"On", "Off"}, 0},
        {"Initialization", "Initialization", 0, 1,  1,  9},
        {"Random",         "Random",         8, 11, 10, 9,  {"On", "Off"}, 0},
        {"Sorted",         "Sorted",         8, 9,  11, 10, {"On", "Off"}, 0},
        {"Asorted",        "Asorted",        8, 10, 9,  11, {"On", "Off"}, 0}, // 11
};

bool algorithms[] = {true, true, true, true, true, true};
bool initializations[] = {true, true, true};

typedef void (*FunctionPointer)();

FunctionPointer algorithm_functions[] = {&bubble_sort, &i_sort, &s_sort, &shell_sort, &m_sort, &q_sort};
FunctionPointer initialization_functions[] = {&sort_initialize_random, &sort_initialize_sorted,
                                              &sort_initialize_asorted};

void display_menu() {
    dma_display->clearScreen();
    dma_display->setTextColor(dma_display->color565(255, 255, 255));
    dma_display->setCursor(2, 10);
    dma_display->print(menu[menu_cursor].display);
    if (!menu[menu_cursor].options.empty()) {
        if (menu[menu_cursor].value == 0) {
            dma_display->setTextColor(dma_display->color565(0, 128, 0));
        } else {
            dma_display->setTextColor(dma_display->color565(255, 255, 255));
        }
        dma_display->setCursor(2, 20);
        dma_display->print(menu[menu_cursor].options[0]);

        if (menu[menu_cursor].value == 1) {
            dma_display->setTextColor(dma_display->color565(0, 128, 0));
        } else {
            dma_display->setTextColor(dma_display->color565(255, 255, 255));
        }
        dma_display->setCursor(15, 20);
        dma_display->print(menu[menu_cursor].options[1]);
    }
}

void menu_loop() {
    while (true) {
        display_menu();
        vTaskDelay(250);
        if (button_clicked[1]) {
            menu_cursor = menu[menu_cursor].next;
            button_clicked[1] = false;
        } else if (button_clicked[2]) {
            menu_cursor = menu[menu_cursor].prev;
            button_clicked[2] = false;
        } else if (button_clicked[3]) {
            if (menu_cursor == 0) {
                in_menu = false;
                speed = 2;
                button_clicked[3] = false;
                return;
            } else {
                menu_cursor = menu[menu_cursor].parent_id;
                button_clicked[3] = false;
            }
        } else if (button_clicked[0]) {
            if (menu_cursor == 0) {
                menu_cursor = 1;
                button_clicked[0] = false;
            } else if (menu_cursor >= 2 && menu_cursor <= 7) {
                algorithms[menu_cursor - 2] = !algorithms[menu_cursor - 2];
                menu[menu_cursor].value++;
                menu[menu_cursor].value %= 2;
                button_clicked[0] = false;
            } else if (menu_cursor >= 9 && menu_cursor <= 11) {
                initializations[menu_cursor - 9] = !initializations[menu_cursor - 9];
                menu[menu_cursor].value++;
                menu[menu_cursor].value %= 2;
                button_clicked[0] = false;
            } else {
                menu_cursor = menu[menu_cursor].child;
                button_clicked[0] = false;
            }
        }

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
    dma_display->setFont(&Font5x7Fixed);


    //push buttons
    for (int i = 0; i < BUTTONS_NB; i++) {
        pinMode(button[i], INPUT_PULLUP);
    }

    attachInterrupt(button[0], button0_clicked, FALLING);
    attachInterrupt(button[1], button1_clicked, FALLING);
    attachInterrupt(button[2], button2_clicked, FALLING);
    attachInterrupt(button[3], button3_clicked, FALLING);

    Serial.printf("MQTT Client init...");
    client.enableLastWillMessage("spp/lastwill",
                                 "I am going offline");  // You can activate the retain flag by setting the third parameter to true
    client.subscribe(TOPIC_RX, onMessageReceived);
    client.enableDebuggingMessages(false);
    Serial.printf("MQTT Client done\n");


}


void loop() {
    if (in_menu) {
        menu_loop();
    } else {
        for (int i = 0; i < 6; i++) {
            if (!algorithms[i]) {
                continue;
            }
            for (int j = 0; j < 3; j++) {
                if (!initializations[j]) {
                    continue;
                }
                initialization_functions[j]();
                algorithm_functions[i]();
                if (in_menu) return;
            }
        }
    }

}

