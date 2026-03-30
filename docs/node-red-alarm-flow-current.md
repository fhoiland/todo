# Node-RED alarm flow (current)

## Device structure

devices = {
  "plc-001": {
    state: "online" | "suspect" | "offline" | "unknown",
    last_seen: timestamp,
    missed_count: number
  }
}

## Heartbeat logic

- Validate deviceId
- Update last_seen
- Reset missed_count
- Send ONLINE only on change

## Timeout logic

- Increase missed_count
- Move to SUSPECT
- Move to OFFLINE after threshold

## Anti-flapping

ONLINE → SUSPECT → OFFLINE
OFFLINE → ONLINE

## Notification

- Only on state change
- Includes deviceId and timestamp
