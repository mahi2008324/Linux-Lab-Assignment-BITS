# Sample Outputs — Question 5

> Realistic terminal outputs demonstrating each vi/vim recovery mechanism.
> All outputs correspond to the screenshots in the `screenshots/` folder.

---

## Output 1: Swap File Automatically Created

```bash
$ echo "server { listen 80; server_name example.com; }" > test_config.conf
$ vim test_config.conf
[vim opens — editing in progress]
```

In a second terminal:

```bash
$ ls -la | grep .swp
-rw-r--r-- 1 student student 4096 Jul 20 22:00 .test_config.conf.swp

$ file .test_config.conf.swp
.test_config.conf.swp: Vim swap file, version 8.2
```

**Explanation:**  
Vim created `.test_config.conf.swp` automatically the moment the file was opened. No configuration was needed. This swap file will survive a crash.

---

## Output 2: Simulating a Crash (Force-Killing Vim)

```bash
$ ps aux | grep vim
student  14523  0.3  0.1  22456  8192 pts/0 S+  22:00  0:00 vim test_config.conf

$ kill -9 14523
Killed
```

**Explanation:**  
`kill -9` sends `SIGKILL` — the uncatchable signal. Vim dies immediately without any cleanup, exactly like a power failure or system crash.

---

## Output 3: E325 ATTENTION Recovery Prompt

After the crash, when the user reopens the file:

```bash
$ vim test_config.conf

E325: ATTENTION
Found a swap file by the name ".test_config.conf.swp"
          owned by: student   dated: Sun Jul 20 22:00:15 2026
         file name: ~/Linux-Lab-Assignment/Question5/test_config.conf
          modified: YES
         user name: student   host name: ubuntu
        process ID: 14523 (not existing)
While opening file "test_config.conf"
             dated: Sun Jul 20 21:58:30 2026

(1) Another program may be editing the same file.  If this is the case,
    be careful not to end up with two different instances of the same
    file when making changes.  Quit, or continue with caution.
(2) An edit session for this file crashed.

If it was a recent crash, it should be possible to recover most of your work.
You can try to do this now by typing ":recover" or "vim -r test_config.conf"

Swap file ".test_config.conf.swp" already exists!
[O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit, (A)bort: R
```

**Explanation:**  
Vim detects the orphaned swap file and shows the E325 prompt. "process ID: 14523 (not existing)" confirms the old Vim process is gone (i.e., this is a crash, not another open session). Selecting **R** starts recovery.

---

## Output 4: `vim -r` Auto-Recovery Command

```bash
$ vim -r test_config.conf

Using swap file ".test_config.conf.swp"
Original file "test_config.conf"
Recovery completed. Buffer contents equals file contents.
You should do  ":write"  to write the file.
NOTE: You need to delete the swap file manually.
```

Inside vim after recovery (status bar):
```
-- RECOVERED --                                      9,1          All
```

**Explanation:**  
`vim -r` instructs Vim to load from the swap file rather than the disk file. "Recovery completed" confirms all unsaved changes have been restored into the buffer.

---

## Output 5: Listing All Recoverable Files

```bash
$ vim -r

   In directory ~/Linux-Lab-Assignment/Question5/:
         -- swap file --
   .test_config.conf.swp
             dated: Sun Jul 20 22:00:15 2026
            file name: test_config.conf
             modified: YES
            user name: student   host name: ubuntu
           process ID: 14523 (not existing)
```

**Explanation:**  
Running `vim -r` with no arguments lists ALL recoverable swap files on the system. This is useful when you don't remember which files were open during the crash.

---

## Output 6: Inside Vim — `:reg` Registers

```vim
:reg

--- Registers ---
""   server { listen 80; server_name example.com; }
"0   server_name example.com;
"1   location / {
":   reg
".   index.html;
"/   server_name
"a   # CRITICAL BACKUP CONFIG LINE
Press ENTER or type command to continue
```

**Explanation:**  
`:reg` lists all Vim register contents. The unnamed register `""` holds the last yanked/deleted text. Named register `"a` stores user-explicitly yanked text. These are embedded in the swap file and may be partially recoverable after a crash.

---

## Output 7: Persistent Undo File

```bash
$ ls -la ~/.vim/undodir/
total 16
drwxr-xr-x 2 student student 4096 Jul 20 22:01 .
drwxr-xr-x 8 student student 4096 Jul 20 22:01 ..
-rw-r--r-- 1 student student 2048 Jul 20 22:01 %home%student%Linux-Lab-Assignment%Question5%test_config.conf
```

**Explanation:**  
With `set undofile` in `~/.vimrc`, Vim saves the complete undo history to a file in `~/.vim/undodir/`. The filename encodes the full path (slashes replaced by `%`). This allows undo operations even after closing and reopening the file.

---

## Output 8: Cleanup After Recovery

```bash
$ ls -la | grep .swp
-rw-r--r-- 1 student student 4096 Jul 20 22:00 .test_config.conf.swp

$ rm .test_config.conf.swp

$ ls -la | grep .swp
(no output)

$ echo "Cleanup complete — no more swap file warnings."
Cleanup complete — no more swap file warnings.
```

**Explanation:**  
After a successful recovery and saving the file (`:w` inside vim), the swap file must be manually deleted. If left in place, Vim will show the E325 warning every time the file is opened.

---

## Summary: Recovery Priority Order

| Step | Command | Data Recovered |
|------|---------|---------------|
| 1 | `vim -r filename` | All unsaved edits (up to 4 seconds before crash) |
| 2 | `:earlier 10m` | Undo tree states from 10 minutes ago |
| 3 | `cat filename~` | Last explicitly saved version |
| 4 | `:reg` | Yanked/deleted text fragments |
