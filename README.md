# Wiegedings

Waage für den Füllstand einer Gasflasche mit Integration in Home Assistant

Verwendete Hardware:
- ESP-01 (esp8266)
- HX711 Load cell amplifier
- 4x 50kg Wägezellen

## Pinout am ESP-01
| Pin | Verwendung | Anmerkung |
| --- | --- | --- |
| GPIO0 | Data von HX711 | 10k pullup, GND zum Programmieren 
| GPIO1 | Serial TX |   
| GPIO2 | SCK zu HX711 | 10k pullup 
| GPIO3 | Serial RX |  
| CH_PD |  | 10k pullup
| RST |  | 10k pullup
| VCC |  | 3,3V


## MQTT
Steuerung und Übermittlung der Werte per Home Assistant MQTT Integration.

Folgende Entitäten werden per Autodiscovery an Home Assistant übermittelt:
| Entity Type | Feature
| --- | --- 
| Sensor | Type "weight"
| Sensor | Füllstand in %
| Button | Tarieren 
| Button | Kalibrierung durchführen
| Number | Ergebnis der Kalibrierung
| Number | Gewicht zum Kalibrieren
| Number | Flasche Eigengewicht
| Number | Flasche Füllung
