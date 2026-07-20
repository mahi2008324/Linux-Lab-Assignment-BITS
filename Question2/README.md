# Question 2 — C Program: Process Monitoring & Zombie Prevention

## 📌 Problem Statement

A web server occasionally becomes unresponsive due to excessive child processes. Design a C program that:

1. Creates child processes using `fork()`
2. Monitors their execution
3. Prevents zombie processes using proper `wait()`/`waitpid()` and signal handling
4. Terminates unresponsive child processes using appropriate signals

---

## 🛠️ Files in this Folder

| File | Description |
|------|-------------|
| `process_monitor.c` | Main C source file |
| `Makefile` | Build automation file |
| `execution_commands.md` | All compilation & execution commands |
| `outputs.md` | Sample program outputs |
| `explanation.md` | Conceptual explanation of processes & signals |
| `screenshots/` | Screenshots of execution |

---

## 🚀 How to Build and Run

```bash
# Option 1: Using Makefile (recommended)
make
./process_monitor

# Option 2: Using GCC directly
gcc -Wall -Wextra -std=c11 process_monitor.c -o process_monitor
./process_monitor

# Clean build files
make clean
```

---

## 📋 What the Program Does

1. **Creates 5 child processes** using `fork()`
2. **Each child** simulates work by sleeping for a random duration (1–10 seconds)
3. **One child** is designated as "unresponsive" (sleeps for 30 seconds)
4. **The parent** sets up `SIGCHLD` handler to reap zombies automatically
5. **Uses `alarm()`** to set a timeout per child
6. **Sends `SIGTERM` first**, waits 2 seconds, then sends `SIGKILL` if still running
7. **Waits for all children** with `waitpid()` to ensure no zombies remain
8. **Reports exit status** of each child

---

## 🔑 Key Concepts Demonstrated

| Concept | System Call/Signal |
|---------|-------------------|
| Process creation | `fork()` |
| Zombie prevention | `SIGCHLD` + `waitpid()` |
| Graceful termination | `SIGTERM` |
| Forced termination | `SIGKILL` |
| Signal handling | `sigaction()` |
| Timeout enforcement | `alarm()` + `SIGALRM` |
| Status checking | `WIFEXITED()`, `WIFSIGNALED()` |

---

## 🔗 Related Files

- [process_monitor.c](./process_monitor.c)
- [Makefile](./Makefile)
- [execution_commands.md](./execution_commands.md)
- [explanation.md](./explanation.md)
