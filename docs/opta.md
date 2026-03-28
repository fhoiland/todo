# Arduino Opta

## Responsibilities
- Read inputs (DI)
- Handle local alarm logic (PLC)
- Send heartbeat / status
- Trigger alarms

## Split architecture
- PLC (Structured Text): control logic
- Sketch: networking (WiFi, HTTP/MQTT)

## Minimum signals
- Alarm_Active_Local
- Cmd_SendFault
- Heartbeat interval

## Communication
- HTTP (current)
- MQTT (planned)

## Reliability
- Watchdog enabled
- Reconnect WiFi
- Retry send
