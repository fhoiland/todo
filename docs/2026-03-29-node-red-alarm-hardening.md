# Node-RED alarm hardening – 2026-03-29

## Summary
This note captures the current hardening work completed on the Node-RED heartbeat and alarm flow.

The work moved the solution from a single-device proof of concept toward a more production-oriented monitoring model.

## Completed steps

### 1. Dynamic device ID
The heartbeat path no longer depends on a fixed internal device ID in the main processing path.

`Heartbeat test` now injects `msg.deviceId`, and `Heartbeat inn` reads the device identity from the incoming message.

This allows the same flow to handle multiple devices instead of only one hardcoded unit.

### 2. Device validation
A whitelist check was added in `Heartbeat inn`.

Only known device IDs are accepted. Unknown IDs are dropped and logged.

This is the first basic protection against malformed, unexpected or hostile input.

### 3. Persistent state storage
Node-RED context storage was moved to persistent filesystem-backed storage using `contextStorage` with `localfilesystem`.

The flow was updated to use explicit context store access:
- `flow.get("devices", "default")`
- `flow.set("devices", devices, "default")`

This means device state can survive Node-RED restart instead of being lost on reboot.

### 4. Multi-device capable offline logic
`Check offline` was rewritten so it no longer checks only one hardcoded device.

It now loops through all entries in the `devices` object and evaluates timeout state per device.

This is the core structural change required for real multi-device monitoring.

### 5. Anti-flapping / suspect state
The alarm logic now uses an intermediate state model:
- `online`
- `suspect`
- `offline`

A timeout does not immediately produce an OFFLINE alarm.
Instead:
1. first missed timeout -> `suspect`
2. second missed timeout -> `offline`

A valid heartbeat resets `missed_count` and returns the device to `online`.

This reduces nuisance alarms caused by short communication disturbances.

## Current devices object
```javascript
{
  "test-enhet-1": {
    state: "online" | "suspect" | "offline" | "unknown",
    last_seen: 1234567890,
    missed_count: 0
  }
}
```

## Current design principles
- No direct notification logic in PLC/device
- Centralized alarm handling in Node-RED
- State-based suppression to avoid repeated spam
- Timeout-based offline detection
- Input validation on incoming device identity
- Persistent storage for restart resilience

## Verified behaviour
- One ONLINE message on transition to online
- No ONLINE spam while already online
- OFFLINE message only after suspect -> offline transition
- State survives Node-RED restart through persistent context
- Unknown device IDs are rejected

## Remaining recommended work
1. Tune heartbeat interval, timeout and check interval as one coordinated timing model
2. Run structured multi-device simulation tests
3. Improve message format and severity conventions
4. Add stronger API authentication / authorization in front of the heartbeat endpoint
5. Consider MQTT migration for production communication path

## Operational note
This milestone represents a meaningful shift from hobby-style test logic to a more industrial monitoring architecture with clearer failure handling and better security posture.
