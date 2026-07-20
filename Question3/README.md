# Question 3 — C Program: Secure File I/O with Linux System Calls

## 📌 Problem Statement

Design a secure file-processing utility using **only Linux system calls** (no stdio). Implement an employee database that:

1. Creates a file using `open()`
2. Writes employee records using `write()`
3. Updates specific records in-place using `lseek()` + `write()` (without rewriting the whole file)
4. Retrieves any record efficiently using `lseek()` for random access
5. Closes the file properly using `close()`

---

## 🛠️ Files in this Folder

| File | Description |
|------|-------------|
| `employee_records.c` | Main C source file (uses ONLY Linux system calls for file I/O) |
| `employees.dat` | Binary database file (generated when program runs) |
| `execution_commands.md` | All compilation & execution commands |
| `outputs.md` | Sample program outputs |
| `explanation.md` | Detailed explanation of each system call |
| `screenshots/` | Screenshots of execution |

---

## 🚀 How to Build and Run

```bash
# Compile
gcc -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L employee_records.c -o employee_records

# Run
./employee_records

# Verify database file was created
ls -lh employees.dat
hexdump -C employees.dat | head -20
```

---

## 📋 Database Schema

Each record is a fixed-size C struct:

```c
typedef struct {
    int    employee_id;          // 4 bytes
    char   name[40];             // 40 bytes
    char   department[30];       // 30 bytes
    double salary;               // 8 bytes
    int    years_of_service;     // 4 bytes
    int    is_active;            // 4 bytes
} Employee;                      // Total: ~90 bytes (+ padding)
```

Fixed-size records enable **O(1) random access**:
```
offset to record N = N × sizeof(Employee)
```

---

## 📊 Sample Employees

| ID | Name | Department | Salary | Years |
|----|------|-----------|--------|-------|
| 1001 | Alice Johnson | Engineering | $85,000 | 5 |
| 1002 | Bob Martinez | Marketing | $62,000 | 3 |
| 1003 | Carol Singh | Human Resources | $70,000 | 7 |
| 1004 | David Chen | Finance | $95,000 | 10 |
| 1005 | Emma Wilson | Engineering | $78,000 | 2 |

---

## 🔗 Related Files

- [employee_records.c](./employee_records.c)
- [execution_commands.md](./execution_commands.md)
- [explanation.md](./explanation.md)
- [outputs.md](./outputs.md)
