# Screenshots — Question 4

This folder contains screenshots from running the real-time log monitoring pipeline.

## Required Screenshots

| Filename | Description | When to Take |
|----------|-------------|--------------|
| `Screenshot-01.png` | `cat sample.log` — full log contents | Before running commands |
| `Screenshot-02.png` | `grep "ERROR" sample.log` output | After running grep |
| `Screenshot-03.png` | Two terminals: monitoring + injection | While tail -f is running |
| `Screenshot-04.png` | `cat error_report.log` — final report | After generating report |
| `Screenshot-05.png` | Background monitoring PID | After starting with `&` |

## Two-Terminal Setup

For best screenshots, set up side-by-side terminals:
- **Left terminal**: Run `tail -f sample.log | grep --line-buffered "ERROR"`
- **Right terminal**: Run `echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Test error" >> sample.log`

Capture the moment the ERROR appears in the left terminal.
