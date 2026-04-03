# MQTT til ntfy status

Denne filen beskriver den løsningen som faktisk er verifisert nå.

Bekreftet kjede:
Arduino Opta -> Mosquitto MQTT -> Node-RED -> ntfy -> mobil

Bekreftet:
- Opta publiserer til MQTT.
- Mosquitto mottar meldinger.
- Node-RED mottar meldinger fra broker.
- Node-RED sender videre til ntfy.
- ntfy sender varsel til mobil.

Viktige detaljer:
- MQTT payload for DI1 er boolean.
- Node-RED må ikke sammenligne mot strengverdien true.
- Intern Docker-kommunikasjon skal bruke container-navn.
- Lokal webtilgang til ntfy går via vertens eksponerte port 8095.

Neste steg:
- rydde Node-RED-flow
- eksportere flow til repo
- legge til normal igjen-varsling
- legge til heartbeat og offline-detektering
- standardisere topics og payloads
- dokumentere Opta-kode og broker-konfig