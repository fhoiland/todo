# Target architecture

## Goal
Build a robust, scalable and movable monitoring/alerting system where both the server and Opta units can be relocated without redesigning the solution.

The design must support:
- local operation over Ethernet
- operation across different sites and networks
- clean separation between control logic, transport, integration and remote administration
- simple fault isolation and future scaling to multiple devices

## Core design principles
1. **PLC is the source of truth**
   - All real IO logic, debounce, delays, alarm state and local fail-safe behavior belong in PLC/ST.
   - The communication layer must not own process logic.

2. **Sketch is a communication gateway**
   - Reads shared variables from PLC.
   - Publishes device state.
   - Handles reconnect and transport details.
   - Must stay as dumb as possible.

3. **MQTT is the primary transport**
   - Primary field-to-server communication is MQTT.
   - HTTP may be kept for testing/admin only, not as the main production path.

4. **Node-RED is integration, not control**
   - Node-RED handles alerting, state aggregation, dashboards and integrations.
   - Critical machine behavior must not depend on Node-RED being online.

5. **Cloudflare is for administration, not field transport**
   - Use Cloudflare Tunnel / Access for secure remote access to server tools.
   - Do not make field communication depend on Cloudflare where local network communication can be used directly.

6. **Movable by design**
   - IP addresses, credentials and site-specific values must be configurable.
   - No hard coupling to one router, one subnet or one site.

## Target layers

### Layer 1 - Field control
**Device:** Arduino Opta PLC runtime

Responsibilities:
- read digital inputs
- filter/debounce signals
- apply on-delay/off-delay logic
- manage local states such as fault, power loss, healthy, startup
- drive local outputs / local indication
- expose shared variables for communication

### Layer 2 - Communication gateway
**Device:** Arduino sketch side on Opta

Responsibilities:
- initialize Ethernet/Wi-Fi transport as needed
- connect to MQTT broker
- publish structured state from PLC shared vars
- publish heartbeat / availability information
- subscribe only if future commands are explicitly required
- reconnect automatically after network interruptions

Non-responsibilities:
- no process decisions
- no alarm latching logic
- no timing logic that belongs to the machine state

### Layer 3 - Messaging backbone
**Service:** Mosquitto MQTT broker

Responsibilities:
- receive messages from Opta units
- provide stable decoupling between devices and consumers
- support scaling to multiple Opta devices and future clients

Requirements:
- authentication enabled
- topic naming standard
- later: TLS where justified

### Layer 4 - Integration and alerting
**Service:** Node-RED

Responsibilities:
- subscribe to MQTT topics
- update device status model
- detect missing heartbeats if needed at system level
- send alerts via ntfy
- provide dashboards / debug / auxiliary logic

Non-responsibilities:
- no critical field control
- no single source of truth for machine state

### Layer 5 - Remote administration
**Service:** Cloudflare Tunnel / Access

Responsibilities:
- secure remote access to Node-RED, SSH and admin endpoints
- avoid inbound port forwarding

Non-responsibilities:
- should not be required for local Ethernet operation between Opta and server

## Network philosophy

### Preferred local mode
At a site with local server access:
- Opta connects by Ethernet to local network/switch
- Server is on the same local network or routed industrial subnet
- MQTT traffic stays local
- Remote access is optional and only for administration

### Movable mode
When server or Opta is moved:
- only site configuration changes
- same topic structure and application logic remain
- no redesign of PLC logic required
- no redesign of Node-RED flow architecture required

## Initial migration direction
Current project history has included HTTP heartbeat based communication. The target production direction is:

1. keep PLC logic in ST
2. reduce sketch to gateway role
3. move heartbeat/state publishing to MQTT
4. let Node-RED consume MQTT and send ntfy alerts
5. keep Cloudflare for admin access only

## Definition of success
The solution is considered architecturally correct when:
- an Opta can be dropped into a local Ethernet network and report state via MQTT
- the server can be moved to another site with only configuration changes
- Node-RED restart does not break local machine logic
- Cloudflare outage does not break local Opta-to-server communication
- adding a second and third Opta does not require redesign of the system
