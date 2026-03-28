# Architecture

## Overview
Opta (WiFi) -> HTTPS (Cloudflare Tunnel) -> Node-RED (API/logic) -> MQTT (Mosquitto) -> Consumers

## Components
- Opta: IO + PLC + sketch (comm)
- Node-RED: API (/api/heartbeat), routing, alerts
- Mosquitto: broker (1883 internal)
- Ubuntu: host (Docker)
- Cloudflare: Tunnel + Access (no open ports)

## Data flows
1. Heartbeat: Opta -> POST /api/heartbeat
2. Processing: Node-RED updates device state
3. Alerts: Telegram on state change (offline/online)
4. MQTT (planned): Opta publish/subscribe topics

## Network
- No inbound ports
- Outbound-only from Opta
- Internal Docker network for services

## Failure handling
- Timeout-based offline detection
- Reconnect logic on client
- Watchdog on Opta
