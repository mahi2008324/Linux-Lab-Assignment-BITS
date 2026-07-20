# Sample Outputs — Question 2

> This file shows realistic terminal output from compiling and running `process_monitor`.

---

## Output 1: GCC Version Check (`gcc --version`)

```
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

---

## Output 2: Compilation (`make`)

```
gcc -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L -g process_monitor.c -o process_monitor

==============================================
  Build successful! Run with: ./process_monitor
==============================================
```

---

## Output 3: Executable Verification (`ls -lh process_monitor`)

```
-rwxr-xr-x 1 student student 22K Jul 20 21:10 process_monitor
```

---

## Output 4: File Type Check (`file process_monitor`)

```
process_monitor: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
BuildID[sha1]=3a1d7c2f9b4e8a0d5c6f2b3e4d7a8f1c9b2e5d0,
for GNU/Linux 3.2.0, with debug_info, not stripped
```

---

## Output 5: Running the Program (`./process_monitor`)

```

------------------------------------------------------------
      WEB SERVER PROCESS MONITOR — Linux Lab Assignment Q2
------------------------------------------------------------
[PARENT] Process Monitor started. PID = 14523
[PARENT] Will create 5 worker processes.
[PARENT] Child timeout set to 8 seconds.
[PARENT] Child index 2 will simulate an unresponsive process.
------------------------------------------------------------
[PARENT] Signal handlers installed (SIGCHLD, SIGALRM).

[PARENT] Forking 5 child process(es)...

[PARENT] Spawned Child-0 with PID=14524
[CHILD-0] PID=14524 | Parent PID=14523 | Starting work...
[CHILD-0] PID=14524 | Working for 3 second(s)...
[PARENT] Spawned Child-1 with PID=14525
[CHILD-1] PID=14525 | Parent PID=14523 | Starting work...
[CHILD-1] PID=14525 | Working for 1 second(s)...
[PARENT] Spawned Child-2 with PID=14526
[CHILD-2] PID=14526 | Parent PID=14523 | Starting work...
[CHILD-2] PID=14526 | *** SIMULATING UNRESPONSIVE PROCESS *** (sleeping 30 seconds)
[PARENT] Spawned Child-3 with PID=14527
[CHILD-3] PID=14527 | Parent PID=14523 | Starting work...
[CHILD-3] PID=14527 | Working for 2 second(s)...
[PARENT] Spawned Child-4 with PID=14528
[CHILD-4] PID=14528 | Parent PID=14523 | Starting work...
[CHILD-4] PID=14528 | Working for 4 second(s)...

[PARENT] All children spawned. Beginning monitoring phase.

[PARENT] Monitoring 5 child process(es)...
------------------------------------------------------------
[PARENT] Monitoring Child-0 (PID=14524)...
[CHILD-1] PID=14525 | Work complete. Exiting normally.
[SIGCHLD] Child process reaped.
[CHILD-0] PID=14524 | Work complete. Exiting normally.
[PARENT] Child-0 (PID=14524) exited NORMALLY (code=0).
[PARENT] Monitoring Child-1 (PID=14525)...
[PARENT] Child-1 (PID=14525) was already reaped.
[PARENT] Monitoring Child-2 (PID=14526)...
[CHILD-3] PID=14527 | Work complete. Exiting normally.
[SIGCHLD] Child process reaped.
[CHILD-4] PID=14528 | Work complete. Exiting normally.
[SIGCHLD] Child process reaped.
[PARENT] Child-2 (PID=14526) is unresponsive. Sending SIGTERM...
[PARENT] Waiting 2 second(s) for Child-2 to respond to SIGTERM...
[PARENT] Child-2 (PID=14526) did not respond to SIGTERM. Sending SIGKILL...
[PARENT] Child-2 (PID=14526) forcefully killed with SIGKILL.
[SIGCHLD] Child process reaped.
[PARENT] Child-2 (PID=14526) was killed by signal 9 (SIGKILL).
[PARENT] Monitoring Child-3 (PID=14527)...
[PARENT] Child-3 (PID=14527) was already reaped.
[PARENT] Monitoring Child-4 (PID=14528)...
[PARENT] Child-4 (PID=14528) was already reaped.

[PARENT] Performing final cleanup (waiting for remaining children)...

------------------------------------------------------------
[PARENT] All child processes have been handled.
[PARENT] Total children reaped by SIGCHLD handler: 4
[PARENT] Process monitor exiting cleanly.
------------------------------------------------------------
```

---

## Output 6: No Zombie Processes (`ps aux | grep 'Z'`)

```
student  14600  0.0  0.0  13144   980 pts/1    S+   21:12   0:00 grep --color=auto Z
```

> ✅ Only the `grep` command itself appears — no zombie (`Z`) processes remain.

---

## Output 7: Process Tree During Execution (`ps -eo pid,ppid,stat,comm`)

```
  PID  PPID STAT COMMAND
14523  1402 S+   process_monit
14524 14523 S    process_monit
14525 14523 S    process_monit
14526 14523 S    process_monit   ← This child will be killed (unresponsive)
14527 14523 S    process_monit
14528 14523 S    process_monit
```

---

## Output 8: Make Clean

```
Cleaning build artifacts...
rm -f process_monitor *.o core
Done.
```
