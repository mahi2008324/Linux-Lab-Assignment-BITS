# Question 5 — `vi` Recovery Mechanisms: Evaluation Report

## 📌 Problem Statement

A software developer was editing a critical configuration file using `vi` when the system crashed before the file was saved. Evaluate the different recovery mechanisms available in `vi`, propose the most reliable recovery strategy, and justify the answer.

---

## 🛠️ Files in this Folder

| File | Description |
|------|-------------|
| `vi_recovery.md` | Complete evaluation report |
| `execution_commands.md` | Commands for testing recovery mechanisms |
| `explanation.md` | Detailed technical explanation |
| `screenshots/` | Screenshots of recovery in action |

---

## 📋 Recovery Mechanisms Covered

1. **Swap Files** — `.swp` files created by vi automatically
2. **Undo History** — vi's internal undo stack
3. **Registers** — vi's clipboard/register system
4. **Backup Files** — auto-created `.bak` or `~` files
5. **Auto-Recovery** — `vim -r` recovery mode

---

## 🔗 Related Files

- [vi_recovery.md](./vi_recovery.md)
- [execution_commands.md](./execution_commands.md)
- [explanation.md](./explanation.md)
