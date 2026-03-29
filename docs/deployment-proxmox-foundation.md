# Proxmox foundation – reusable setup

## Goal
Simple, repeatable deployment with minimal troubleshooting.

## Core containers
- Node-RED
- ntfy
- (optional) Mosquitto

## Docker network

Create shared network:

```bash
docker network create iot_net
```

All containers must join this network.

## Node-RED
- Port 1880
- Persistent /data volume
- Connected to iot_net
- Enable persistent context storage

## ntfy
- Internal port 80
- Optional host port (e.g. 8095)
- Connected to iot_net
- Auth enabled
- Persistent data volume

## Internal communication

Use Docker DNS:

```text
http://ntfy/maskinvarsler
```

Never use:
- host IP
- localhost between containers

## Security baseline
- No direct internet exposure of Node-RED
- Use Cloudflare Tunnel
- Use dedicated service users
- No secrets in GitHub

## Persistence
- Node-RED: /data
- ntfy: /var/lib/ntfy

## Test checklist
1. Node-RED reachable
2. ntfy reachable
3. Node-RED can POST to ntfy
4. Notification received

## Reuse
Copy setup and only change:
- topics
- device IDs
- alarm logic
