# Security Guidelines

## 🔐 Core principle
Never store secrets in GitHub.

## ❌ Never commit
- Passwords
- API tokens
- MQTT credentials
- Cloudflare credentials
- Private keys / certificates

## ✅ Use instead
- Environment variables (.env)
- Node-RED credential storage
- Secure storage on server

## 🌐 Network security
- No open ports (use Cloudflare Tunnel)
- Segment services where possible
- Avoid exposing MQTT externally without TLS

## 🔑 Authentication
- Use strong passwords
- Prefer token-based auth for APIs
- Use Cloudflare Access for external login

## 🧠 Industrial mindset
- Assume network is hostile
- Validate all incoming data
- Log important events
- Design for failure (watchdog, reconnect)

## 📋 Checklist
- [ ] No secrets in repo
- [ ] MQTT secured
- [ ] Node-RED protected
- [ ] Cloudflare access configured
- [ ] Logs enabled
