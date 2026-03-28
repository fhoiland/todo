# Progress log

## 2026-03

### Infrastruktur og tilgang
- Ubuntu-server satt opp som host for Node-RED, Mosquitto og støttetjenester.
- Cloudflare Tunnel tatt i bruk for sikker ekstern tilgang uten åpne inbound-porter.
- SSH via Cloudflare testet og brukt som administrasjonsvei.
- Docker-miljø i drift for flere tjenester på samme VM.

### Node-RED og alarmplattform
- Node-RED kjører stabilt i Docker.
- HTTP-endepunkt for heartbeat er satt opp og testet.
- Uautorisert POST mot `/api/heartbeat` ga `Unauthorized`, som bekreftet at auth-logikk er aktiv i API-laget.
- Arkitekturen er bevisst flyttet mot sentral alarm- og overvåkingslogikk i Node-RED i stedet for direkte varsling fra feltenhet.

### MQTT
- Mosquitto er installert og kjører i Docker.
- Broker brukes som planlagt kommunikasjonsvei for mer robust og skalerbar enhetsintegrasjon.
- MQTT/TLS-løpet er fortsatt under arbeid og ikke definert som ferdig produksjonsklart.

### Varsling
- Telegram ble først vurdert og testet i tidligere løp, men er valgt bort som primær kanal.
- ntfy er installert lokalt i Docker på serveren.
- ntfy er konfigurert med lokal lagring, brukerautentisering og web-login.
- Egen bruker for Node-RED-publisering er opprettet.
- Testmeldinger fungerer både via curl og fra Node-RED til topic `maskinvarsler`.

### Opta / feltenhet
- Det er jobbet med både HTTPS- og MQTT-retning for Opta.
- Opta-delen er ikke landet ennå; fokus er derfor flyttet til server- og plattformdel først.
- Videre arbeid skal bruke simulator/testenhet før endelig hardwarevalg låses.

## Beslutninger tatt
- Bygge backend og varslingsplattform ferdig før endelig valg av feltenhetsløsning.
- Prioritere lokale og kontrollerbare komponenter der det er praktisk mulig.
- Holde tydelig skille mellom testoppsett og senere produksjonsnært design.
- Behandle sikkerhet, autentisering og eksponering som førstelinjekrav.

## Neste tekniske milepæl
1. Lage device-state i Node-RED.
2. Lage offline-detektering med timeout.
3. Lage online-gjenopprettingsvarsel.
4. Lage simulator/testenhet.
5. Koble valgt feltenhet på samme grensesnitt senere.
