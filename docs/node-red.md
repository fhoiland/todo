# Node-RED

## Endpoints
- POST /api/heartbeat
  - body: { device_id, status, ts }

## Flow (high-level)
HTTP IN -> Auth check -> Function: update state -> Switch: change? -> Telegram

## State model
flow.devices = {
  device_id: {
    last_seen: epoch,
    online: bool
  }
}

## Offline detection
- Trigger if now - last_seen > threshold (e.g. 120s)

## Security
- Require token header
- Do not export credentials to repo

## Export policy
- Use sanitized exports (no creds)
