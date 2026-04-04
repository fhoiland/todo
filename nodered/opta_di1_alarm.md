# Opta DI1 alarm flow

Denne dokumentasjonen beskriver den eksporterte Node-RED-flowen i `nodered/flows_opta_di1_alarm.json`.

## Formål
Flowen sender varsel til ntfy når DI1 fra Arduino Opta er aktiv, med enkel beskyttelse mot varselspam.

## Hovedkjede

```text
MQTT DI1 in - Opta
-> Filter DI1 alarm true
-> Rate limit DI1 24h
-> Build ntfy DI1 alarm
-> HTTP ntfy send
-> Debug ntfy response
```

## Hva hver node gjør

### MQTT DI1 in - Opta
Abonnerer på topic `opta/di1` fra broker `mosquitto`.

### Filter DI1 alarm true
Slipper gjennom bare boolske verdier `true` og `false`, slik at neste node kan vite både når alarmen er aktiv og når den har vært borte.

### Rate limit DI1 24h
Sender ikke samme alarm kontinuerlig.
- Ny `true` kan sendes igjen etter 24 timer.
- Hvis signalet har vært `false` i minst 10 minutter, nullstilles resend-logikken slik at ny alarm kan sendes før 24 timer har gått.

### Build ntfy DI1 alarm
Bygger overskrift, prioritet, tags og alarmtekst for ntfy.
Denne noden sender bare videre meldinger når `msg.payload === true`.

### HTTP ntfy send
Sender HTTP POST til lokal ntfy-instans via intern Docker-adresse.

### Debug ntfy response
Viser responsen fra ntfy-kallet i debug-panelet.

## Viktig avgrensning
Den eksporterte flowen inneholder også eldre test- og heartbeat-relaterte noder i samme tab. Disse er ikke en del av hovedkjeden for DI1-alarm, men ligger fortsatt i eksporten fordi fanen ikke er helt separert ennå.

## Viktig sikkerhetsregel
Eksporterte flows skal ikke inneholde klartekst-passord, tokens eller andre hemmeligheter. Credentials må holdes utenfor repoet eller anonymiseres før commit.