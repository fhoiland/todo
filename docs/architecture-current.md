# Current architecture

## Overview

This is the actual architecture in use in Phase 1.

```text
Device (simulated)
        ↓
Node-RED (alarm engine)
        ↓
ntfy (notifications)
```

## Deployment model

All services run in Docker.

```text
Node-RED container -> http://ntfy/maskinvarsler -> ntfy container
```

## Key characteristics

- Centralized alarm logic in Node-RED
- State-based processing (not event-based spam)
- Multi-device support
- Persistent state using Node-RED context storage
- No dependency on host LAN IP
- Internal communication via Docker network (`iot_net`)

## External access

- Cloudflare Tunnel is used for remote access
- No direct port exposure required

## Future extension

Next phase will introduce:

```text
Opta / device -> HTTP or MQTT -> Node-RED
```

This will reuse the same alarm engine.
