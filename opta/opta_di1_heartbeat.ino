#include <Ethernet.h>
#include <ArduinoMqttClient.h>

// ============================================================
// KONFIG - NETTVERK
// FYLL INN EGNE VERDIER LOKALT
// IKKE COMMIT EKTE CREDENTIALS TIL GITHUB
// ============================================================
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };

// DHCP først, fallback til statisk IP
IPAddress fallbackIp(192, 168, 1, 156);
IPAddress brokerIp(192, 168, 1, 100);

// ============================================================
// KONFIG - MQTT
// FYLL INN EGNE VERDIER LOKALT
// ============================================================
const char* MQTT_CLIENT_ID = "opta-1";
const char* MQTT_USERNAME  = "REPLACE_ME_USERNAME";
const char* MQTT_PASSWORD  = "REPLACE_ME_PASSWORD";
const int   MQTT_PORT      = 1883;

const char* TOPIC_DI1       = "opta/di1";
const char* TOPIC_HEARTBEAT = "opta/heartbeat";

// ============================================================
// KONFIG - I/O
// ============================================================
const int DI1_PIN = A0;

// ============================================================
// TIDER
// ============================================================
const unsigned long MQTT_RECONNECT_MS    = 3000;
const unsigned long MQTT_HEARTBEAT_MS    = 30000;
const unsigned long SERIAL_HEARTBEAT_MS  = 10000;

// ============================================================
// OBJEKTER
// ============================================================
EthernetClient ethClient;
MqttClient mqttClient(ethClient);

// ============================================================
// TILSTAND
// ============================================================
bool currentDi1State = false;
bool lastPublishedDi1State = false;
bool firstScan = true;

unsigned long lastReconnectAttemptMs = 0;
unsigned long lastMqttHeartbeatMs = 0;
unsigned long lastSerialHeartbeatMs = 0;

// ============================================================
// HJELPEFUNKSJONER
// ============================================================
void printIpAddress(IPAddress ip) {
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.println(ip[3]);
}

bool connectEthernet() {
  Serial.println("Starter Ethernet...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP feilet, bruker fallback IP");
    Ethernet.begin(mac, fallbackIp);
    delay(1000);

    Serial.print("Lokal IP (fallback): ");
    printIpAddress(Ethernet.localIP());
    return true;
  }

  delay(1000);

  Serial.print("Lokal IP (DHCP): ");
  printIpAddress(Ethernet.localIP());
  return true;
}

bool connectMqtt() {
  Serial.println("Forsoker MQTT connect...");

  mqttClient.setId(MQTT_CLIENT_ID);
  mqttClient.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);

  if (mqttClient.connect(brokerIp, MQTT_PORT)) {
    Serial.println("MQTT OK");
    return true;
  }

  Serial.print("MQTT FAIL, error code: ");
  Serial.println(mqttClient.connectError());
  return false;
}

bool publishBool(const char* topic, bool value) {
  mqttClient.beginMessage(topic);
  mqttClient.print(value ? "true" : "false");
  bool ok = mqttClient.endMessage();

  Serial.print("Publiserte ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.print(value ? "true" : "false");
  Serial.print(" | OK: ");
  Serial.println(ok ? "YES" : "NO");

  return ok;
}

bool publishHeartbeat() {
  unsigned long uptimeS = millis() / 1000UL;

  mqttClient.beginMessage(TOPIC_HEARTBEAT);
  mqttClient.print("{");
  mqttClient.print("\"device\":\"");
  mqttClient.print(MQTT_CLIENT_ID);
  mqttClient.print("\",");
  mqttClient.print("\"alive\":true,");
  mqttClient.print("\"di1\":");
  mqttClient.print(currentDi1State ? "true" : "false");
  mqttClient.print(",");
  mqttClient.print("\"uptime_s\":");
  mqttClient.print(uptimeS);
  mqttClient.print("}");

  bool ok = mqttClient.endMessage();

  Serial.print("Heartbeat publish: ");
  Serial.println(ok ? "OK" : "FAIL");

  return ok;
}

void updateLed() {
  digitalWrite(LED_BUILTIN, mqttClient.connected() ? HIGH : LOW);
}

// ============================================================
// SETUP
// ============================================================
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DI1_PIN, INPUT);

  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("=== SETUP START ===");

  connectEthernet();
  connectMqtt();

  currentDi1State = digitalRead(DI1_PIN);
  lastPublishedDi1State = currentDi1State;
  firstScan = true;

  Serial.print("Initial DI1 state: ");
  Serial.println(currentDi1State ? "true" : "false");

  updateLed();
}

// ============================================================
// LOOP
// ============================================================
void loop() {
  mqttClient.poll();
  updateLed();

  unsigned long now = millis();

  // MQTT reconnect
  if (!mqttClient.connected()) {
    if (now - lastReconnectAttemptMs >= MQTT_RECONNECT_MS) {
      lastReconnectAttemptMs = now;
      Serial.println("MQTT disconnected, prover reconnect...");
      connectMqtt();
      updateLed();
    }
  }

  // Les DI1 direkte
  currentDi1State = digitalRead(DI1_PIN);

  // Første scan: synk intern state uten å sende event
  if (firstScan) {
    firstScan = false;
    lastPublishedDi1State = currentDi1State;
  }

  // Send DI1 kun ved endring
  if (mqttClient.connected() && currentDi1State != lastPublishedDi1State) {
    if (publishBool(TOPIC_DI1, currentDi1State)) {
      lastPublishedDi1State = currentDi1State;
    } else {
      Serial.println("DI1-endring ble ikke sendt, prover igjen.");
    }
  }

  // Serial heartbeat
  if (now - lastSerialHeartbeatMs >= SERIAL_HEARTBEAT_MS) {
    lastSerialHeartbeatMs = now;

    Serial.print("MQTT connected: ");
    Serial.println(mqttClient.connected() ? "YES" : "NO");

    Serial.print("DI1: ");
    Serial.println(currentDi1State ? "true" : "false");

    Serial.print("Uptime (s): ");
    Serial.println(now / 1000UL);
  }

  // MQTT heartbeat
  if (mqttClient.connected() && (now - lastMqttHeartbeatMs >= MQTT_HEARTBEAT_MS)) {
    lastMqttHeartbeatMs = now;
    publishHeartbeat();
  }
}
