# Execution Commands — Question 4

> All commands are run from inside the `Question4/` directory.

---

## Step 1: Navigate to Question4 Directory

```bash
cd Question4
```

**Explanation:**  
Changes the current working directory to `Question4/` where `sample.log` and monitoring commands are located.

**Observation:**  
The terminal prompt updates to show `Question4`, and all subsequent commands reference files in this directory.

---

## Step 2: Inspect the Sample Log File

```bash
cat sample.log
```

**Explanation:**  
`cat` outputs the entire contents of `sample.log`. This gives an overview of the log format: each entry has a timestamp, severity level (INFO/WARNING/ERROR), and a message.

**Observation:**  
50 log entries are displayed with a mix of INFO, WARNING, and ERROR levels. This confirms the log file is present and properly formatted.

---

## Step 3: Count Total Log Entries

```bash
wc -l sample.log
```

**Explanation:**  
`wc -l` counts the number of lines in the file. Each line = one log entry. This gives the baseline count before filtering.

**Observation:**  
Output shows `50 sample.log`, confirming there are 50 log entries total in the file.

---

## Step 4: Count ERROR Entries

```bash
grep -c "ERROR" sample.log
```

**Explanation:**  
`grep -c` counts matching lines instead of printing them. This quickly reports how many ERROR entries exist in the log without displaying them all.

**Observation:**  
Output: `10` — there are 10 ERROR entries in the 50-line log file.

---

## Step 5: Extract All ERROR Lines

```bash
grep "ERROR" sample.log
```

**Explanation:**  
`grep "ERROR"` searches each line and prints only those containing the string "ERROR". It reads the file once and exits, unlike `tail -f`.

**Observation:**  
10 ERROR lines are printed to the terminal, demonstrating that grep can filter a log file for specific severity levels.

---

## Step 6: Save Error Report Using Redirection

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
The `{ ... } > file` construct redirects all output from the group to `error_report.log`. `$(date)` inserts the current timestamp. This creates a formatted report without opening an editor.

**Observation:**  
`error_report.log` is created with a professional header, all 10 ERROR lines, and a total count — ready for administrator review.

---

## Step 7: View the Generated Error Report

```bash
cat error_report.log
```

**Explanation:**  
Displays the contents of `error_report.log` to verify that all 10 ERROR entries were captured correctly with proper formatting.

**Observation:**  
The report shows the header with date, all 10 ERROR lines, and the total count line — confirming the pipeline and redirection worked correctly.

---

## Step 8: Monitor Log File in Real-Time (Static Demo)

```bash
tail -10 sample.log
```

**Explanation:**  
`tail -10` shows the last 10 lines of `sample.log`. This is a static (non-following) version useful for seeing recent entries without continuous monitoring.

**Observation:**  
The last 10 log entries are displayed, showing INFO entries near the end of the file.

---

## Step 9: Simulate Real-Time Monitoring (Append to Log)

```bash
# Terminal 1: Start monitoring
tail -f sample.log | grep --line-buffered "ERROR"

# Terminal 2: Add new log entries
echo "[$(date '+%Y-%m-%d %H:%M:%S')] INFO: System check complete" >> sample.log
echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Disk I/O failure on /dev/sdb" >> sample.log
echo "[$(date '+%Y-%m-%d %H:%M:%S')] INFO: Recovery initiated" >> sample.log
```

**Explanation:**  
`tail -f` follows the file in real time; new lines appended in Terminal 2 immediately appear in Terminal 1 (if they match grep's filter). The INFO line is silently dropped.

**Observation:**  
Only the ERROR line appears in Terminal 1. The two INFO lines are filtered out by grep, demonstrating selective real-time monitoring.

---

## Step 10: Suppress Error Messages with /dev/null

```bash
tail -f sample.log | grep --line-buffered "ERROR" >> error_report.log 2>/dev/null &
echo "Monitoring started in background (PID: $!)"
```

**Explanation:**  
`2>/dev/null` redirects stderr to `/dev/null` (the null device — discards all input). `&` runs the pipeline in background. `$!` is the PID of the last background command.

**Observation:**  
The terminal returns immediately with `Monitoring started in background (PID: 14900)`. ERROR lines are silently appended to `error_report.log` in the background.

---

## Step 11: Stop Background Monitoring

```bash
jobs
kill %1
```

**Explanation:**  
`jobs` lists all background jobs with their job numbers. `kill %1` sends SIGTERM to job #1 (the background pipeline), stopping the monitoring gracefully.

**Observation:**  
The background tail-grep pipeline is terminated. The terminal shows `[1] Terminated tail -f sample.log | grep ...`.

---

## Step 12: Verify Final Error Report

```bash
wc -l error_report.log
cat error_report.log
```

**Explanation:**  
`wc -l` counts the total lines in the error report (including headers). `cat` displays the complete report to verify all errors were captured.

**Observation:**  
The report contains exactly the ERROR lines from `sample.log`, confirming the pipeline correctly filtered and persisted all error events.
