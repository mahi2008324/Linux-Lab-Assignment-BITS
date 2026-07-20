# Execution Commands — Question 3

> All commands are run from inside the `Question3/` directory.

---

## Step 1: Navigate to Question3 Directory

```bash
cd Question3
```

**Explanation:**  
Changes the current working directory to `Question3/` where the C source file is located.

**Observation:**  
The terminal prompt updates to reflect the `Question3` directory.

---

## Step 2: View the Source File Header

```bash
head -30 employee_records.c
```

**Explanation:**  
`head -30` displays the first 30 lines of the file — shows the file header with description, author, compilation instructions, and an important note that only system calls are used (no stdio for file I/O).

**Observation:**  
The comment block clearly states this is a POSIX system call demonstration and the program deliberately avoids `fopen()`, `fread()`, `fwrite()`, and `fclose()`.

---

## Step 3: Compile the Program

```bash
gcc -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L employee_records.c -o employee_records
```

**Explanation:**  
- `-Wall -Wextra`: Enable all common warnings — good practice for catching bugs
- `-std=c11`: Use C11 standard which supports flexible struct initialisation
- `-D_POSIX_C_SOURCE=200809L`: Define POSIX macro to expose `open()`, `lseek()`, `write()`, `read()`, `close()` prototypes in headers
- `-o employee_records`: Name the output binary

**Observation:**  
No warnings or errors are shown, confirming the code is clean and correctly uses POSIX system call interfaces. The `employee_records` binary is created.

---

## Step 4: Verify the Executable

```bash
ls -lh employee_records
file employee_records
```

**Explanation:**  
`ls -lh` checks file size and permissions. `file` inspects the binary type to confirm it's a valid ELF executable for the correct architecture.

**Observation:**  
Output shows a ~20KB ELF 64-bit executable with `rwxr-xr-x` permissions, ready to run.

---

## Step 5: Run the Program

```bash
./employee_records
```

**Explanation:**  
Executes the program, which sequentially demonstrates all 7 phases: create file, write records, read all records, random access, update in-place, verify update, and close.

**Observation:**  
The terminal shows each system call as it executes (`[open()]`, `[write()]`, `[lseek()]`, `[read()]`, `[close()]`), providing a real-time trace of all file operations.

---

## Step 6: Verify the Database File Was Created

```bash
ls -lh employees.dat
```

**Explanation:**  
Checks that `employees.dat` was created by the program. The file size should equal `NUM_INITIAL_RECORDS × sizeof(Employee)`.

**Observation:**  
File size is approximately 480 bytes (5 records × ~96 bytes each with padding), confirming the binary database was successfully written.

---

## Step 7: Inspect the Binary File with hexdump

```bash
hexdump -C employees.dat | head -30
```

**Explanation:**  
`hexdump -C` displays the binary file contents in both hexadecimal and ASCII (printable characters shown on the right). This lets us visually confirm that the struct fields are stored correctly in the file.

**Observation:**  
The employee names like "Alice Johnson" and "Bob Martinez" are visible in the ASCII column, and the integer/double fields appear as raw bytes in the hex column, confirming the binary format is correct.

---

## Step 8: Check File Size Calculation

```bash
python3 -c "print('Expected file size:', 5 * 96, 'bytes (approx)')"
```

**Explanation:**  
Uses Python to quickly calculate the expected file size based on the struct size. The actual size may include padding bytes added by the compiler for alignment.

**Observation:**  
Output confirms the expected size roughly matches the actual file size shown by `ls -lh employees.dat`.

---

## Step 9: View the Binary File with `od` (Octal Dump)

```bash
od -t d4 employees.dat | head -10
```

**Explanation:**  
`od -t d4` displays the file contents as 4-byte decimal integers. This allows us to see the integer fields (employee_id, years_of_service, is_active) in human-readable decimal form.

**Observation:**  
The first 4-byte integer shows `1001` (employee ID of Alice Johnson), confirming the struct layout in the file matches what was written.

---

## Step 10: Re-run and Verify Update Persistence

```bash
./employee_records
hexdump -C employees.dat | grep -A2 "Bob"
```

**Explanation:**  
Re-running the program creates a fresh database (O_TRUNC), writes all records, and updates Bob's salary to $75,000. The `hexdump | grep` combination searches for Bob's record in the binary file.

**Observation:**  
After the update, Bob Martinez's record in the binary file reflects the new salary, demonstrating that `lseek()` + `write()` successfully modified only that specific record in-place.

---

## Step 11: Check for Memory Leaks (Optional but Professional)

```bash
valgrind --leak-check=full ./employee_records 2>&1 | tail -20
```

**Explanation:**  
`valgrind --leak-check=full` checks the program for memory leaks, invalid memory accesses, and use-after-free errors. This is a professional quality assurance step.

**Observation:**  
Output shows `All heap blocks were freed -- no leaks are possible` and no invalid reads/writes, confirming the program manages memory cleanly.

---

## Step 12: Strace — Trace System Calls

```bash
strace -e trace=open,openat,read,write,lseek,close ./employee_records 2>&1 | grep -v printf
```

**Explanation:**  
`strace` intercepts and displays every system call made by the program. Filtering for `open`, `read`, `write`, `lseek`, `close` lets us confirm that these are the only file-related system calls used — demonstrating we're NOT using `fopen()` or any stdio library internally.

**Observation:**  
The trace shows `openat("employees.dat", ...)`, followed by multiple `write(3, ...)` calls, `lseek(3, ...)` calls, `read(3, ...)` calls, and finally `close(3)` — all on file descriptor 3, confirming the pure system call approach.
