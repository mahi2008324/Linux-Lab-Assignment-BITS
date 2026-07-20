# Question 1 — Shell Script: Duplicate Submission Detection & Backup

## 📌 Problem Statement

A university Linux server stores assignment submissions from hundreds of students. Design a shell script that:

1. Identifies duplicate submissions (using MD5 checksums)
2. Creates a backup of unique submissions
3. Generates a report showing total files processed, duplicates, and backed-up files
4. Stores all error messages in a separate log file

---

## 🛠️ Files in this Folder

| File/Folder | Description |
|------------|-------------|
| `duplicate_backup.sh` | Main shell script |
| `sample_submissions/` | Sample student files used to test the script |
| `backup/` | Created by the script — contains unique file backups |
| `reports/` | Contains the generated report |
| `errors.log` | All error messages captured during script execution |
| `execution_commands.md` | All commands executed with explanations |
| `outputs.md` | Sample terminal output |
| `explanation.md` | Conceptual explanation of Linux techniques used |
| `screenshots/` | Screenshots of execution |

---

## 🚀 How to Run

```bash
# Step 1: Give execute permission
chmod +x duplicate_backup.sh

# Step 2: Run the script
./duplicate_backup.sh

# Step 3: View the generated report
cat reports/backup_report_*.txt

# Step 4: View errors (if any)
cat errors.log
```

---

## 📋 Expected Behavior

- All `.txt` files inside `sample_submissions/` are scanned
- Files with identical MD5 checksums are identified as duplicates
- Only unique files are backed up to `backup/`
- A timestamped report is saved in `reports/`
- All errors are appended to `errors.log`

---

## 📊 Sample Report Format

```
============================================================
    UNIVERSITY SUBMISSION BACKUP REPORT
============================================================
Date and Time    : Sun Jul 20 21:00:00 IST 2026
Source Directory : ./sample_submissions
Backup Directory : ./backup
------------------------------------------------------------
Total files processed : 10
Duplicate files found : 3
Unique files backed up: 7
------------------------------------------------------------
DUPLICATE FILES:
  [DUPLICATE] student04.txt  (same as student01.txt)
  [DUPLICATE] student07.txt  (same as student02.txt)
  [DUPLICATE] student09.txt  (same as student03.txt)
------------------------------------------------------------
UNIQUE FILES BACKED UP:
  student01.txt
  student02.txt
  ...
============================================================
```

---

## 🔗 Related Files

- [duplicate_backup.sh](./duplicate_backup.sh)
- [execution_commands.md](./execution_commands.md)
- [explanation.md](./explanation.md)
- [outputs.md](./outputs.md)
