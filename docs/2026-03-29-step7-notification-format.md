# Step 7 – Notification format improvements

## Summary
This step focuses on improving the readability and usability of alarm notifications sent from Node-RED to ntfy.

The goal is to make messages clearer, more structured and more suitable for real operational use.

## Changes implemented

### 1. Structured message format
Previous format:
```
ONLINE - deviceId
```

New format:
```
🟢 ENHET ONLINE
Enhet: plc-001
Tid: 29.03.2026, 13:45:10
```

Offline:
```
🔴 ENHET OFFLINE
Enhet: plc-001
Tid: 29.03.2026, 13:45:30
```

### 2. Added timestamp
Each notification now includes a human-readable timestamp:

```javascript
let nowStr = new Date().toLocaleString();
```

This is included in both ONLINE and OFFLINE messages.

## Why this matters

- Easier to read on mobile
- Clear identification of which device triggered the event
- Precise timing for troubleshooting and correlation with other systems

## Operational impact

- No change in logic or state machine
- No change in alarm triggering
- Only improves message presentation layer

## Status

Notification format is now suitable for real-world use and aligns with typical industrial alarm readability requirements.

## Next step

Continue with:
- Multi-device scaling validation (completed)
- Further system hardening (security and API control)
