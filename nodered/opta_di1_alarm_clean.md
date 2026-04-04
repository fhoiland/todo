# Opta DI1 alarm flow (clean tab)

Denne dokumentasjonen beskriver den rene Node-RED-tabben i `nodered/flows_opta_di1_alarm_clean.json`.

## Formål
Flowen sender varsel til ntfy når DI1 fra Arduino Opta er aktiv, med kontroll på resend og reset slik at systemet ikke spammer, men fortsatt kan sende ny alarm når signalet har vært borte en stund.

## Hovedkjede

```text
MQTT DI1 in - Opta
-> Filter DI1 alarm true
-> Rate limit DI1 24h
-> Build ntfy DI1 alarm
-> HTTP ntfy send
-> Debug ntfy response
```

## Hva flowen gjør
- Leser `opta/di1` fra Mosquitto.
- Slipper bare gjennom boolske `true` og `false`.
- Sender alarm når DI1 er aktiv.
- Hindrer kontinuerlig gjentatt varsling.
- Tillater ny alarm når signalet har vært `false` lenge nok.
- Sender til lokal ntfy-instans via intern Docker-adresse.

## Gjeldende alarmtekst
- Title: `Hyttedass alarm`
- Message: `🚨💩 Alarm: Nå koker det i do-systemet`

## Gjeldende resend/reset-logikk
I den eksporterte flowen er logikken:
- resend-vindu: 24 timer
- reset etter alarm borte: 30 sekunder

Det betyr:
- første `true` sender alarm
- ny `true` blokkeres mens alarmen fortsatt regnes som aktiv
- hvis signalet har vært `false` i minst 30 sekunder, kan neste `true` sende ny alarm
- hvis alarmen står lenge, kan samme alarm sendes igjen etter 24 timer

## Viktig teknisk detalj
`Title` sendes som HTTP-header til ntfy. Emoji i `Title` kan gi ugyldig header og føre til feil. Derfor brukes ren tekst i `Title`, mens emoji brukes i meldingsinnholdet.

## Viktig sikkerhetsregel
Eksporterte flows skal ikke inneholde klartekst-passord, tokens eller andre hemmeligheter. Credentials må holdes utenfor repoet eller anonymiseres før commit.

## Sannhetskilde
Bruk denne filen og `flows_opta_di1_alarm_clean.json` som gjeldende dokumentasjon for V1 av DI1-alarmen. Eldre filer beskriver tidligere blandede eller mindre ryddige versjoner av flowen.