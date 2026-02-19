#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// ========== WiFi 配置 ==========
const char* ssid = "TP-LINK_C0AE";
const char* password = "12345678";

// ========== MQTT 配置 ==========
const char* mqtt_server = "broker-cn.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic_control = "esp32/led/control";
const char* mqtt_topic_status = "esp32/led/status";

// ========== LED 引脚（PWM）==========
const int ledPin = 2;
bool ledStatus = false;               // 外部期望的开关状态（true=开，false=关）
#define LEDC_BASE_FREQ  5000           // PWM频率
#define LEDC_TIMER_BIT  8              // 8位分辨率

// ========== 蜂鸣器引脚 ==========
#define BUZZER_PIN 4

// ========== BLE 相关 ==========
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// ========== WiFi 客户端与 MQTT 客户端 ==========
WiFiClient espClient;
PubSubClient client(espClient);

// ========== 音乐播放相关 ==========
const int toneFreq[] = {
  262, 294, 330, 349, 392, 440, 494,  // 中音 1-7
  523, 587, 659, 698, 784, 880, 988   // 高音 1-7
};
const int musicNotes[] = {
  1, 2, 3, 1,    1, 2, 3, 1,    3, 4, 5,       3, 4, 5,
  5, 6, 5, 4, 3, 1,    5, 6, 5, 4, 3, 1,
  1, 5, 1,       1, 5, 1
};
const int musicDurations[] = {
  400, 400, 400, 400,    400, 400, 400, 400,    400, 400, 800,    400, 400, 800,
  300, 100, 300, 100, 400, 400,    300, 100, 300, 100, 400, 400,
  400, 400, 800,    400, 400, 800
};
#define NOTE_COUNT (sizeof(musicNotes)/sizeof(musicNotes[0]))

bool playingMusic = false;
int currentNoteIndex = 0;
unsigned long noteStartTime = 0;

// ========== LED PWM 控制函数 ==========
void initLedPWM() {
  // 新版 API：直接指定引脚、频率、分辨率
  ledcAttach(ledPin, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
}

// 设置 LED 亮度 (0-100)
void setLedBrightness(int percent) {
  int duty = map(percent, 0, 100, 0, 255);
  ledcWrite(ledPin, duty);
}

// 设置 LED 开关（全亮或全灭）
void setLed(bool on) {
  if (on) {
    ledcWrite(ledPin, 255); // 全亮
  } else {
    ledcWrite(ledPin, 0);   // 全灭
  }
}

// ========== BLE 服务器回调类 ==========
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("BLE 已连接");
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("BLE 断开连接");
    pServer->getAdvertising()->start();
  }
};

// ========== BLE 特征值回调类（接收手机写入）==========
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
      Serial.print("BLE 收到原始字节: ");
      // 以十六进制打印每个字节，便于调试
      for (int i = 0; i < value.length(); i++) {
        Serial.print("0x");
        Serial.print((uint8_t)value[i], HEX);
        Serial.print(" ");
      }
      Serial.print(" 字符串形式: ");
      Serial.println(value);

      if (value == "1") {
        ledStatus = true;
        if (!playingMusic) {
          setLed(true);
        }
        client.publish(mqtt_topic_status, "1");
      } else if (value == "2") {
        ledStatus = false;
        if (!playingMusic) {
          setLed(false);
        }
        client.publish(mqtt_topic_status, "2");
      } else if (value == "3") {
        startMusic();
      }
    }
  }
};

// ========== MQTT 回调 ==========
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT 收到 [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (length == 1) {
    char cmd = (char)payload[0];
    if (cmd == '1') {
      ledStatus = true;
      if (!playingMusic) {
        setLed(true);
      }
    } else if (cmd == '2') {
      ledStatus = false;
      if (!playingMusic) {
        setLed(false);
      }
    } else if (cmd == '3') {
      startMusic();
    }
  }
}

// ========== 开始播放音乐 ==========
void startMusic() {
  if (!playingMusic) {
    playingMusic = true;
    currentNoteIndex = 0;
    noteStartTime = millis();
    Serial.println("开始播放《两只老虎》");
  }
}

// ========== 停止播放音乐（恢复 LED 状态）==========
void stopMusic() {
  playingMusic = false;
  noTone(BUZZER_PIN);
  // 根据 ledStatus 恢复 LED
  setLed(ledStatus);
  Serial.println("音乐播放结束");
}

// ========== 播放一个音符（非阻塞） ==========
void playNote(int noteIdx) {
  if (noteIdx < 0 || noteIdx >= NOTE_COUNT) return;
  int note = musicNotes[noteIdx] - 1;
  int freq = toneFreq[note];
  tone(BUZZER_PIN, freq);

  // --- 夸张的亮度映射（指数曲线） ---
  const int minFreq = 130;   // 最低频率约130Hz
  const int maxFreq = 1000;  // 最高频率约1000Hz
  int limitedFreq = constrain(freq, minFreq, maxFreq);
  
  float ratio = (float)(limitedFreq - minFreq) / (maxFreq - minFreq);
  ratio = ratio * ratio * ratio;   // 立方
  
  int minBrightness = 5;     // 最低亮度5%
  int maxBrightness = 100;    // 最高亮度100%
  int brightness = minBrightness + (int)(ratio * (maxBrightness - minBrightness));
  brightness = constrain(brightness, 0, 100);
  
  setLedBrightness(brightness);
}

// ========== MQTT 重连 ==========
void reconnect() {
  while (!client.connected()) {
    Serial.print("连接 MQTT...");
    String clientId = "ESP32_" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("已连接");
      client.subscribe(mqtt_topic_control);
      client.publish(mqtt_topic_status, ledStatus ? "1" : "2");
    } else {
      Serial.print("失败，错误码=");
      Serial.print(client.state());
      Serial.println(" 5秒后重试");
      delay(5000);
    }
  }
}

// ========== 初始化 BLE ==========
void initBLE() {
  BLEDevice::init("ESP32-C3_LED");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMaxPreferred(0x0C);
  pAdvertising->setScanResponse(false);
  pAdvertising->start();
  Serial.println("BLE 服务已启动，设备名: ESP32-C3_LED");
}

// ========== 初始化 ==========
void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口稳定

  initLedPWM();                // 初始化 LED PWM
  setLed(ledStatus);           // 根据初始状态设置 LED

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("连接 WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 连接成功！IP: " + WiFi.localIP().toString());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  initBLE();
}

// ========== 主循环 ==========
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (playingMusic) {
    unsigned long now = millis();
    int duration = musicDurations[currentNoteIndex];

    if (currentNoteIndex == 0 || now - noteStartTime >= duration) {
      currentNoteIndex++;
      if (currentNoteIndex >= NOTE_COUNT) {
        stopMusic();
      } else {
        noteStartTime = now;
        playNote(currentNoteIndex);
      }
    }
  }

  delay(10);
}
