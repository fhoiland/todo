# Industriell overvåking og varsling – prosjektoversikt

## Formål
Dette repoet dokumenterer oppsettet vårt for robust overvåking, varsling og fjernaksess rundt Arduino Opta, Node-RED, MQTT, Ubuntu-server og Cloudflare.

Målene er:
- få stabil kommunikasjon fra feltenheter til server
- få varsling ved feil, offline og tilbake online
- unngå port forwarding og usikre eksponeringer
- dokumentere løsningene slik at de kan driftes, feilsøkes og videreutvikles
- bygge løsningen stegvis med fokus på både elkraft/automatisering og cybersikkerhet

## Hovedkomponenter
- **Arduino Opta**: feltenhet / lokal IO / PLC-logikk / sketch-del for kommunikasjon
- **Node-RED**: API, logikk, meldingsruting, varsling og integrasjoner
- **Mosquitto MQTT**: meldingsbroker for mer robust og skalerbar kommunikasjon
- **Ubuntu Server**: vert for Node-RED, MQTT og støttetjenester
- **Cloudflare Tunnel / Access**: sikker ekstern tilgang uten åpne porter

## Arbeidsprinsipper
- endre én ting om gangen
- dokumentere alle viktige endringer
- ha tydelig skille mellom testoppsett og produksjonsnære valg
- prioritere robuste og sikre designvalg framfor raske snarveier
- behandle nettverk, autentisering og eksponering som sikkerhetskritiske områder

## Struktur i repoet
- `docs/architecture.md` – teknisk arkitektur og dataflyt
- `docs/security.md` – sikkerhetskrav og herdingspunkter
- `TODO.md` – aktiv oppgaveliste og backlogg

## Nåværende fokus
1. dokumentere dagens oppsett
2. stabilisere heartbeat / varsling
3. forbedre MQTT-løpet mellom Opta og server
4. dokumentere sikker tilgang via Cloudflare
5. gjøre løsningen lettere å drifte og feilsøke
