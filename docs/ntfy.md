# ntfy – lokal varslingsplattform

## Formål
ntfy brukes som lokal og kontrollert varslingskanal for systemet.

Den erstatter tidligere vurdert Telegram-løsning og gir:
- enklere integrasjon
- full kontroll (self-host)
- ingen ekstern avhengighet

## Oppsett
- Kjøres i Docker på Ubuntu-server
- Eksponert på intern port (8095)
- Ikke direkte eksponert til internett
- Planlagt eksponert via Cloudflare Tunnel senere

## Konfigurasjon
- Lokal database for cache og auth
- `deny-all` som default access
- Web-login aktivert
- Egen admin-bruker
- Egen Node-RED-bruker med begrenset tilgang

## Topics
- `maskinvarsler` – hovedkanal for alarm
- `test` – testkanal brukt under oppsett

## Integrasjon med Node-RED
- HTTP POST brukes for publisering
- Basic auth / token brukes for autentisering
- Payload sendes som tekst

Eksempel:
```
POST /maskinvarsler
Content-Type: text/plain

🚨 OFFLINE - enhet
```

## Designvalg
- Varsling håndteres sentralt i Node-RED
- ntfy er kun transport/visning
- Ingen direkte varsling fra PLC/feltenhet

## Videre arbeid
- Flytte ntfy bak Cloudflare (HTTPS)
- Ta i bruk Bearer tokens i stedet for passord
- Definere flere topics (kritisk, debug, etc.)
