# Devices object (current)

## Structure

{
  "device-id": {
    state: "online" | "suspect" | "offline" | "unknown",
    last_seen: number,
    missed_count: number
  }
}

## Storage

- Node-RED flow context
- Persistent filesystem storage

## Purpose

- Track device state
- Prevent duplicate notifications
- Enable multi-device support
