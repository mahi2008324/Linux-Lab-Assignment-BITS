# Execution Commands — Question 5

> These commands demonstrate and test vi recovery mechanisms.

---

## Step 1: Create a Test Configuration File

```bash
echo "server { listen 80; server_name example.com; }" > test_config.conf
```

**Explanation:**  
Creates a simple test configuration file that we can safely use to demonstrate vi crash recovery without touching any real system configuration files.

**Observation:**  
`test_config.conf` is created with a basic nginx-style server block. Using `ls -l test_config.conf` confirms the file exists.

---

## Step 2: Open File in vi/vim

```bash
vim test_config.conf
```

**Explanation:**  
Opens the test file in the Vim editor. Immediately after opening, Vim creates a hidden swap file (`.test_config.conf.swp`) in the same directory to track all edits.

**Observation:**  
Vim opens with the file content visible. The swap file is created silently in the background.

---

## Step 3: Check Swap File Creation (in another terminal)

```bash
ls -la | grep .swp
```

**Explanation:**  
`ls -la` lists all files including hidden ones. The `grep .swp` filter shows only swap files. This confirms Vim created the swap file immediately upon opening.

**Observation:**  
Output shows `.test_config.conf.swp` with the current timestamp, confirming the swap file was created the moment Vim opened the file.

---

## Step 4: Simulate a Crash (Force-Kill Vim)

```bash
# While vim is open, find its PID and kill it
ps aux | grep vim
kill -9 <PID>
```

**Explanation:**  
`kill -9` sends SIGKILL to the Vim process, simulating an abrupt crash (like a power outage). The `-9` signal cannot be caught or ignored — Vim dies immediately without cleanup.

**Observation:**  
Vim closes abruptly without the normal cleanup sequence. The swap file `.test_config.conf.swp` remains on disk since Vim never got to delete it.

---

## Step 5: Verify Swap File Remains

```bash
ls -la .test_config.conf.swp
```

**Explanation:**  
After the simulated crash, we verify the swap file is still present. This is the key artifact that makes crash recovery possible.

**Observation:**  
The swap file still exists with the pre-crash modification time, ready to be used for recovery.

---

## Step 6: List All Recoverable Files

```bash
vim -r
```

**Explanation:**  
Running `vim -r` without a filename lists all swap files Vim can find and the files they correspond to, along with the last modification time of each swap file.

**Observation:**  
Output shows `test_config.conf` as a recoverable file with the crash timestamp, confirming the swap file was found.

---

## Step 7: Perform Auto-Recovery

```bash
vim -r test_config.conf
```

**Explanation:**  
This command starts Vim in recovery mode for `test_config.conf`. Vim reads the swap file (not the disk file) to reconstruct the editing session, restoring all unsaved changes.

**Observation:**  
Vim opens with a recovery header message and the restored content. All edits made before the crash are visible in the buffer.

---

## Step 8: Save the Recovered File (inside vim)

```vim
:w test_config_recovered.conf
:q
```

**Explanation:**  
`:w test_config_recovered.conf` writes the recovered buffer to a new file (safe approach). This avoids overwriting the original until we verify the recovery is complete. `:q` quits Vim.

**Observation:**  
The recovered file is saved to disk. A diff between `test_config.conf` and `test_config_recovered.conf` shows the changes that were recovered.

---

## Step 9: Delete the Swap File After Recovery

```bash
rm .test_config.conf.swp
```

**Explanation:**  
After successful recovery and saving, the swap file must be manually deleted. If left in place, Vim will always show the "swap file exists" warning when opening the file.

**Observation:**  
The swap file is removed. Running `ls -la | grep .swp` confirms no swap files remain.

---

## Step 10: Configure Persistent Undo (Demonstrating Undo File)

```bash
mkdir -p ~/.vim/undodir
cat >> ~/.vimrc << 'EOF'
set undofile
set undodir=~/.vim/undodir
set undolevels=1000
set backup
set backupdir=~/.vim/backup/
EOF
mkdir -p ~/.vim/backup
```

**Explanation:**  
These commands configure Vim for maximum safety by enabling persistent undo files and backup files. The `<<` here-document syntax appends multiple lines to `~/.vimrc` without opening an editor.

**Observation:**  
`~/.vimrc` now contains the undo and backup settings. After sourcing (`:source ~/.vimrc`), Vim creates undo files in `~/.vim/undodir/` whenever a file is edited.

---

## Step 11: View Vim Registers

```bash
vim test_config.conf
# Inside Vim, type:
:reg
```

**Explanation:**  
`:reg` (short for `:registers`) displays all registers and their current contents inside Vim. This shows what text is stored in each register (yanked/deleted text, last command, etc.).

**Observation:**  
The register list shows the unnamed register `""` containing recently yanked text, register `":` with the last command, and any named registers that were populated during editing.

---

## Step 12: Check for Backup Files

```bash
ls -la *.conf~    # Default backup extension
ls -la *.conf.bak # Alternative backup extension (if configured)
diff test_config.conf test_config.conf~ 2>/dev/null && echo "Files are identical" || echo "Backup differs from current"
```

**Explanation:**  
Lists backup files created by Vim's backup feature. `diff` compares the current file with its backup to show what changed since the last save. `2>/dev/null` suppresses errors if no backup exists.

**Observation:**  
If backup files are configured, `test_config.conf~` shows the state before the last `:w` command. The diff output shows exactly what was changed in the last save.
