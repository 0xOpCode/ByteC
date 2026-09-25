# ByteC: 1-Click C and C++ Setup for Android Termux

[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Android%20%7C%20Termux%20%7C%20Linux-green.svg)]()
[![Stack](https://img.shields.io/badge/stack-Clang%20%7C%20Neovim%20%7C%20GDB%20%7C%20CMake-orange.svg)]()
[![Documentation](https://img.shields.io/badge/docs-GitHub%20Pages-blue.svg)](https://0xopcode.github.io/ByteC/)
[![Version](https://img.shields.io/badge/version-1.1.0-brightgreen.svg)](VERSION)

ByteC sets up a native C and C++ environment in Termux with one command. It installs compilers, an Allman-configured Neovim editor, debugging tools, and lab templates in 120MB of storage. It runs without proot containers.

Read the full guide: [0xopcode.github.io/ByteC](https://0xopcode.github.io/ByteC/)

---

## Installation

Open Termux on your phone and run:

```bash
curl -sL https://raw.githubusercontent.com/0xOpCode/ByteC/main/install.sh | bash
```

When installation finishes, reload your shell configuration:

```bash
source ~/.bashrc
```

> **Note for new Termux users:** Do not install Termux from Google Play Store. The Play Store build is obsolete and fails to update packages. Install Termux from [F-Droid](https://f-droid.org/packages/com.termux/) or GitHub Releases.

---

## Comparison

| Problem | Standard Termux | ByteC |
| :--- | :--- | :--- |
| Storage footprint | 3GB to 5GB (Ubuntu proot) | 120MB (Native Termux packages) |
| Missing GCC | `gcc: command not found` error | Wrappers forward `gcc` and `g++` to Clang |
| Mobile punctuation | Hidden behind keyboard sub-panels | 2-row toolbar with `{ } ( ) ; "` and arrows |
| App uninstalls | Erases code in Termux home | Links `~/c_projects` to `/sdcard/C_Projects` |
| University assignments | Write boilerplate from scratch | 10 built-in DSA and lab templates |

---

## Command Reference

### `cnew` - Create Files and Templates

Create a starter C or C++ file:

```bash
cnew lab1.c
cnew test.cpp
```

Run `cnew` without arguments to open an interactive prompt.

Create a file from a built-in lab template:

```bash
cnew lab2.c -t linkedlist    # Singly linked list operations
cnew lab3.c -t stack         # Array-based stack
cnew lab4.c -t queue         # Circular queue
cnew lab5.c -t binarytree    # Binary search tree
cnew lab6.c -t sort          # Bubble, Selection, and Insertion sorts
cnew lab7.c -t matrix        # 2D matrix addition and multiplication
cnew lab8.c -t fileio        # File read and write operations
cnew lab9.c -t pointers      # Dynamic memory and pointer swap
cnew lab10.c -t strings      # Custom string algorithms
cnew -l                      # List all templates
```

### `crun` - Compile and Run in One Step

Compile with Clang, run the binary, and print execution time:

```bash
crun lab1.c
crun lab1.c -i input.txt     # Read stdin from file
crun lab1.c -g               # Compile with debug symbols (-g -O0)
crun main.c utils.c          # Compile multiple source files
crun lab1.c arg1 arg2        # Pass command line arguments
```

If compilation fails, `crun` flags common student errors like missing semicolons, undeclared variables, and missing headers.

### `cbuild` - Multi-File Project Compiler

Compile every `.c` or `.cpp` file in the current folder into `./app`:

```bash
cbuild
cbuild main.c engine.c -o myprogram
cbuild clean                 # Remove *.o, *.out, and binary artifacts
```

### `cformat` - Code Formatter

Format source code in place using Allman or Google indentation:

```bash
cformat lab1.c               # Allman style
cformat --all                # Format all source files in directory
cformat -s google lab1.c     # Google style
```

### `cdebug` - Guided GDB Debugger

Compile with debug symbols and start GDB with a reference card:

```bash
cdebug lab1.c
```

### `ctest` - Automated Test Runner

Test program output against test cases:

```bash
ctest lab1.c input.txt expected.txt
ctest lab1.c testcase.txt    # Single file with --- INPUT --- and --- EXPECTED --- blocks
```

### `gcc` and `g++` - Compiler Wrappers

Run compiler commands from university lab sheets directly:

```bash
gcc lab1.c -o lab1
./lab1
```

---

## ByteC Manager (`bytec`)

| Command | Action |
| :--- | :--- |
| `bytec doctor` | Check installed compilers, storage links, git, and disk space |
| `bytec git` | View or customize Git author name and email |
| `bytec tutorial` | Run interactive 60-second walkthrough |
| `bytec cheatsheet <topic>` | View offline guides for `printf`, `pointers`, `strings`, `loops`, `dsa`, `keys` |
| `bytec share <file.c>` | Upload file to paste service and copy link to clipboard |
| `bytec backup` | Save archive of `~/c_projects` to `/sdcard/Download/` |
| `bytec restore <archive>` | Restore workspace from backup archive |
| `bytec stats` | Display file counts, line counts, and workspace size |
| `bytec update` | Pull latest ByteC tools and Neovim configuration |
| `bytec status` | Show installed tool versions and commit hashes |
| `bytec version` | Display current release version |
| `bytec uninstall` | Remove ByteC while preserving `~/c_projects` |

---

## Neovim Shortcuts

ByteC includes a customized Neovim setup. Keybindings:

| Shortcut | Action |
| :--- | :--- |
| `<Space> + r` | Save, format in Allman style, compile, and run |
| `Ctrl + s` | Save and format file |
| `Ctrl + z` | Undo |
| `Ctrl + y` | Redo |
| `Ctrl + Enter` | Insert newline below |
| `Ctrl + d` | Select word under cursor |
| `Ctrl + ~` or `Ctrl + j` | Toggle floating terminal |
| `<Space> + e` | Toggle file tree |
| `<Space> + ff` | Search files |

---

## Touch Toolbar

ByteC puts two rows of keys above your mobile keyboard:

```
Row 1: [ ESC   | TAB  | CTRL | ALT | { | } | ( | ) ]
Row 2: [ LEFT  | DOWN | UP   | RIGHT | " | ; | / | - ]
```

Use arrow keys to navigate code without screen tapping. Enter C punctuation without opening keyboard symbol pages.

---

## Directory Structure

```
~/.bytec/
├── bin/
│   ├── gcc                 # GCC wrapper (forwards to clang)
│   ├── g++                 # G++ wrapper (forwards to clang++)
│   ├── cnew                # File and template generator
│   ├── crun                # Compile and run runner
│   ├── cbuild              # Multi-file compiler
│   ├── cformat             # Code formatter
│   ├── cdebug              # GDB wrapper
│   ├── ctest               # Test runner
│   └── bytec               # Manager CLI
├── config/
│   ├── templates/          # 10 university lab templates
│   ├── termux.properties   # 2-row touch toolbar configuration
│   ├── colors.properties   # Pure black OLED theme
│   └── banner.sh           # Terminal startup banner
├── shell/
│   ├── aliases.sh          # Shell aliases
│   └── prompt.sh           # Minimal git prompt
├── VERSION                 # Version number
└── install.sh              # Installation script
```

---

## License

MIT License. Copyright (c) 2026 Akashdeep (0xOpCode).
