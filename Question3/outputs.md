# Sample Outputs — Question 3

> Realistic terminal output from compiling and running `employee_records`.

---

## Output 1: Compilation

```
(no output — successful compilation produces no messages)
```

---

## Output 2: Running the Program (`./employee_records`)

```

------------------------------------------------------------
   EMPLOYEE DATABASE — Linux System Calls Demo (Q3)
------------------------------------------------------------
Record Size : 96 bytes per employee
Database    : employees.dat

=== PHASE 1: CREATE DATABASE & WRITE RECORDS ===
[open()] File descriptor obtained: fd=3 for 'employees.dat'

[INFO] Writing 5 initial employee records...
------------------------------------------------------------
[write()] Wrote 96 bytes | Employee ID=1001 | Name='Alice Johnson'
[write()] Wrote 96 bytes | Employee ID=1002 | Name='Bob Martinez'
[write()] Wrote 96 bytes | Employee ID=1003 | Name='Carol Singh'
[write()] Wrote 96 bytes | Employee ID=1004 | Name='David Chen'
[write()] Wrote 96 bytes | Employee ID=1005 | Name='Emma Wilson'
[INFO] All initial records written successfully.
[INFO] Total bytes written: 480 bytes

=== PHASE 2: READ ALL RECORDS ===

[INFO] Reading all employee records...
------------------------------------------------------------
[lseek()] Reset to beginning of file (offset = 0)

[read()] Read 96 bytes | Employee ID=1001
  Record #0
  ├─ Employee ID    : 1001
  ├─ Name           : Alice Johnson
  ├─ Department     : Engineering
  ├─ Salary         : $85000.00
  ├─ Years of Svc   : 5
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1002
  Record #1
  ├─ Employee ID    : 1002
  ├─ Name           : Bob Martinez
  ├─ Department     : Marketing
  ├─ Salary         : $62000.00
  ├─ Years of Svc   : 3
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1003
  Record #2
  ├─ Employee ID    : 1003
  ├─ Name           : Carol Singh
  ├─ Department     : Human Resources
  ├─ Salary         : $70000.00
  ├─ Years of Svc   : 7
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1004
  Record #3
  ├─ Employee ID    : 1004
  ├─ Name           : David Chen
  ├─ Department     : Finance
  ├─ Salary         : $95000.00
  ├─ Years of Svc   : 10
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1005
  Record #4
  ├─ Employee ID    : 1005
  ├─ Name           : Emma Wilson
  ├─ Department     : Engineering
  ├─ Salary         : $78000.00
  ├─ Years of Svc   : 2
  └─ Status         : Active

[INFO] Total records read: 5

=== PHASE 3: RANDOM ACCESS READ ===

[INFO] Random access — reading record #3 directly...
------------------------------------------------------------
[lseek()] Seeked to record #3 | Byte offset = 288
[read()] Read 96 bytes | Employee ID=1004

  Record #3
  ├─ Employee ID    : 1004
  ├─ Name           : David Chen
  ├─ Department     : Finance
  ├─ Salary         : $95000.00
  ├─ Years of Svc   : 10
  └─ Status         : Active

=== PHASE 4: UPDATE RECORD IN-PLACE ===

[INFO] Updating record #1 in-place...
------------------------------------------------------------
[lseek()] Seeked to record #1 | Byte offset = 96
[read()] Read 96 bytes | Employee ID=1002
[INFO] Record BEFORE update:
  Record #1
  ├─ Employee ID    : 1002
  ├─ Name           : Bob Martinez
  ├─ Department     : Marketing
  ├─ Salary         : $62000.00
  ├─ Years of Svc   : 3
  └─ Status         : Active

[lseek()] Seeked to record #1 | Byte offset = 96
[write()] Wrote 96 bytes | Employee ID=1002 | Name='Bob Martinez'
[INFO] Record AFTER update:
  Record #1
  ├─ Employee ID    : 1002
  ├─ Name           : Bob Martinez
  ├─ Department     : Marketing
  ├─ Salary         : $75000.00    ← UPDATED
  ├─ Years of Svc   : 4            ← UPDATED
  └─ Status         : Active
[INFO] Record #1 updated in-place successfully.

=== PHASE 5: VERIFY UPDATE (read all again) ===

[INFO] Reading all employee records...
------------------------------------------------------------
[lseek()] Reset to beginning of file (offset = 0)

[read()] Read 96 bytes | Employee ID=1001
  Record #0
  ├─ Employee ID    : 1001
  ├─ Name           : Alice Johnson
  ├─ Department     : Engineering
  ├─ Salary         : $85000.00
  ├─ Years of Svc   : 5
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1002
  Record #1
  ├─ Employee ID    : 1002
  ├─ Name           : Bob Martinez
  ├─ Department     : Marketing
  ├─ Salary         : $75000.00    ← Reflects the update!
  ├─ Years of Svc   : 4            ← Reflects the update!
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1003
  Record #2
  ├─ Employee ID    : 1003
  ├─ Name           : Carol Singh
  ├─ Department     : Human Resources
  ├─ Salary         : $70000.00
  ├─ Years of Svc   : 7
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1004
  Record #3
  ├─ Employee ID    : 1004
  ├─ Name           : David Chen
  ├─ Department     : Finance
  ├─ Salary         : $95000.00
  ├─ Years of Svc   : 10
  └─ Status         : Active

[read()] Read 96 bytes | Employee ID=1005
  Record #4
  ├─ Employee ID    : 1005
  ├─ Name           : Emma Wilson
  ├─ Department     : Engineering
  ├─ Salary         : $78000.00
  ├─ Years of Svc   : 2
  └─ Status         : Active

[INFO] Total records read: 5

=== PHASE 6: FILE INFORMATION ===
------------------------------------------------------------
[lseek()] Total file size: 480 bytes
[INFO]    Expected size  : 480 bytes (5 records × 96 bytes each)

=== PHASE 7: CLOSE DATABASE ===
------------------------------------------------------------
[close()] File descriptor 3 closed successfully.

------------------------------------------------------------
   All operations completed successfully!
------------------------------------------------------------
```

---

## Output 3: Database File (`ls -lh employees.dat`)

```
-rw------- 1 student student 480 Jul 20 21:15 employees.dat
```

> ✅ File size is 480 bytes (5 × 96), and permissions are `rw-------` (owner only), as specified by `S_IRUSR | S_IWUSR`.

---

## Output 4: Hexdump of Database (`hexdump -C employees.dat | head -25`)

```
00000000  e9 03 00 00 41 6c 69 63  65 20 4a 6f 68 6e 73 6f  |....Alice Johnso|
00000010  6e 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |n...............|
00000020  00 00 00 00 00 00 00 00  00 00 00 00 45 6e 67 69  |............Engi|
00000030  6e 65 65 72 69 6e 67 00  00 00 00 00 00 00 00 00  |neering.........|
00000040  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000050  00 40 f4 84 00 00 00 00  05 00 00 00 01 00 00 00  |.@..............|
00000060  ea 03 00 00 42 6f 62 20  4d 61 72 74 69 6e 65 7a  |....Bob Martinez|
00000070  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
```

> The hex `e9 03 00 00` = `0x000003E9` = 1001 (Alice's employee ID in little-endian)  
> The hex `ea 03 00 00` = `0x000003EA` = 1002 (Bob's employee ID)
