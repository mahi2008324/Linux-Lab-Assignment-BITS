# 🐧 Linux Lab Assignment — Modules 5–10

> **Course**: Operating Systems & Linux Systems Programming  
> **Assignment Type**: Graded Lab Assignment  
> **Total Marks**: 20 (4 marks per question × 5 questions)  
> **Repository Visibility**: Public ✅

---

## 📋 Project Overview

This repository contains a complete, professional solution to a 5-question Linux Systems Programming Lab Assignment covering:

- Shell scripting and file management
- Process creation, monitoring, and signal handling in C
- Linux system calls for file I/O
- Real-time log monitoring with command pipelines
- `vi` editor recovery mechanisms

All solutions follow **Linux best practices**, include **proper error handling**, provide **realistic sample outputs**, and contain **command-by-command explanations** as required by the grading rubric.

---

## 🎯 Assignment Objectives

| # | Topic | Marks |
|---|-------|-------|
| Q1 | Shell Script: Duplicate Detection & Backup | 4 |
| Q2 | C Program: Process Monitoring & Zombie Prevention | 4 |
| Q3 | C Program: File I/O using Linux System Calls | 4 |
| Q4 | Command Pipeline: Real-Time Log Monitoring | 4 |
| Q5 | Report: `vi` Recovery Mechanisms Evaluation | 4 |
| **Total** | | **20** |

---

## 📁 Repository Structure

```
Linux-Lab-Assignment/
│
├── README.md                          ← This file (project overview)
│
├── Question1/                         ← Shell Script: Duplicate Detection & Backup
│   ├── README.md                      ← Question overview and how to run
│   ├── duplicate_backup.sh            ← Main shell script (POSIX-compatible)
│   ├── execution_commands.md          ← Step-by-step command guide with explanations
│   ├── explanation.md                 ← Conceptual explanation of approach
│   ├── outputs.md                     ← Sample terminal outputs
│   ├── errors.log                     ← Error messages from script run
│   ├── sample_submissions/            ← 10 test student files (3 are duplicates)
│   ├── backup/                        ← 7 unique files backed up by script
│   ├── reports/                       ← Auto-generated timestamped backup reports
│   └── screenshots/
│       ├── README.md                  ← Screenshot descriptions
│       ├── Screenshot-01-script-execution.png
│       └── Screenshot-02-backup-report.png
│
├── Question2/                         ← C Program: Process Monitoring
│   ├── README.md
│   ├── process_monitor.c              ← C source (fork/wait/signal handling)
│   ├── Makefile                       ← Build with `make`
│   ├── execution_commands.md
│   ├── explanation.md
│   ├── outputs.md
│   └── screenshots/
│       ├── README.md
│       ├── Screenshot-01-compilation-and-spawn.png
│       └── Screenshot-02-sigterm-sigkill.png
│
├── Question3/                         ← C Program: System Calls File I/O
│   ├── README.md
│   ├── employee_records.c             ← C source (open/read/write/lseek/close only)
│   ├── employees.dat                  ← Generated binary database (480 bytes)
│   ├── employees_dat_note.txt         ← Explanation of binary format
│   ├── execution_commands.md
│   ├── explanation.md
│   ├── outputs.md
│   └── screenshots/
│       ├── README.md
│       ├── Screenshot-01-write-read-records.png
│       └── Screenshot-02-update-hexdump.png
│
├── Question4/                         ← Command Pipeline: Real-Time Log Monitoring
│   ├── README.md
│   ├── sample.log                     ← 52-line log file (10 ERRORs, 6 WARNINGs, 36 INFOs)
│   ├── error_report.log               ← Pre-generated ERROR-only report
│   ├── monitoring_commands.md         ← All pipeline commands explained
│   ├── execution_commands.md
│   ├── explanation.md
│   ├── outputs.md
│   └── screenshots/
│       ├── README.md
│       ├── Screenshot-01-grep-filter-errors.png
│       └── Screenshot-02-tail-pipe-realtime-monitoring.png
│
└── Question5/                         ← Report: vi Recovery Mechanisms
    ├── README.md
    ├── vi_recovery.md                 ← Full evaluation report (5 mechanisms)
    ├── execution_commands.md
    ├── explanation.md
    ├── outputs.md
    └── screenshots/
        ├── README.md
        ├── Screenshot-01-swap-file-created.png
        ├── Screenshot-02-E325-recovery-prompt.png
        ├── Screenshot-03-vim-r-recovery-complete.png
        └── Screenshot-04-registers-and-undofile.png
```

---

## ⚙️ System Requirements

| Requirement | Details |
|-------------|---------|
| **Operating System** | Linux (Ubuntu 20.04+ / Debian / CentOS 7+) |
| **Compiler** | GCC 9.0 or higher |
| **Shell** | Bash 3.2+ (compatible with macOS default shell too) |
| **Tools** | `md5sum` (or `md5` on macOS), `find`, `tail`, `grep`, `awk`, `vim` |
| **Standards** | C11, POSIX.1-2008 (`-D_POSIX_C_SOURCE=200809L`) |

---

## 🚀 How to Run Each Question

### Question 1 — Shell Script
```bash
cd Question1
chmod +x duplicate_backup.sh
./duplicate_backup.sh
# Output: backup/ folder + reports/backup_report_*.txt
```

### Question 2 — Process Monitor (C)
```bash
cd Question2
make
# OR: gcc -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L process_monitor.c -o process_monitor
./process_monitor
```

### Question 3 — Employee Records (C)
```bash
cd Question3
gcc -Wall -Wextra -std=c11 employee_records.c -o employee_records
./employee_records
# Generates: employees.dat (480 bytes = 5 records × 96 bytes each)
```

### Question 4 — Log Monitor Pipeline
```bash
cd Question4
# Real-time monitoring (Terminal 1):
tail -f sample.log | grep --line-buffered "ERROR"

# Save errors to report (Terminal 1):
grep "ERROR" sample.log | tee error_report.log

# Inject new log entries (Terminal 2):
echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: Test error" >> sample.log
```

### Question 5 — vi Recovery
```bash
cd Question5
# Read the full report:
cat vi_recovery.md

# Demonstrate recovery (simulate crash):
vim test_config.conf        # Open and edit file
kill -9 $(pgrep vim)        # Simulate crash from another terminal
vim -r test_config.conf     # Recover from swap file
```

---

## 📸 Screenshots

All screenshots are stored in the `screenshots/` subfolder of each question. Each `screenshots/README.md` contains full descriptions and embedded image previews.

| Question | Screenshots | What They Show |
|---------|-------------|----------------|
| **Q1** | `Screenshot-01-script-execution.png` | Script running — 10 files processed, 3 duplicates detected |
| **Q1** | `Screenshot-02-backup-report.png` | Generated backup report with duplicate list and unique files |
| **Q2** | `Screenshot-01-compilation-and-spawn.png` | `make` compilation + 5 child processes forked and running |
| **Q2** | `Screenshot-02-sigterm-sigkill.png` | SIGTERM sent to unresponsive child → SIGKILL fallback |
| **Q3** | `Screenshot-01-write-read-records.png` | Writing 5 records + reading back with `open/write/lseek/read` |
| **Q3** | `Screenshot-02-update-hexdump.png` | In-place record update + `hexdump -C employees.dat` binary view |
| **Q4** | `Screenshot-01-grep-filter-errors.png` | `grep "ERROR" sample.log` filtering 10 errors from 52 lines |
| **Q4** | `Screenshot-02-tail-pipe-realtime-monitoring.png` | Two-terminal: `tail -f \| grep` live + error injection |
| **Q5** | `Screenshot-01-swap-file-created.png` | Vim creating `.swp` file automatically on file open |
| **Q5** | `Screenshot-02-E325-recovery-prompt.png` | E325 ATTENTION crash recovery prompt with `[R]ecover` option |
| **Q5** | `Screenshot-03-vim-r-recovery-complete.png` | `vim -r` recovery complete — `-- RECOVERED --` status bar |
| **Q5** | `Screenshot-04-registers-and-undofile.png` | `:reg` register listing + persistent `undofile` configuration |

**Total: 12 screenshots across all 5 questions.**

---

## 📚 Learning Outcomes

After completing this assignment, students will be able to:

1. **Write production-quality Bash scripts** with error handling, functions, and redirection
2. **Use `fork()`, `wait()`, `waitpid()`, `kill()`** to manage processes in C
3. **Apply Linux system calls** (`open`, `read`, `write`, `lseek`, `close`) for direct file I/O
4. **Build monitoring pipelines** using `tail`, `grep`, pipes, and redirection operators
5. **Understand vi crash recovery** mechanisms and recommend best practices

---

## 👨‍💻 Author Information

| Field | Value |
|-------|-------|
| **Assignment** | Linux Lab Assignment (Modules 5–10) |
| **Subject** | Operating Systems |
| **Language** | C (GCC, C11 standard), Bash (POSIX-compatible) |
| **Platform** | Linux / macOS (POSIX-compliant) |

---

## 📄 License

This project is submitted for academic purposes. All code is original and written specifically for this assignment.

---

*For detailed instructions, refer to the individual `README.md` inside each question folder.*
