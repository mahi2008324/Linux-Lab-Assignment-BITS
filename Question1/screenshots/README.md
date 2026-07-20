# Screenshots — Question 1

This folder contains screenshots taken during the execution of `duplicate_backup.sh`.

## Required Screenshots

| Filename | Description | When to Take |
|----------|-------------|--------------|
| `Screenshot-01.png` | Terminal showing `ls -la` output of Question1/ | Before running the script |
| `Screenshot-02.png` | `md5sum sample_submissions/*.txt` — showing duplicate hashes | After running md5sum manually |
| `Screenshot-03.png` | Script running — real-time INFO messages | While `./duplicate_backup.sh` is running |
| `Screenshot-04.png` | Final execution summary in terminal | After script completes |
| `Screenshot-05.png` | `ls -lh backup/` — showing 7 backed-up files | After script completes |
| `Screenshot-06.png` | `cat reports/backup_report_*.txt` — full report | After viewing the report |
| `Screenshot-07.png` | `cat errors.log` — empty or with errors | After running the script |

## How to Take Screenshots on Linux

```bash
# Using scrot (install first: sudo apt install scrot)
scrot 'Screenshot-%02d.png'

# Using gnome-screenshot
gnome-screenshot -f Screenshot-01.png

# Using import (ImageMagick)
import Screenshot-01.png
```

## How to Take Screenshots on macOS

Use `Cmd + Shift + 4` to capture a selection, or `Cmd + Shift + 3` for full screen.
