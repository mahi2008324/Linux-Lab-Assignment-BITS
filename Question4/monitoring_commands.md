# Monitoring Commands — Question 4

> Complete set of command pipelines for real-time log monitoring.  
> All commands operate on `sample.log` in the `Question4/` directory.

---

## Pipeline 1: Basic Real-Time Log Monitoring

```bash
tail -f sample.log
```

**Explanation:**  
`tail -f` (follow) continuously reads new lines appended to `sample.log` and prints them to the terminal. It does NOT exit — it waits indefinitely for new content.

**Observation:**  
The terminal displays all current log content and then pauses, waiting for new lines. Any line added to `sample.log` appears instantly in the terminal.

---

## Pipeline 2: Monitor and Filter Only ERROR Messages

```bash
tail -f sample.log | grep "ERROR"
```

**Explanation:**  
Pipes the output of `tail -f` into `grep "ERROR"`, which filters and displays only lines containing the word "ERROR". The pipe (`|`) connects stdout of `tail` to stdin of `grep`.

**Observation:**  
Only lines with "ERROR" in their content are displayed. INFO and WARNING lines are silently discarded by `grep`.

---

## Pipeline 3: Line-Buffered Grep (Recommended for Pipelines)

```bash
tail -f sample.log | grep --line-buffered "ERROR"
```

**Explanation:**  
The `--line-buffered` flag forces grep to flush its output buffer after every line it processes. Without this, grep may batch-buffer output, causing delays in pipelines connected to tail.

**Observation:**  
ERROR lines appear immediately as they are added — no buffering delay. This is critical for real-time monitoring.

---

## Pipeline 4: Monitor + Save Errors to Report File

```bash
tail -f sample.log | grep --line-buffered "ERROR" | tee -a error_report.log
```

**Explanation:**  
`tee -a error_report.log` reads from stdin and simultaneously writes to stdout AND appends to `error_report.log`. `-a` (append) ensures existing entries are not overwritten.

**Observation:**  
ERROR lines appear in the terminal AND are written to `error_report.log` simultaneously. The report file accumulates all errors over time.

---

## Pipeline 5: Suppress Normal Output (Silent Background Monitoring)

```bash
tail -f sample.log | grep --line-buffered "ERROR" >> error_report.log 2>/dev/null &
```

**Explanation:**  
- `>> error_report.log`: Appends ERROR lines to the report file
- `2>/dev/null`: Sends any errors (e.g., broken pipe on exit) to `/dev/null` (discarded)
- `&`: Runs the entire pipeline in the background, returning the terminal prompt immediately

**Observation:**  
The terminal returns immediately with a job number (e.g., `[1] 14800`). Monitoring continues silently in the background.

---

## Pipeline 6: Monitor with Timestamp in Errors

```bash
tail -f sample.log | grep --line-buffered "ERROR" | \
    awk '{ print strftime("[Detected: %Y-%m-%d %H:%M:%S]"), $0 }' >> error_report.log
```

**Explanation:**  
Adds a "detection timestamp" using `awk`'s `strftime()` function to each ERROR line before saving it. This shows when the monitoring system detected the error, which may differ from when the error was logged.

**Observation:**  
Each ERROR line in `error_report.log` now has two timestamps: the original log timestamp and the detection timestamp, enabling latency analysis.

---

## Pipeline 7: Extract Errors from Existing Log File

```bash
grep "ERROR" sample.log
```

**Explanation:**  
Unlike `tail -f`, this processes the file from start to finish and exits. It extracts all ERROR lines already present in the log file without monitoring for new ones.

**Observation:**  
10 ERROR lines from `sample.log` are printed, then the command exits immediately.

---

## Pipeline 8: Count Errors by Type

```bash
grep "ERROR" sample.log | wc -l
```

**Explanation:**  
Pipes grep output to `wc -l` (word count, lines) to count the total number of ERROR entries. Useful for a quick error count without reading the full file.

**Observation:**  
Output: `10` — confirming there are 10 ERROR entries in `sample.log`.

---

## Pipeline 9: Extract Errors with Context Lines

```bash
grep -B 1 -A 1 "ERROR" sample.log
```

**Explanation:**  
`-B 1` includes 1 line **before** each match, and `-A 1` includes 1 line **after**. This provides context around each error — useful for understanding what led to the error.

**Observation:**  
Each ERROR line is shown with one surrounding line above and below, providing immediate context for diagnosis.

---

## Pipeline 10: Generate Summary Report

```bash
{
    echo "============================================================"
    echo "        ERROR REPORT — $(date)"
    echo "============================================================"
    grep "ERROR" sample.log
    echo "============================================================"
    echo "Total ERROR entries: $(grep -c "ERROR" sample.log)"
    echo "============================================================"
} > error_report.log
```

**Explanation:**  
Uses a bash group command `{ ... }` with `>` redirection to create a formatted report. All echo and grep output is written to `error_report.log` in one operation.

**Observation:**  
`error_report.log` contains a professional report with header, all ERROR lines, and total count — ready for administrator review.

---

## Pipeline 11: Case-Insensitive Error Detection

```bash
grep -i "error\|warn\|critical\|fatal" sample.log | tee error_report.log
```

**Explanation:**  
`-i` makes grep case-insensitive. `\|` (escaped pipe) is grep's "OR" operator — matches lines containing any of: "error", "warn", "critical", or "fatal". `tee` shows output and saves it.

**Observation:**  
More entries are matched including WARNING lines, providing a more comprehensive incident report covering multiple severity levels.

---

## Pipeline 12: Real-Time Monitoring with Log Injection

Open **Terminal 1** (monitoring):
```bash
tail -f sample.log | grep --line-buffered "ERROR" | tee -a error_report.log
```

Open **Terminal 2** (inject new log entries):
```bash
echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: New error injected for testing" >> sample.log
echo "[$(date '+%Y-%m-%d %H:%M:%S')] INFO: Normal operation resumed" >> sample.log
echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Second test error — disk I/O failure" >> sample.log
```

**Explanation:**  
Terminal 1 runs the monitoring pipeline continuously. Terminal 2 appends new lines to `sample.log`. When ERROR lines are injected, they immediately appear in Terminal 1 and in `error_report.log`.

**Observation:**  
Only the two ERROR lines appear in Terminal 1 — the INFO line is filtered out by grep. Both ERROR lines are also written to `error_report.log`.
