# Node-RED alarm flow – current state

## Purpose
This document captures the current test implementation of the local alarm engine in Node-RED after ntfy integration was verified.

## What is confirmed working
- ntfy is running locally in Docker.
- ntfy user auth and web login are working.
- Node-RED can publish to ntfy topic `maskinvarsler`.
- Manual `ONLINE` and `OFFLINE` test messages are working.
- State-based suppression is working for manual transitions, so repeated identical events do not spam notifications.

## Current flow structure
The test flow currently contains these paths:

1. `Heartbeat test` -> `Heartbeat inn` -> `Send til ntfy`
2. `Check timeout` -> `Check offline` -> `Send til ntfy`
3. `Trigger OFFLINE` -> `OFFLINE logic` -> `Send til ntfy`
4. `Trigger ONLINE` -> `ONLINE logic` -> `Send til ntfy`
5. `Send til ntfy` -> `ntfy svar`

## Logic implemented
### Heartbeat path
`Heartbeat inn`:
- stores `last_seen`
- checks `device_state`
- sends `ONLINE` only when the state changes to `online`
- suppresses repeated `ONLINE` messages when already online

### Timeout path
`Check offline`:
- reads `last_seen`
- compares current time against timeout
- changes `device_state` to `offline` when heartbeat timeout is exceeded
- sends `OFFLINE` only once per transition

### Manual test paths
Manual trigger nodes remain in the flow as test tools for controlled verification of transitions.

## State variables used
The current test implementation uses flow context:
- `device_state`
- `last_seen`

## Status of latest step
The flow layout has been built and visually checked.
The final automatic timeout behaviour had not yet been fully field-tested at the time of this note, but the structure and logic were in place and appeared correct.

## Expected behaviour when tested
- no heartbeat for configured timeout -> one `OFFLINE` notification
- heartbeat returns after offline -> one `ONLINE` notification
- repeated heartbeat while online -> no spam
- repeated timeout checks while offline -> no spam

## Next work
- verify timeout behaviour end-to-end in live test
- remove or isolate manual trigger nodes when no longer needed
- replace test heartbeat with simulator or real device input
- move to device-specific IDs instead of single test state
- extend from single-device state to multi-device monitoring
