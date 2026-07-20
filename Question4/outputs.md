# Sample Outputs — Question 4

> Realistic terminal outputs from all monitoring commands.

---

## Output 1: `cat sample.log` (first 15 lines)

```
[2026-07-20 20:00:01] INFO: Web server started on port 8080
[2026-07-20 20:00:02] INFO: Database connection established
[2026-07-20 20:00:05] INFO: Worker thread pool initialized (4 threads)
[2026-07-20 20:01:12] INFO: GET /api/users — 200 OK (45ms)
[2026-07-20 20:01:15] INFO: GET /api/products — 200 OK (32ms)
[2026-07-20 20:01:22] WARNING: Slow query detected (query took 1200ms)
[2026-07-20 20:01:30] INFO: POST /api/orders — 201 Created (128ms)
[2026-07-20 20:02:01] ERROR: Database connection timed out after 30 seconds
[2026-07-20 20:02:02] INFO: Attempting database reconnect...
[2026-07-20 20:02:03] INFO: Database reconnect successful
...
```

---

## Output 2: `wc -l sample.log`

```
52 sample.log
```

---

## Output 3: `grep -c "ERROR" sample.log`

```
10
```

---

## Output 4: `grep "ERROR" sample.log`

```
[2026-07-20 20:02:01] ERROR: Database connection timed out after 30 seconds
[2026-07-20 20:04:00] ERROR: Failed to write to disk — No space left on device
[2026-07-20 20:04:01] ERROR: Transaction rolled back due to disk write failure
[2026-07-20 20:06:00] ERROR: SSL certificate expires in 7 days — renewal required
[2026-07-20 20:07:30] ERROR: Authentication service unavailable
[2026-07-20 20:09:15] ERROR: Worker thread #3 crashed unexpectedly
[2026-07-20 20:12:00] ERROR: Network interface eth1 went down
[2026-07-20 20:12:01] ERROR: Failover to eth0 — 3 connections dropped
[2026-07-20 20:15:30] ERROR: Rate limit exceeded — 52 requests rejected
[2026-07-20 20:18:00] ERROR: Upstream service timeout — /api/payment returned 504
```

---

## Output 5: Generated Error Report (`cat error_report.log`)

```
============================================================
        ERROR REPORT — Sun Jul 20 21:30:00 IST 2026
============================================================
[2026-07-20 20:02:01] ERROR: Database connection timed out after 30 seconds
[2026-07-20 20:04:00] ERROR: Failed to write to disk — No space left on device
[2026-07-20 20:04:01] ERROR: Transaction rolled back due to disk write failure
[2026-07-20 20:06:00] ERROR: SSL certificate expires in 7 days — renewal required
[2026-07-20 20:07:30] ERROR: Authentication service unavailable
[2026-07-20 20:09:15] ERROR: Worker thread #3 crashed unexpectedly
[2026-07-20 20:12:00] ERROR: Network interface eth1 went down
[2026-07-20 20:12:01] ERROR: Failover to eth0 — 3 connections dropped
[2026-07-20 20:15:30] ERROR: Rate limit exceeded — 52 requests rejected
[2026-07-20 20:18:00] ERROR: Upstream service timeout — /api/payment returned 504
============================================================
Total ERROR entries: 10
============================================================
```

---

## Output 6: Real-Time Monitoring (`tail -f sample.log | grep --line-buffered "ERROR"`)

```
(terminal waits here — following the file)

[2026-07-20 21:30:45] ERROR: Disk I/O failure on /dev/sdb
[2026-07-20 21:31:02] ERROR: Connection pool exhausted — 12 requests queued
```

> The two lines above appear instantly when injected via Terminal 2:
> ```bash
> echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Disk I/O failure on /dev/sdb" >> sample.log
> echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Connection pool exhausted — 12 requests queued" >> sample.log
> ```

---

## Output 7: Background Monitoring Job

```
[1] 14900
Monitoring started in background (PID: 14900)
```

---

## Output 8: Stopping Background Job (`jobs` + `kill %1`)

```
[1]+  Running    tail -f sample.log | grep --line-buffered "ERROR" >> error_report.log 2>/dev/null

(after kill %1)

[1]+  Terminated  tail -f sample.log | grep --line-buffered "ERROR" >> error_report.log 2>/dev/null
```

---

## Output 9: Context Lines (`grep -B1 -A1 "ERROR" sample.log | head -15`)

```
[2026-07-20 20:01:30] INFO: POST /api/orders — 201 Created (128ms)
[2026-07-20 20:02:01] ERROR: Database connection timed out after 30 seconds
[2026-07-20 20:02:02] INFO: Attempting database reconnect...
--
[2026-07-20 20:03:30] INFO: Cache flush completed successfully
[2026-07-20 20:04:00] ERROR: Failed to write to disk — No space left on device
[2026-07-20 20:04:01] ERROR: Transaction rolled back due to disk write failure
[2026-07-20 20:04:02] INFO: Administrator alerted via email
```
