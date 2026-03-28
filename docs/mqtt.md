# MQTT

## Broker
- Mosquitto (internal)
- Port: 1883 (internal only)

## Topic structure
- devices/{device_id}/status
- devices/{device_id}/heartbeat
- devices/{device_id}/alarm

## Payload (JSON)
{
  "ts": 0,
  "status": "online|offline",
  "value": 0
}

## Client requirements (Opta)
- Auto reconnect
- Keepalive configured
- Unique client_id

## Security
- Username/password required
- TLS (future step)
