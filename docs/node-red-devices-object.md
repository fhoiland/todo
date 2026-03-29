# Node-RED devices object – milestone

## Summary
The alarm flow has now been moved from single flat state variables to a `devices` object stored in Node-RED flow context.

## What was verified
- `Heartbeat inn` now stores data in `devices[deviceId]`
- `Check offline` now reads from the same `devices` object
- The test flow still works end-to-end after the change
- A heartbeat transitions the device to `online`
- Timeout transitions the same device to `offline`
- Notifications include `deviceId`

## Current object shape
```javascript
{
  "test-enhet-1": {
    state: "online" | "offline" | "unknown",
    last_seen: 1234567890
  }
}
```

## Why this matters
This is the first real step from a single-device proof of concept to a scalable multi-device monitoring model.

The flow no longer depends on one global `device_state` and one global `last_seen`.
That means the same logic can later be expanded to several PLCs, Opta units, gateways or simulated devices.

## Current limitation
The flow still uses a fixed device ID:
- `test-enhet-1`

So the internal data model is ready for multiple devices, but the test input is still only feeding one device.

## Next recommended step
Replace the fixed test ID with a message-driven ID so that the same heartbeat path can handle multiple devices, for example:
- `plc-001`
- `plc-002`
- `gateway-01`

## Notes
The timeout behaviour was re-tested after the migration to `devices` object and confirmed working for the test device.
