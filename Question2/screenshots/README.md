# Screenshots — Question 2

This folder contains screenshots taken during compilation and execution of `process_monitor`.

## Required Screenshots

| Filename | Description | When to Take |
|----------|-------------|--------------|
| `Screenshot-01.png` | `gcc --version` output | Before compiling |
| `Screenshot-02.png` | `make` compilation output | During/after make |
| `Screenshot-03.png` | `./process_monitor` — process spawning | During execution start |
| `Screenshot-04.png` | Unresponsive child being killed | During execution (timeout phase) |
| `Screenshot-05.png` | Final cleanup and exit summary | End of program |
| `Screenshot-06.png` | `ps aux | grep Z` — no zombies | After program exits |

## Tips

- Run `ps -eo pid,ppid,stat,comm | grep process_monitor` in a second terminal while
  the program is running to observe the process tree live.
- Use `watch -n 0.5 'ps -eo pid,ppid,stat,comm | grep process'` for continuous monitoring.
