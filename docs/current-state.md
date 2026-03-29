# Current state

## Phase status

**Phase 1 is complete:** backend foundation, alarm engine hardening, notification path, and container networking.

**Phase 2 is not started yet:** real device ingress from Opta or simulator.

This distinction is important. The current platform is a hardened backend and test foundation, not yet a full end-to-end field deployment.

## What is completed

### Core backend and alarm engine
- Node-RED alarm engine is running in Docker
- Device state is tracked in a `devices` object in flow context
- Multi-device support is implemented
- Device IDs are validated through a whitelist in the heartbeat path
- State is persisted using filesystem-backed Node-RED context storage
- Anti-flapping logic is implemented with `online -> suspect -> offline`
- Heartbeat/check timing model has been tuned for deterministic behaviour in the current test setup

### Notification path
- ntfy is self-hosted in Docker
- Authentication is enabled
- A dedicated `nodered` user is used for publishing
- Node-RED publishes to ntfy using Docker DNS instead of host LAN IP
- Notification format includes device ID and timestamp

### Networking and exposure
- No open inbound ports are required for external administrative access
- Cloudflare Tunnel is used for secure remote access to the environment
- Container-to-container communication between Node-RED and ntfy now uses the shared Docker network

## What is not completed

### Real device ingress
The real heartbeat ingress from Opta is not connected yet.

The current platform uses test/inject nodes and internal validation of the alarm engine.

### API protection for field ingress
Since the real device ingress is not active yet, final API protection for `/api/heartbeat` is not implemented.

Before connecting Opta or another real device, the ingress layer must include:
- API key or token
- payload validation
- clear error handling for malformed input

### End-to-end field validation
The following tests remain for the next phase:
- Opta on same LAN as server
- Opta on different network through external path
- behaviour during disconnects, delayed heartbeats and reconnects

## Current recommended interpretation

This repository should currently be read as:

> Hardened backend foundation for industrial-style monitoring and notification, ready for simulator or real device integration.

It should **not** yet be read as:

> Fully completed field-integrated production system.

## Current devices object

```javascript
{
  "plc-001": {
    state: "online" | "suspect" | "offline" | "unknown",
    last_seen: 1234567890,
    missed_count: 0
  }
}
```

## Current internal notification path

```text
Node-RED container -> http://ntfy/maskinvarsler -> ntfy container
```

## Next phase
1. Add simulator or real Opta heartbeat ingress
2. Add API authentication for field ingress
3. Test same-network and external-network device scenarios
4. Decide whether final field transport should be HTTP or MQTT
