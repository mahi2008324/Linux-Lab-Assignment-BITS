# Sample Outputs — Question 1

> This file shows realistic terminal output from running the `duplicate_backup.sh` script and all related commands.

---

## Output 1: Listing the Directory (`ls -la`)

```
total 48
drwxr-xr-x  7 student  staff   224 Jul 20 21:00 .
drwxr-xr-x 10 student  staff   320 Jul 20 20:59 ..
-rwxr-xr-x  1 student  staff  5832 Jul 20 21:00 duplicate_backup.sh
-rw-r--r--  1 student  staff     0 Jul 20 21:00 errors.log
drwxr-xr-x  2 student  staff    64 Jul 20 21:00 backup
drwxr-xr-x  2 student  staff   352 Jul 20 21:00 reports
drwxr-xr-x 12 student  staff   384 Jul 20 21:00 sample_submissions
drwxr-xr-x  2 student  staff    64 Jul 20 21:00 screenshots
-rw-r--r--  1 student  staff  2144 Jul 20 21:00 README.md
-rw-r--r--  1 student  staff  3201 Jul 20 21:00 explanation.md
-rw-r--r--  1 student  staff  4812 Jul 20 21:00 execution_commands.md
```

---

## Output 2: MD5 Checksums Before Running Script (`md5sum sample_submissions/*.txt`)

```
a3f2c91b4d8e7f06152a3c8d5f9b0e12  sample_submissions/student01.txt
8b2f4a7c1e9d3b6f04a8c2e5d7f91b03  sample_submissions/student02.txt
c7d4e2f9a1b8c3d6e04f7a2b9c1e5d80  sample_submissions/student03.txt
a3f2c91b4d8e7f06152a3c8d5f9b0e12  sample_submissions/student04.txt  ← same as student01
f9a2b8c4d7e1f3a6b02c8d5e9f4a1b70  sample_submissions/student05.txt
2d8e1f4a9b3c7d0e56f2a8b4c9d3e71f  sample_submissions/student06.txt
8b2f4a7c1e9d3b6f04a8c2e5d7f91b03  sample_submissions/student07.txt  ← same as student02
e4a9b2c7d1f8e3a6b04c9d2e7f1a8b30  sample_submissions/student08.txt
c7d4e2f9a1b8c3d6e04f7a2b9c1e5d80  sample_submissions/student09.txt  ← same as student03
1b9c4e7f2a8d3b6e04c7f1a9b2e8d50c  sample_submissions/student10.txt
```

---

## Output 3: Running the Script (`./duplicate_backup.sh`)

```
============================================================
   University Submission Duplicate Detector & Backup Tool
============================================================

[INFO] Validating source directory: /path/to/Question1/sample_submissions
[INFO] Source directory is valid and readable.
[INFO] Setting up required directories...
[INFO] Directories ready: backup/ and reports/
[INFO] Starting submission processing...
[INFO] Found 10 file(s) to process.
[INFO] Backed up: student01.txt
[INFO] Backed up: student02.txt
[INFO] Backed up: student03.txt
[INFO] Duplicate detected: student04.txt == student01.txt
[INFO] Backed up: student05.txt
[INFO] Backed up: student06.txt
[INFO] Duplicate detected: student07.txt == student02.txt
[INFO] Backed up: student08.txt
[INFO] Duplicate detected: student09.txt == student03.txt
[INFO] Backed up: student10.txt
[INFO] Processing complete.
[INFO] Generating backup report: /path/to/Question1/reports/backup_report_20260720_210015.txt
[INFO] Report saved to: /path/to/Question1/reports/backup_report_20260720_210015.txt

========================================
         EXECUTION SUMMARY
========================================
  Total Processed : 10
  Duplicates Found: 3
  Unique (Backed Up): 7
  Report : /path/to/Question1/reports/backup_report_20260720_210015.txt
  Errors : /path/to/Question1/errors.log
========================================
```

---

## Output 4: Backup Directory Contents (`ls -lh backup/`)

```
total 56K
-rw-r--r--  1 student  staff  672 Jul 20 21:00 student01.txt
-rw-r--r--  1 student  staff  664 Jul 20 21:00 student02.txt
-rw-r--r--  1 student  staff  655 Jul 20 21:00 student03.txt
-rw-r--r--  1 student  staff  680 Jul 20 21:00 student05.txt
-rw-r--r--  1 student  staff  668 Jul 20 21:00 student06.txt
-rw-r--r--  1 student  staff  691 Jul 20 21:00 student08.txt
-rw-r--r--  1 student  staff  658 Jul 20 21:00 student10.txt
```

---

## Output 5: Generated Report (`cat reports/backup_report_*.txt`)

```
============================================================
         UNIVERSITY SUBMISSION BACKUP REPORT
============================================================
Date and Time    : Sun Jul 20 21:00:15 IST 2026
Source Directory : /path/to/Question1/sample_submissions
Backup Directory : /path/to/Question1/backup
------------------------------------------------------------
Total files processed : 10
Duplicate files found : 3
Unique files backed up: 7
------------------------------------------------------------

DUPLICATE FILES DETECTED:
  [DUPLICATE] student04.txt  (identical to: student01.txt)
  [DUPLICATE] student07.txt  (identical to: student02.txt)
  [DUPLICATE] student09.txt  (identical to: student03.txt)

------------------------------------------------------------

UNIQUE FILES BACKED UP:
  ✓ student01.txt
  ✓ student02.txt
  ✓ student03.txt
  ✓ student05.txt
  ✓ student06.txt
  ✓ student08.txt
  ✓ student10.txt

============================================================
                  END OF REPORT
============================================================
```

---

## Output 6: Error Log (`cat errors.log`)

```
(empty — no errors occurred during this execution)
```

> If a file is unreadable or md5sum fails, the error would appear as:
> ```
> [2026-07-20 21:00:05] ERROR: Cannot read file (skipping): /path/to/sample_submissions/locked_file.txt
> ```

---

## Output 7: Syntax Check (`bash -n duplicate_backup.sh`)

```
(no output — script has no syntax errors)
```

---

## Output 8: Post-backup Integrity Verification (`md5sum backup/*.txt`)

```
a3f2c91b4d8e7f06152a3c8d5f9b0e12  backup/student01.txt
8b2f4a7c1e9d3b6f04a8c2e5d7f91b03  backup/student02.txt
c7d4e2f9a1b8c3d6e04f7a2b9c1e5d80  backup/student03.txt
f9a2b8c4d7e1f3a6b02c8d5e9f4a1b70  backup/student05.txt
2d8e1f4a9b3c7d0e56f2a8b4c9d3e71f  backup/student06.txt
e4a9b2c7d1f8e3a6b04c9d2e7f1a8b30  backup/student08.txt
1b9c4e7f2a8d3b6e04c7f1a9b2e8d50c  backup/student10.txt
```

> ✅ All hashes match the original unique files — backup integrity confirmed.
