/**
 * @author Henrik Wahlström (henrik.g.wahlstrom@gmail.com)
 * @date 2025-01-12
 * 
 * Based on the example ESP_NOW_Serial from the ESP_NOW library included with
 * the esp32 board manager.
 * 
 * Forward Serial data to and from a peer device using ESP-NOW.
 */

#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"

// Transmission speed used for serial communication in bits per second (bps).
const auto baud_rate = 115200;

// Set the MAC address of the device that will receive the data as six
// hexadecimal values. For example: EC:DA:3B:65:D0:88 is written as 
// { 0xEC, 0xDA, 0x3B, 0x65, 0xD0, 0x88 }.
const auto peer_mac_address = MacAddress{ 0xEC, 0xDA, 0x3B, 0x65, 0xD0, 0x88 };
//const auto peer_mac_address = MacAddress{ 0xEC, 0xDA, 0x3B, 0x65, 0xDC, 0x18 };

// Wi-Fi channel to be used by the ESP-NOW protocol. Channels 1 to 13 are
// allowed in Europe.
const auto wifi_channel = 1;

// While 'Serial' is used to send data over USB or the TX/RX pins, 'Wireless'
// sends data over Wi-Fi using the ESP-NOW protocol. You use 'Wireless' in the
// same way as 'Serial', using .read(), .write(), .print(), .println(), etc.
auto Wireless = ESP_NOW_Serial_Class(peer_mac_address, wifi_channel, WIFI_IF_STA);

void setup() {
  // Initialize serial communication.
  Serial.begin(baud_rate);
  // Wait for the serial connection to be established to make sure the prints
  // in setup() are shown.
  delay(2000);

  // Initialize Wi-Fi and wait for it to complete.
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(wifi_channel, WIFI_SECOND_CHAN_NONE);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  // Print the mac address to be used by the peer device.
  Serial.print("MAC address of this device: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Use this as the peer mac address on your other device.");

  // Initialize ESP-NOW communication.
  Serial.println("ESP-NOW communication starting...");
  Wireless.begin(baud_rate);
  Serial.println("Serial data received will now be forwarded to the peer device.\n");
}

void loop() {
  // Serially forward wireless data from the peer device to serial.
  while (Wireless.available()) {
    Serial.write(Wireless.read());
  }

  // Forward serial data to the peer device wirelessly.
  while (Serial.available() && Wireless.availableForWrite()) {
    Wireless.write(Serial.read());
  }
}
