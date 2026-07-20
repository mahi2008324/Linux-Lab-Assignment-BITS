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
├── Question1/                         ← Shell Script: Duplicate & Backup
│   ├── README.md
│   ├── duplicate_backup.sh
│   ├── sample_submissions/            ← Sample student submission files
│   ├── backup/                        ← Created by script at runtime
│   ├── reports/                       ← Generated reports
│   ├── errors.log                     ← Error messages
│   ├── execution_commands.md
│   ├── outputs.md
│   ├── screenshots/
│   └── explanation.md
│
├── Question2/                         ← C Program: Process Monitoring
│   ├── README.md
│   ├── process_monitor.c
│   ├── Makefile
│   ├── execution_commands.md
│   ├── outputs.md
│   ├── screenshots/
│   └── explanation.md
│
├── Question3/                         ← C Program: System Calls File I/O
│   ├── README.md
│   ├── employee_records.c
│   ├── employees.dat                  ← Generated binary database
│   ├── execution_commands.md
│   ├── outputs.md
│   ├── screenshots/
│   └── explanation.md
│
├── Question4/                         ← Command Pipeline: Log Monitor
│   ├── README.md
│   ├── monitoring_commands.md
│   ├── sample.log
│   ├── error_report.log
│   ├── execution_commands.md
│   ├── outputs.md
│   ├── screenshots/
│   └── explanation.md
│
└── Question5/                         ← Report: vi Recovery Mechanisms
    ├── README.md
    ├── vi_recovery.md
    ├── execution_commands.md
    ├── screenshots/
    └── explanation.md
```

---

## ⚙️ System Requirements

| Requirement | Details |
|-------------|---------|
| **Operating System** | Linux (Ubuntu 20.04+ / Debian / CentOS 7+) |
| **Compiler** | GCC 9.0 or higher |
| **Shell** | Bash 5.0 or higher |
| **Tools** | `md5sum`, `find`, `tail`, `grep`, `awk`, `vi/vim` |
| **Kernel** | Linux 5.x or higher recommended |

---

## 🚀 How to Run Each Question

### Question 1 — Shell Script
```bash
cd Question1
chmod +x duplicate_backup.sh
./duplicate_backup.sh
```

### Question 2 — Process Monitor (C)
```bash
cd Question2
make
./process_monitor
```

### Question 3 — Employee Records (C)
```bash
cd Question3
gcc employee_records.c -o employee_records
./employee_records
```

### Question 4 — Log Monitor
```bash
cd Question4
# Follow commands in monitoring_commands.md step by step
```

### Question 5 — vi Recovery
```bash
cd Question5
# Read vi_recovery.md for the complete evaluation report
```

---

## 📸 Screenshots Guide

| Screenshot | Description | Where to Take |
|-----------|-------------|---------------|
| `Screenshot-01.png` | Running `duplicate_backup.sh` | After `./duplicate_backup.sh` |
| `Screenshot-02.png` | Generated backup report | After `cat reports/backup_report_*.txt` |
| `Screenshot-03.png` | Errors log contents | After `cat errors.log` |
| `Screenshot-04.png` | Q2 compilation success | After `make` |
| `Screenshot-05.png` | Q2 process_monitor running | After `./process_monitor` |
| `Screenshot-06.png` | Q3 compilation & execution | After `gcc ... && ./employee_records` |
| `Screenshot-07.png` | Q3 employee record read/update | Inside program menu |
| `Screenshot-08.png` | Q4 tail -f monitoring | Terminal with `tail -f sample.log` |
| `Screenshot-09.png` | Q4 grep pipeline output | Pipeline running with errors |
| `Screenshot-10.png` | Q4 error_report.log contents | After `cat error_report.log` |
| `Screenshot-11.png` | Q5 vi swap file recovery prompt | After opening crashed file in vi |

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
| **Language** | C (GCC), Bash |
| **Platform** | Linux |

---

## 📄 License

This project is submitted for academic purposes. All code is original and written specifically for this assignment.

---

*For any issues, refer to the individual `README.md` inside each question folder.*
