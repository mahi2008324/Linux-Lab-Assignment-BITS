# Execution Commands — Question 1

> All commands are run from inside the `Question1/` directory.  
> Each command is followed by an **Explanation** and **Observation**.

---

## Step 1: Navigate to the Question1 Directory

```bash
cd Question1
```

**Explanation:**  
This changes the current working directory to `Question1/` so that all subsequent commands are relative to this directory where the script and sample files reside.

**Observation:**  
The terminal prompt changes to reflect the new directory (`Question1`), confirming the navigation was successful.

---

## Step 2: List the Contents of the Directory

```bash
ls -la
```

**Explanation:**  
`ls -la` lists all files and directories (including hidden ones) with detailed information such as permissions, ownership, size, and modification time. This verifies all required files are present.

**Observation:**  
The output shows `duplicate_backup.sh`, `sample_submissions/`, `backup/`, `reports/`, and `errors.log` — confirming the repository structure is intact.

---

## Step 3: Verify the Sample Submissions

```bash
ls -lh sample_submissions/
```

**Explanation:**  
This lists all files inside `sample_submissions/` with human-readable file sizes (`-h`). It confirms that 10 student submission files exist before running the script.

**Observation:**  
10 `.txt` files (student01.txt through student10.txt) are listed with their sizes, confirming the test dataset is ready.

---

## Step 4: Check MD5 Checksums Manually (Pre-test)

```bash
md5sum sample_submissions/*.txt
```

**Explanation:**  
`md5sum` computes the MD5 hash of each file. By examining the output, we can manually verify which files share identical hashes (indicating duplicates) before running the script.

**Observation:**  
Files student04.txt, student07.txt, and student09.txt show the same hashes as student01.txt, student02.txt, and student03.txt respectively, confirming the test data has 3 known duplicates.

---

## Step 5: Give Execute Permission to the Script

```bash
chmod +x duplicate_backup.sh
```

**Explanation:**  
`chmod +x` adds the execute (`x`) permission to the script file, allowing it to be run directly as a program. Without this, the shell would refuse to execute the file.

**Observation:**  
The permission bits change from `-rw-r--r--` to `-rwxr-xr-x`, as confirmed by `ls -la duplicate_backup.sh`.

---

## Step 6: Run the Script

```bash
./duplicate_backup.sh
```

**Explanation:**  
`./duplicate_backup.sh` runs the shell script in the current directory. The `./` prefix explicitly tells the shell to look in the current directory (since `.` is not in `$PATH` for security reasons).

**Observation:**  
The script prints progress messages, processes all 10 files, detects 3 duplicates, backs up 7 unique files, and generates a report — all visible in the terminal output.

---

## Step 7: Verify the Backup Directory

```bash
ls -lh backup/
```

**Explanation:**  
This confirms that the `backup/` directory was created by the script and contains exactly the unique files identified during processing.

**Observation:**  
7 files are present in `backup/` (student01.txt through student06.txt and student08.txt, student10.txt — the 7 unique submissions), matching the script's reported count.

---

## Step 8: View the Generated Report

```bash
cat reports/backup_report_*.txt
```

**Explanation:**  
`cat` displays the contents of the report file. The `*` wildcard matches the timestamped filename automatically without needing to know the exact timestamp.

**Observation:**  
The report clearly shows 10 files processed, 3 duplicates found, and 7 unique files backed up, along with the names of all duplicate and unique files.

---

## Step 9: Check the Error Log

```bash
cat errors.log
```

**Explanation:**  
This displays the contents of `errors.log`, which captures all errors that occurred during script execution via the `>>` append redirection operator.

**Observation:**  
The file is empty (or shows no critical errors), indicating the script ran without failures. If permissions issues or missing files existed, they would appear here.

---

## Step 10: Verify File Integrity (Post-backup)

```bash
md5sum backup/*.txt
```

**Explanation:**  
Re-running `md5sum` on the backed-up files confirms that the files were copied correctly — the hashes in `backup/` must exactly match the original unique files in `sample_submissions/`.

**Observation:**  
All hashes in `backup/` match the hashes of the corresponding original unique files, confirming the integrity of the backup operation.

---

## Step 11: View Script with Line Numbers (Code Inspection)

```bash
cat -n duplicate_backup.sh | head -50
```

**Explanation:**  
`cat -n` adds line numbers to each line of the file, and `head -50` shows only the first 50 lines. This is useful for reviewing and explaining specific sections of the script.

**Observation:**  
The script header, strict mode settings, and variable declarations are clearly visible and well-commented.

---

## Step 12: Check Bash Script for Syntax Errors

```bash
bash -n duplicate_backup.sh
```

**Explanation:**  
`bash -n` runs the script in "no-execute" (dry-run) mode — it parses the entire script for syntax errors without actually running it. This is a crucial pre-execution validation step.

**Observation:**  
No output is produced, which means there are no syntax errors. If there were errors, bash would print the line number and description.

---

## Step 13: Verify Report Count

```bash
wc -l reports/backup_report_*.txt
```

**Explanation:**  
`wc -l` counts the number of lines in the report file. This gives a quick sanity check that the report was generated with a reasonable amount of content.

**Observation:**  
The line count matches the expected report length, confirming the report is complete and not truncated.
