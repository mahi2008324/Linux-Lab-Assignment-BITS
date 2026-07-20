# Execution Commands — Question 2

> All commands are run from inside the `Question2/` directory.  
> Each command is followed by an **Explanation** and **Observation**.

---

## Step 1: Navigate to Question2 Directory

```bash
cd Question2
```

**Explanation:**  
Changes the working directory to `Question2/` where all C source files and the Makefile reside. All subsequent commands are relative to this directory.

**Observation:**  
The terminal prompt updates to show `Question2`, confirming the directory change was successful.

---

## Step 2: Inspect the Source File

```bash
cat -n process_monitor.c | head -60
```

**Explanation:**  
`cat -n` displays the source file with line numbers. `head -60` limits the output to the first 60 lines. This allows us to review the file header, includes, and constants before compiling.

**Observation:**  
The header shows the file description, compile options, and required includes, confirming the source file is present and properly structured.

---

## Step 3: Check GCC Version

```bash
gcc --version
```

**Explanation:**  
Displays the installed GCC compiler version. This confirms that GCC is available and checks that it supports C11 (requires GCC 4.7+).

**Observation:**  
Output shows something like `gcc (Ubuntu 11.4.0) 11.4.0`, confirming GCC 11 is installed which fully supports C11 and POSIX.1-2008.

---

## Step 4: Compile Using GCC Directly

```bash
gcc -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L process_monitor.c -o process_monitor
```

**Explanation:**  
- `gcc`: The GNU C Compiler
- `-Wall -Wextra`: Enable all standard + extra warnings to catch potential bugs
- `-std=c11`: Compile using the C11 language standard
- `-D_POSIX_C_SOURCE=200809L`: Define the POSIX feature-test macro to enable POSIX.1-2008 APIs like `sigaction()`
- `-o process_monitor`: Name the output executable `process_monitor`

**Observation:**  
No warnings or errors are printed, confirming the code compiles cleanly. The `process_monitor` executable is created in the current directory.

---

## Step 5: Build Using Make (Recommended)

```bash
make
```

**Explanation:**  
`make` reads the `Makefile` and executes the default target (`all`), which runs the same GCC compilation command with all flags pre-configured. Make automatically skips recompilation if the source hasn't changed.

**Observation:**  
The build output shows the GCC command being executed, followed by the success message `Build successful! Run with: ./process_monitor`.

---

## Step 6: Verify the Executable

```bash
ls -lh process_monitor
```

**Explanation:**  
`ls -lh` lists the compiled executable with human-readable file size. The `x` bits in the permissions confirm it is executable.

**Observation:**  
Output shows `-rwxr-xr-x 1 student staff 18K Jul 20 21:10 process_monitor`, confirming the executable was created with proper permissions.

---

## Step 7: Check the Executable Type

```bash
file process_monitor
```

**Explanation:**  
The `file` command identifies the type of a file by examining its content. For compiled C programs, it shows the architecture, linking type, and OS.

**Observation:**  
Output: `process_monitor: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, not stripped` — confirms a valid 64-bit Linux executable.

---

## Step 8: Run the Process Monitor

```bash
./process_monitor
```

**Explanation:**  
Executes the compiled program. The parent process spawns 5 children using `fork()`. Four complete normally, while child-2 simulates being unresponsive and is terminated by the parent using SIGTERM/SIGKILL.

**Observation:**  
The terminal shows PIDs being assigned, children working, the unresponsive child being detected and killed, and a final report confirming no zombie processes remain.

---

## Step 9: Monitor Running Processes (in another terminal)

```bash
ps -eo pid,ppid,stat,comm | grep process_monitor
```

**Explanation:**  
`ps -eo` displays processes with custom columns: PID, Parent PID, status, and command name. Running this while `process_monitor` is executing lets us observe the parent-child relationships in real time.

**Observation:**  
Shows the parent (stat=S, sleeping) and all child processes (stat=S or R). After children exit, they briefly appear as `Z` (zombie) state before the SIGCHLD handler reaps them.

---

## Step 10: Check for Zombie Processes

```bash
ps aux | grep 'Z'
```

**Explanation:**  
`ps aux` shows all processes with full details. Piping to `grep 'Z'` filters for processes in zombie state (stat=Z). After the monitor completes, there should be no zombies.

**Observation:**  
After the program finishes, no zombie entries remain — confirming that the SIGCHLD handler and final `waitpid()` loop successfully reaped all child processes.

---

## Step 11: Run with strace (System Call Tracing)

```bash
strace -e trace=fork,clone,wait4,kill ./process_monitor 2>&1 | head -40
```

**Explanation:**  
`strace` traces system calls made by the program. We filter for `fork`, `clone`, `wait4`, and `kill` to observe exactly how processes are created and reaped at the kernel level. `2>&1` merges stderr into stdout for display.

**Observation:**  
Output shows exact `fork()` return values (child PIDs), `waitpid()` calls, and `kill()` signals being sent, confirming the program's behaviour matches the expected design.

---

## Step 12: Clean Build Artifacts

```bash
make clean
```

**Explanation:**  
Runs the `clean` target from the Makefile, which executes `rm -f process_monitor *.o core` to delete the executable and any object/core files. This leaves only the source code and Makefile.

**Observation:**  
The `process_monitor` executable is removed. `ls` confirms only `process_monitor.c`, `Makefile`, and documentation files remain.
