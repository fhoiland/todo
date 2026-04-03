# Nåværende systemstatus

## Mål
Bygge en robust og skalerbar overvåkingskjede for Arduino Opta, MQTT, Node-RED og mobilvarsling, med fokus på lokal drift, sikker autentisering og enkel videreutvikling.

## Bekreftet fungerende kjede

```text
Arduino Opta -> Mosquitto MQTT -> Node-RED -> ntfy -> mobil
```

## Status per delsystem

### Arduino Opta
- Leser DI1 og publiserer til MQTT.
- Kommunikasjon fra Opta til broker er verifisert.
- MQTT payload er verifisert som `boolean`, ikke streng.

### Mosquitto
- Kjører i Docker.
- Broker og autentisering er verifisert.
- Node-RED kobler til broker via container-navn, ikke IP.

### Node-RED
- Mottar MQTT-meldinger fra Opta.
- Filtrering på alarmtilstand er verifisert.
- Varselkjede mot ntfy fungerer.
- Enkel resend-begrensning er lagt inn for å unngå varselspam.

### ntfy
- Kjører lokalt i Docker.
- Webgrensesnitt er tilgjengelig via port 8095.
- Varsler fra Node-RED til mobil er verifisert.

## Viktige tekniske avklaringer
- `msg.payload` for DI1-alarm er `true/false` som boolean.
- Node-RED må derfor ikke sammenligne mot strengverdien `"true"`.
- Interne Docker-kall skal bruke container-navn som `mosquitto` og `ntfy`.
- Eksterne nettleserkall til ntfy skal gå via server-IP og eksponert port.

## Dette er ferdig
- Ende-til-ende alarm fra Opta til mobil.
- Lokal ntfy-integrasjon.
- Node-RED-filtrering og enkel rate limiting.

## Dette gjenstår
- Rydde Node-RED-flow og navngiving.
- Dokumentere faktisk topic-struktur.
- Eksportere og lagre Node-RED-flow i repoet.
- Legge inn heartbeat/offline-overvåking.
- Legge til `normal igjen`-varsling.
- Dokumentere Opta-kode og Mosquitto-konfig.

## Viktig avgrensning
Dette repoet skal dokumentere struktur, konfigurasjon og arbeidsmåte uten å lekke hemmeligheter. Passord, tokens, nøkler og sensitiv intern nettverksinformasjon skal ikke committes.