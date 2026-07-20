# Screenshots — Question 5

This folder contains screenshots demonstrating vi recovery mechanisms.

## Required Screenshots

| Filename | Description | When to Take |
|----------|-------------|--------------|
| `Screenshot-01.png` | `ls -la \| grep .swp` — showing swap file created | Right after opening vim |
| `Screenshot-02.png` | vim -r output listing recoverable files | After `vim -r` with no arguments |
| `Screenshot-03.png` | vim -r recovery mode with "Recovery completed" | After `vim -r filename` |
| `Screenshot-04.png` | Recovered file contents in vim | During recovery |
| `Screenshot-05.png` | `:reg` output — showing registers | Inside vim |
| `Screenshot-06.png` | Swap file E325 warning prompt | Opening file with existing swap |

## Simulating a Crash for Screenshots

```bash
# Terminal 1: Open vim and make some edits
vim test_config.conf
# Type: i (insert mode), add some text, press Escape

# Terminal 2: Capture swap file
ls -la | grep .swp
# Take Screenshot-01 here

# Terminal 2: Kill vim to simulate crash
kill -9 $(pgrep vim)
# Take Screenshot of killed terminal

# Recovery:
vim -r test_config.conf
# Take Screenshot-03 here
```
