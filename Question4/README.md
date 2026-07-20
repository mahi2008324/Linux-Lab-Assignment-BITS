# Question 4 — Real-Time Log Monitoring Command Pipeline

## 📌 Problem Statement

Design a monitoring tool that continuously analyzes a log file and:

1. Displays newly added log entries in real time (`tail -f`)
2. Extracts only ERROR messages (`grep`)
3. Maintains a separate error report file (redirection `>>`)
4. Suppresses unnecessary output (`/dev/null`)

---

## 🛠️ Files in this Folder

| File | Description |
|------|-------------|
| `monitoring_commands.md` | All pipeline commands with detailed explanations |
| `sample.log` | Sample log file with mixed entry types |
| `error_report.log` | Pre-generated error report (also created by commands) |
| `execution_commands.md` | Step-by-step commands |
| `outputs.md` | Sample terminal outputs |
| `explanation.md` | Conceptual explanation of pipes, grep, tail, redirection |
| `screenshots/` | Screenshots of monitoring in action |

---

## 🚀 How to Run

```bash
# Step 1: Monitor log file in real-time (basic)
tail -f sample.log

# Step 2: Monitor and filter only ERRORs
tail -f sample.log | grep "ERROR"

# Step 3: Full pipeline — monitor, filter, and save to report
tail -f sample.log | grep --line-buffered "ERROR" | tee -a error_report.log

# Step 4: Suppress non-error output to /dev/null
tail -f sample.log | grep --line-buffered "ERROR" >> error_report.log 2>/dev/null

# Step 5: Test by injecting log entries in another terminal
echo "[2026-07-20 21:30:00] ERROR: Connection timeout" >> sample.log
```

---

## 🔗 Related Files

- [monitoring_commands.md](./monitoring_commands.md)
- [execution_commands.md](./execution_commands.md)
- [explanation.md](./explanation.md)
- [outputs.md](./outputs.md)
