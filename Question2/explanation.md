# Conceptual Explanation — Question 2

## 1. Process Creation with `fork()`

### What is `fork()`?
`fork()` is a Linux system call that creates a **new process** by duplicating the calling process. The new process is called the **child**; the caller is the **parent**.

### How does it work?
```
         fork()
          │
Parent ───┤──────────────────────► Parent continues (fork returns child PID)
          │
          └──► Child is born (fork returns 0 in child, child gets copy of parent's memory)
```

### Return Values:
| Return Value | In Which Process | Meaning |
|-------------|-----------------|---------|
| `> 0` (child PID) | Parent | Fork succeeded; this is the child's PID |
| `== 0` | Child | This is the child process |
| `== -1` | Parent only | Fork failed (e.g., too many processes) |

### Code Pattern Used:
```c
pid_t pid = fork();
if (pid < 0)      { /* error */ }
else if (pid == 0) { /* child code */ }
else               { /* parent code */ }
```

---

## 2. Parent-Child Relationship

After `fork()`:
- The child is an **exact copy** of the parent's virtual address space
- Both processes continue executing from the line **after** `fork()`
- They differ only in the return value of `fork()`
- Changes in child memory do **NOT** affect the parent (Copy-on-Write)

### Parent's Responsibilities:
1. Store the child's PID for monitoring
2. Monitor child health (timeout)
3. Reap the child with `waitpid()` to prevent zombies

---

## 3. Zombie Processes

### What is a Zombie?
When a child exits, it doesn't disappear immediately. It becomes a **zombie** — it retains its entry in the process table with exit status, waiting for the parent to `wait()` for it.

```
Child exits ──► enters Z (zombie) state
                  ↓
Parent calls wait() ──► zombie is reaped (removed from process table)
```

### Why are Zombies Dangerous?
- Each zombie consumes a **process table entry**
- The process table has a fixed size (typically 32,768 entries on Linux)
- If zombies accumulate, the system cannot create new processes
- This is how a web server becomes unresponsive

### Prevention in This Program:
```c
/* Install SIGCHLD handler — called whenever a child changes state */
sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
sigaction(SIGCHLD, &sa_chld, NULL);

/* Handler reaps children with non-blocking waitpid */
while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    children_reaped++;
}
```

---

## 4. Orphan Processes

### What is an Orphan?
If a **parent exits before its child**, the child becomes an **orphan**. Linux automatically **re-parents** orphans to PID 1 (`init`/`systemd`), which periodically calls `wait()` to reap them.

### In This Program:
We prevent orphans by ensuring the parent **outlives** all children and waits for them explicitly.

---

## 5. Waiting — `wait()` vs `waitpid()`

### `wait(int *status)`:
- Blocks until **any** child exits
- Cannot choose which child to wait for
- Cannot check without blocking

### `waitpid(pid_t pid, int *status, int options)`:
| Parameter | Value | Meaning |
|-----------|-------|---------|
| `pid` | `> 0` | Wait for specific child |
| `pid` | `-1` | Wait for any child |
| `options` | `WNOHANG` | Return immediately if no child exited |
| `options` | `0` | Block until specified child exits |

### Why `waitpid()` is Better:
- More **flexible** — can target specific PIDs
- Supports **non-blocking** mode (`WNOHANG`)
- Can detect **stopped** processes too

### Status Macros:
```c
WIFEXITED(status)   → true if child called exit()
WEXITSTATUS(status) → exit code of child
WIFSIGNALED(status) → true if child killed by signal
WTERMSIG(status)    → signal number that killed child
```

---

## 6. Signal Handling

### `SIGCHLD`:
Sent to parent when child **exits, stops, or continues**. Used to reap zombies asynchronously without blocking the parent.

### `SIGTERM` (Signal 15):
- "Please terminate gracefully"
- Can be caught, blocked, or ignored by the process
- Allows the process to clean up before dying

### `SIGKILL` (Signal 9):
- "Die immediately" — cannot be caught, blocked, or ignored
- The kernel forcefully destroys the process
- No cleanup happens — use as last resort only

### `SIGALRM`:
- Sent by the kernel after `alarm(n)` — fires after `n` seconds
- Used to implement timeouts inside child processes

### `sigaction()` vs `signal()`:

| Feature | `signal()` | `sigaction()` |
|---------|-----------|--------------|
| POSIX standard | Yes | Yes (preferred) |
| Handler auto-reset | System-dependent | Controllable via `sa_flags` |
| Signal mask during handler | Undefined | Explicit via `sa_mask` |
| Restart interrupted calls | System-dependent | `SA_RESTART` flag |
| Recommended for production | No | **Yes** |

---

## 7. How Everything Works Together

```
Parent                          Child-N
  │                                │
  │── fork() ──────────────────────►│  Child born
  │   stores child PID             │
  │                                │── alarm(CHILD_TIMEOUT_SEC)
  │                                │── sleep(work_duration)
  │                                │  (if unresponsive: 30s sleep)
  │                                │
  │  [SIGCHLD received]            │
  │── sigchld_handler()            │
  │   waitpid(-1, WNOHANG)         │
  │                                │
  │── monitor: timeout?            │
  │   yes → kill(SIGTERM)          │── receives SIGTERM
  │   wait 2s                      │  (may or may not exit)
  │   still alive?                 │
  │   yes → kill(SIGKILL)          │── forcefully destroyed
  │   waitpid(pid, 0)              │
  │── child reaped, no zombie      │
  │                                ○ (child gone)
  │── final waitpid loop           
  │── EXIT_SUCCESS                 
```

This cooperative design ensures **no zombies**, **no orphans**, and **no runaway processes**.
