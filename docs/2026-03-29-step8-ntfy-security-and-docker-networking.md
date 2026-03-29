# Step 8 – ntfy hardening and Docker networking

## Summary
This step secured and cleaned up the current notification path used in the test platform.

The PLC/device side is not connected yet, so the focus in this phase was not on a production heartbeat API, but on securing and hardening the Node-RED -> ntfy path that is actually in use today.

## Changes implemented

### 1. Dedicated ntfy publishing user
Node-RED no longer uses the `admin` account for normal message publishing.

A dedicated `nodered` user is now used for publishing to ntfy.

This follows least-privilege design:
- `admin` for administration only
- `nodered` for operational publishing only

### 2. Privilege separation verified
The active Node-RED flow was checked so that ntfy publishing is tied to the dedicated operational user rather than the administrative account.

### 3. Removed dependency on host LAN IP
The notification path previously depended on a hardcoded host IP in the HTTP request URL:

```text
http://192.168.2.229:8095/maskinvarsler
```

That design would break if the server moved, changed network, or got a different LAN address.

### 4. Attached ntfy container to shared Docker network
The `ntfy` container was connected to the same Docker network as `nodered` (`iot_net`).

This allows container-to-container communication through Docker DNS/service naming rather than host IP.

### 5. Switched Node-RED -> ntfy URL to Docker service name
The Node-RED HTTP request node now targets:

```text
http://ntfy/maskinvarsler
```

This is more robust because it is:
- independent of host LAN IP
- cleaner in a containerized environment
- easier to move between networks

## Why this matters

This step improves both security and operational robustness:

- reduced use of privileged credentials
- cleaner separation of admin and runtime functions
- removal of fragile host-IP dependency
- better portability if the server is moved or renumbered
- stronger alignment with container-native design

## Current architecture result

```text
Node-RED container -> http://ntfy/maskinvarsler -> ntfy container
```

Communication now stays inside Docker networking instead of bouncing through a LAN IP on the host.

## Verified result
- Node-RED still publishes notifications successfully
- ntfy remains reachable after switch to Docker DNS name
- notification path no longer depends on server IP address

## Status

Step 8 is complete for the current test stage.

The platform is now significantly closer to a production-ready v1 foundation, even though the real PLC/device input path is not connected yet.

## Recommended next phase
1. Add a simulator path for realistic device heartbeat testing without hardware
2. Decide whether the future field interface should be HTTP or MQTT
3. If MQTT is chosen, migrate the device communication layer while keeping Node-RED alarm logic centralized
4. Add stronger API protection when the real device ingress is introduced
