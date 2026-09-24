# ⚡ ByteC — 1-Click Pocket C/C++ IDE for Android & Termux

[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Android%20%7C%20Termux%20%7C%20Linux-green.svg)]()
[![Stack](https://img.shields.io/badge/stack-Clang%20%7C%20Neovim%20%7C%20CMake-orange.svg)]()

> **Turn your Android phone into a high-performance C & C++ development powerhouse in 1 click — no heavy proot distro needed (~100MB vs 3GB).**

---

## 🚀 1-Click Installation

Open **Termux** and paste this single command:

```bash
curl -sL https://raw.githubusercontent.com/0xOpCode/ByteC/main/install.sh | bash
```

After installation completes, restart Termux or run:
```bash
source ~/.bashrc
```

---

## 🌟 Why ByteC? (Built for College Students & Developers)

| Problem in College / Termux | How ByteC Solves It |
| :--- | :--- |
| **No Phone Storage for proot** (Ubuntu/Debian needs 2-4GB) | **100% Native Termux** (~120MB total footprint). |
| **No GCC in Termux** (`gcc: command not found` error) | **Smart `gcc` & `g++` wrappers** forwarding seamlessly to `clang`. |
| **Typing `{ } ( ) ; "` on mobile keyboard is painful** | **Custom Termux Touch Row** with all C programming symbols. |
| **Code lost when clearing Termux data / uninstalling** | Auto-symlinks `~/c_projects` ➔ Phone Internal Storage (`/sdcard/C_Projects`). |
| **Manual re-installs when new features arrive** | **`bytec update`** pulls latest features in 1 second. |
| **Clunky terminal editors** | Bundled with **0xOpCode Neovim C IDE** (VS Code shortcuts, Allman style, OLED black). |

---

## 🛠️ CLI Tools & Helpers

### 1. `cnew <file.c>` — Instant Boilerplate
Create a clean, ready-to-run C or C++ file with standard library headers and `main()` function:
```bash
cnew lab1.c
cnew test.cpp
```

### 2. `crun <file.c>` — 1-Step Compile & Run
Compiles with `clang`, runs the binary, measures execution time in milliseconds, and prints exit status:
```bash
crun lab1.c
crun lab1.c "arg1" "arg2"
```

### 3. `gcc` & `g++` — Seamless Lab Commands
Run your college lab manual commands without errors:
```bash
gcc lab1.c -o lab1
./lab1
```

### 4. `bytec update` — 1-Word Auto Updater
Pulls latest Neovim settings and ByteC tools directly from GitHub:
```bash
bytec update
```

### 5. `bytec status` — Environment Inspector
Shows installed compiler versions, Neovim commit, and storage link status.

---

## ⌨️ Neovim IDE Keybindings Cheatsheet

When editing in Neovim (`nvim file.c`):

| Shortcut | Action |
| :--- | :--- |
| **`<Space> + r`** | **Save, Auto-Format (Allman), Compile & Run in Full Screen** |
| **`Ctrl + s`** | Save & Format current file |
| **`Ctrl + z`** | Undo |
| **`Ctrl + y`** | Redo |
| **`Ctrl + Enter`** | Insert new line below |
| **`Ctrl + Shift + Enter`** | Insert new line above |
| **`Ctrl + d`** | Select word under cursor |
| **`Select text + (`** | Auto-wrap selected text in `(...)` (also works with `[`, `{`, `"`, `'`) |
| **`Ctrl + ~` / `Ctrl + j`** | Toggle / Hide / Unhide Floating Terminal |
| **`<Space> + e`** | Toggle File Explorer Tree |
| **`<Space> + ff`** | Fuzzy Search Files |

---

## 📱 Termux Touch Extra Keys Bar

Custom row configured above your phone keyboard:
```
[ ESC | TAB | CTRL | ALT | { | } | ( | ) | " | ; | / | - ]
```

---

## 📂 Project Structure

```
~/.bytec/
├── bin/
│   ├── gcc                 # Smart GCC wrapper
│   ├── g++                 # Smart G++ wrapper
│   ├── cnew                # Boilerplate generator
│   ├── crun                # Compile & Run tool
│   └── bytec               # Master updater & CLI
├── config/
│   ├── termux.properties   # Touch keys row
│   ├── colors.properties   # OLED Pure Black theme
│   └── banner.sh           # Minimal startup banner
├── shell/
│   ├── aliases.sh          # Quick shortcuts & auto-update daemon
│   └── prompt.sh           # Fast git-aware shell prompt
└── install.sh              # 1-Click installer
```

---

## 🛡️ License

MIT License © 2026 [0xOpCode](https://github.com/0xOpCode)
