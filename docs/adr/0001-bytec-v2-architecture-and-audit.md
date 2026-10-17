# ADR 0001: ByteC v2.0.0 Audit, Beginner-First Command Architecture, and Neovim VS Code Emulation

- **Status:** Proposed
- **Date:** 2026-10-17
- **Target Release:** ByteC v2.0.0 & `0xOpCode/nvim` v2.0.0

---

## 1. Context and Problem Statement

ByteC v1.1.0 provides shell wrappers (`cnew`, `crun`, `cbuild`, `cformat`, `cdebug`, `ctest`, `bytec`) and a companion Neovim configuration (`0xOpCode/nvim`) for Android Termux and Linux. Usage by first-year students with zero Linux or CLI background exposed defects across four areas:

1. **Neovim VS Code Shortcut Emulation (`0xOpCode/nvim`):** Terminal keycode limitations, mode mismatches, conflicting format-on-save hooks, and missing modal transitions break standard VS Code workflows (`Ctrl+C`, `Ctrl+V`, `Ctrl+A`, `Ctrl+/`, `Ctrl+D`, `Ctrl+Enter`, `Ctrl+~`, `Space+r`).
2. **Installer and CLI Reliability (`ByteC`):** Output redirection hides mirror prompts, interactive `read` calls fail under `curl | bash`, argument quoting in `crun` corrupts spaces, and `cbuild` fails when multiple lab files reside in one directory.
3. **Cognitive Load for Non-CLI Users:** Students who have never used a terminal struggle with directory navigation, modal editing (`Normal` vs `Insert` mode), compiler diagnostics, test verification, and lab report formatting.
4. **Documentation Tone and Learning Path:** Existing documentation lacks beginner onboarding, practical humor (such as *Beej's Guide to C*), and structured links to external C, C++, DSA, Linux, and Termux learning resources.

---

## 2. Technical Audit (v1.1.0)

### 2.1 Neovim Configuration Bugs (`0xOpCode/nvim`)

| ID | File & Lines | Defect | Root Cause & Failure Scenario |
| :--- | :--- | :--- | :--- |
| **NV-01** | `lua/config/keymaps.lua:191-214` | `<Space>r` runner fails on user `scanf` input and leaves binaries in source dir | `full_cmd` chains compile and run with `&&` followed by `; read`. When user code reads `stdin` via `scanf` and leaves a newline or EOF in the buffer, `read` consumes that leftover input and closes the floating window before the student can read the output. It also writes compiled binaries into `~/c_projects`. |
| **NV-02** | `lua/plugins/lsp.lua:191-196` vs `after/ftplugin/c.lua:21` | `Ctrl+S` reformats Allman braces back to K&R/LLVM | `conform.nvim` invokes `clang-format` on `BufWritePre` (`Ctrl+S` and `<Space>r`) without `--style` flags. When `.clang-format` is absent in `~/c_projects`, `clang-format` defaults to LLVM style (opening brace on the same line), overriding `cinoptions`. |
| **NV-03** | `lua/config/keymaps.lua:81-83` & `lua/plugins/snacks.lua:70-72` | `Ctrl+~` and `Ctrl+Enter` do nothing in Termux | Standard terminal emulators and Android touch keyboards do not transmit distinct CSI sequences for `<C-~>`, `<C-`>`, `<C-CR>`, or `<C-S-CR>`. `<C-CR>` arrives as `<CR>`. |
| **NV-04** | `lua/config/keymaps.lua:43-45` | `Ctrl+D` broken across modes | In Normal mode, `viw` selects the word in Visual mode. Pressing `Ctrl+D` a second time runs `[["yy/<C-r>y<CR>gn]]`, which fails on regex characters and moves the cursor without multi-cursor editing. In Insert mode, `<Esc>viw` shifts the cursor 1 column left before selecting, grabbing the wrong word at boundaries. |
| **NV-05** | `lua/config/keymaps.lua:48-72` | Visual auto-surround breaks on multi-line selections | `visual_wrap` runs `normal! "sd` and `nvim_paste(open .. sel .. close)`. In linewise visual mode (`V`), `sel` contains a trailing newline, placing the closing quote or bracket on a separate line. |
| **NV-06** | `lua/config/keymaps.lua` (Missing) | Missing `Ctrl+C`, `Ctrl+V`, `Ctrl+X`, `Ctrl+A`, `Ctrl+/` | Students press `Ctrl+C` to copy (cancels mode), `Ctrl+V` to paste (enters Visual Block mode `<C-v>`), `Ctrl+A` to select all (increments numbers in Normal mode), and `Ctrl+/` to toggle comments (sends `<C-_>` in terminals). |
| **NV-07** | `lua/config/options.lua:31` | Clipboard copy/paste fails in Termux | `opt.clipboard = "unnamedplus"` requires `termux-clipboard-get` and `termux-clipboard-set` from `termux-api`. `install.sh` does not install `termux-api` or configure a fallback. |
| **NV-08** | `lua/plugins/snacks.lua:15-23` | Dashboard branding and keys mismatch | Dashboard displays "DAEMON OS - ULTRA FAST OLED NEURON IDE" instead of ByteC student onboarding actions. |
| **NV-09** | `lua/plugins/lsp.lua:45,158` | `require("lspconfig")` deprecation on Neovim 0.11+ | Termux ships Neovim `0.11+`, where `lspconfig[server].setup` triggers deprecation warnings in favor of `vim.lsp.config` and `vim.lsp.enable`. |
| **NV-10** | `lua/plugins/treesitter.lua:10-32` | Startup freeze on mobile CPUs | `nvim-treesitter` compiles 20 language parsers on first launch, freezing budget ARM phones for 2-4 minutes. |

### 2.2 ByteC Installer & CLI Bugs (`ByteC`)

| ID | File & Lines | Defect | Root Cause & Failure Scenario |
| :--- | :--- | :--- | :--- |
| **BC-01** | `install.sh:73-88, 263` | Hang during `install.sh` | `pkg update -y >/dev/null 2>&1` and `pkg install ... >/dev/null 2>&1` hide progress and any dpkg prompt. Users on slow mobile connections see a frozen screen with zero status output. |
| **BC-02** | `install.sh:273-276` | `/sdcard` permission race condition | `termux-setup-storage` opens an asynchronous Android OS permission popup. `sleep 1` completes before the user taps "Allow", so `[ -d "/sdcard" ]` fails and `~/c_projects` is created as an internal folder instead of linking to `/sdcard/C_Projects`. |
| **BC-03** | `install.sh:315-316` | New Termux sessions skip ByteC | Login shells in Termux read `~/.bash_profile` or `~/.profile`. When `~/.bash_profile` exists without sourcing `~/.bashrc`, ByteC aliases and PATH updates never load. |
| **BC-04** | `bin/gcc:12-22`, `bin/g++:12-22` | Recursion hazard if `clang` is missing | `bin/gcc` calls `exec clang "$@"` without resolving the full binary path or guarding against wrapper self-invocation. |
| **BC-05** | `bin/crun:66, 107, 144-150` | Broken argument quoting and spaces in paths | `SOURCES="$SOURCES $1"` and `eval "\"$BIN\" $PROG_ARGS"` split filenames or arguments containing spaces. |
| **BC-06** | `bin/crun:43-70` | Cannot pass flags to the user's C program | `while [ $# -gt 0 ]` intercepts `-i` and `-g` at any position instead of forwarding arguments after the source filename or `--`. |
| **BC-07** | `bin/crun:107` | Uninformative runtime crashes | Out-of-bounds array accesses or null pointer dereferences print `Segmentation fault` with no line number or variable state. |
| **BC-08** | `bin/cbuild:55-61` | Compiles unrelated `.c` files into one binary | Students store `lab1.c`, `lab2.c`, and `lab3.c` in `~/c_projects`. Running `cbuild` compiles all of them together and fails with `multiple definition of 'main'`. |
| **BC-09** | `bin/ctest:52-53, 83` | Infinite loop hang and single-test limit | Student code with `while(1)` hangs `ctest` with no timeout. Also, `ctest` supports one test case per file instead of multiple cases. |
| **BC-10** | `shell/prompt.sh:17-38` | Broken prompt in Zsh | `shell/prompt.sh` sets `PROMPT_COMMAND` and uses Bash `\[\033...\]` escapes, printing literal escape strings in Zsh. |
| **BC-11** | `config/termux.properties:7-10` | Missing C/C++ symbols on mobile touch bar | Touch bar lacks `#`, `<`, `>`, `&`, `*`, `=`, `[`, `]`, `\`, and `|`, which are required for `#include <stdio.h>`, `scanf("%d", &x)`, `int *p`, and `arr[i]`. |
| **BC-12** | `config/templates/fileio.c:25` | Infinite loop on EOF in `fileio.c` | `while (getchar() != '\n');` spins forever if `stdin` reaches `EOF`. |

---

## 3. Deep-Dive Specification: `explain`, `check`, and `submit`

### 3.1 How `explain` Works (`bin/explain` + Integrated into `run` / `crun`)

`explain` is an offline, zero-internet diagnostic translator for compiler errors, warnings, and runtime crashes. Students invoke it via `explain lab1.c` (or `explain` with no arguments to analyze the last edited file), and `run` invokes it whenever compilation or execution fails.

#### Pipeline Architecture
1. **Stage 1: Static Pre-Scan & Compiler Diagnostic Capture**
   - Invokes `clang` / `clang++` with `-Wall -Wextra -Wformat -Wuninitialized -fsyntax-only -fno-color-diagnostics` and captures `stderr` in structured `file:line:col: severity: message` format.
   - Extracts the offending source line and the preceding line from the student's `.c` / `.cpp` file.
2. **Stage 2: Pattern-Matching Rule Engine (25+ Student Error Signatures)**
   - Matches compiler output and source context against common beginner mistakes and prints:
     - **Where:** File name, line number, and visual caret pointing at the token.
     - **What Happened (Plain English):** A clear explanation without compiler jargon.
     - **How to Fix (Before vs After):** Concrete code example showing the correction.
   - **Covered Signatures:**
     1. `expected ';' after expression` -> Points to the exact previous line where `;` was forgotten.
     2. `format '%d' expects argument of type 'int *', but argument has type 'int'` -> Explains that `scanf` needs the memory address `&var`, whereas `variable` alone passes its value.
     3. `using the result of an assignment as a condition without parentheses` -> Explains `if (x = 5)` (assigns 5 to x) vs `if (x == 5)` (compares x to 5).
     4. `use of undeclared identifier 'x'` -> Checks for case-sensitivity typos (`Count` vs `count`) or missing header includes (`printf` -> `<stdio.h>`, `malloc` -> `<stdlib.h>`, `sqrt`/`pow` -> `<math.h>`, `strlen`/`strcpy` -> `<string.h>`, `bool` -> `<stdbool.h>`).
     5. `implicit declaration of function 'gets'` -> Explains why `gets()` was removed from C11 and shows `fgets(str, sizeof(str), stdin)`.
     6. `variable 'sum' is uninitialized when used here` -> Explains garbage values in C stack variables (`int sum = 0;`).
     7. `array index X is past the end of the array` -> Explains 0-based indexing (`int arr[5]` has valid indices `0..4`, so `arr[5]` is out of bounds).
     8. `control reaches end of non-void function` -> Explains missing `return 0;` at the end of `int main()` or helper functions.
     9. `redefinition of 'x'` / `conflicting types for 'func'` -> Explains duplicate variable declarations or mismatched function prototypes.
     10. `undefined reference to 'main'` -> File has no `int main()` entry point or misspelled `mian()`.
     11. `expected '}'` / `to match this '{'` -> Shows the line number of the unclosed opening brace `{`.
     12. `character constant too long for its type` -> Explains single quotes `'a'` (single `char`) vs double quotes `"hello"` (char array / string).
3. **Stage 3: Runtime Crash Forensics (When Compilation Succeeds but Program Crashes)**
   - If the program compiles cleanly, `explain` compiles it with `-g -O0` and runs it under a crash catcher (using `gdb --batch -ex "run" -ex "bt" -ex "info locals"` if `gdb` is installed, or signal exit code decoding):
     - **Exit 139 (`SIGSEGV` - Segmentation Fault):** Prints the exact crashing line number, the offending pointer/array expression, and the local variable values at the moment of crash (e.g., `Line 18: arr[i] crashed because i = 100000 (did you forget & in scanf?)`).
     - **Exit 136 (`SIGFPE` - Floating Point Exception):** Identifies division or modulo by zero (`a / b` where `b == 0`) with line number.
     - **Exit 134 (`SIGABRT` - Aborted):** Identifies double `free()`, invalid pointer `free()`, or stack smashing (`strcpy` overflow).

---

### 3.2 How `check` Works (`bin/check` + Upgraded `bin/ctest`)

`check` is a pre-submission safety verifier and automated test runner. Students run `check lab1.c` (or `check lab1.c test.txt`).

#### 4-Layer Verification Pipeline
1. **Layer 1: Static Bug & Code Smell Audit**
   - Compiles with strict flags (`-Wall -Wextra -Werror=format -Werror=uninitialized -Werror=return-type`).
   - Scans source for beginner traps:
     - `scanf("%d", x)` without `&`
     - `fflush(stdin)` (undefined behavior in standard C)
     - `void main()` (advises `int main(void)` for standard compliance)
     - `gets()` usage
     - Missing `free()` when `malloc()`/`calloc()` is present
2. **Layer 2: Runtime Memory & Undefined Behavior Sanitizer**
   - Checks if Clang's AddressSanitizer / UndefinedBehaviorSanitizer (`-fsanitize=address,undefined`) is supported on the current target; if not (on stripped mobile toolchains), compiles with `-fstack-protector-all -D_FORTIFY_SOURCE=2 -g -O0`.
   - Catches signed integer overflow, null pointer dereference, stack buffer overflow, and out-of-bounds array access.
3. **Layer 3: Infinite Loop Timeout Guard**
   - Executes the binary under a strict watchdog timer (default `5 seconds`, configurable via `-t <sec>`).
   - If the program does not finish within 5 seconds (and is not waiting on interactive TTY input), `check` terminates the process (`SIGTERM`/`SIGKILL`), prints the last lines of output, and warns:
     `TIMED OUT after 5s: Your program appears stuck in an infinite loop. Check your while() or for() loop conditions and make sure your loop counter updates.`
4. **Layer 4: Multi-Case Input/Output Test Runner**
   - Supports three modes:
     - **Interactive Dry Run (`check lab1.c`):** Runs static audit + compiles + runs with timeout and crash guard.
     - **Two-File Mode (`check lab1.c input.txt expected.txt`):** Compares actual output against `expected.txt`.
     - **Multi-Case Suite Mode (`check lab1.c tests.txt`):** Supports multiple test blocks in one file:
       ```text
       --- INPUT ---
       5 10
       --- EXPECTED ---
       15
       --- INPUT ---
       -3 8
       --- EXPECTED ---
       5
       ```
   - Prints a per-case scorecard (`Case 1: PASS (4ms)`, `Case 2: FAIL - Expected '5', got '11'`) and summary (`2/2 Passed`).

---

### 3.3 How `submit` Works (`bin/submit`)

`submit` turns any `.c` or `.cpp` program into a ready-to-print, ready-to-upload college lab record in both **`.doc` (Microsoft Word / WPS Office / Google Docs)** and **`.pdf` (Adobe PDF 1.4)** formats inside `/sdcard/Download/ByteC/`.

#### Step-by-Step Execution Flow
1. **Step 1: Storage Permission Verification & Auto-Repair**
   - On Android Termux (`IS_TERMUX=1`):
     - Checks whether `/sdcard/Download` exists and is writable (`[ -d "/sdcard/Download" ] && [ -w "/sdcard/Download" ]` and attempts a test touch file `/sdcard/Download/.bytec_perm_test`).
     - If permission is missing:
       1. Prints a clear guide: `"ByteC needs Android storage permission to save your files in Downloads/ByteC."`
       2. Runs `termux-setup-storage`.
       3. Prompts: `"Tap 'Allow' on the Android popup screen, then press Enter here to continue..."`
       4. Polls for up to 15 seconds until `/sdcard/Download` becomes writable.
       5. If the user denies permission, saves a fallback copy in `~/c_projects/submissions/` and prints instructions to run `termux-setup-storage` later.
     - Creates `/sdcard/Download/ByteC/` (`mkdir -p "/sdcard/Download/ByteC"`).
   - On Standard Linux (Desktop/Laptop):
     - Resolves `${XDG_DOWNLOAD_DIR:-$HOME/Downloads}/ByteC`, verifies write permissions, and creates the directory.

2. **Step 2: Student Profile & Lab Metadata Collection**
   - Checks `~/.bytec/student_profile.conf` for saved student details (`STUDENT_NAME`, `ROLL_NO`, `COURSE_SECTION`).
   - On first run of `submit`, asks once:
     - `Your Full Name (saved for future labs):`
     - `Roll Number / Registration ID:`
     - `Class / Batch (optional):`
   - Asks for current practical metadata (or reads from top comments `@aim:` / `@title:` if present in the `.c` file):
     - `Experiment / Practical Title (Enter for filename):`
     - `Aim / Problem Statement (1 line, optional):`

3. **Step 3: Compile & Interactive Execution Capture (With Visible Input Echo)**
   - Compiles the program with Clang. If compilation fails, halts and runs `explain` so the student does not submit broken code.
   - Runs the binary to capture terminal output:
     - If `-i input.txt` is provided, runs the program while echoing each input value next to its prompt so the output box looks like a real terminal session (`Enter number: 29` instead of `Enter number: \nPrime`).
     - If run interactively, uses `script -q` (or an interactive input-echo wrapper) so the student types their test inputs live on screen and the entire terminal transcript (prompts + typed inputs + output) is captured verbatim.

4. **Step 4: Dual-Format Document Generation (`.doc` + `.pdf`)**
   - **Output File 1: `/sdcard/Download/ByteC/<basename>_LabReport.doc`**
     - Generates a standalone **Rich Text Format (RTF 1.5)** document with `.doc` extension (and Word XML compatibility headers).
     - Opens natively in Microsoft Word, Android WPS Office, Google Docs, Collabora Office, and LibreOffice with zero external packages.
     - Contains:
       - Clean header block (Student Name, Roll Number, Date, Practical Title, Aim).
       - **Source Code Section:** Formatted in `Courier New` 10pt inside a bordered light-gray box with line numbers.
       - **Terminal Output Section:** Formatted in `Courier New` 10pt with white/green text on a dark `#101018` terminal background box showing exact execution transcript.
   - **Output File 2: `/sdcard/Download/ByteC/<basename>_LabReport.pdf`**
     - Uses a built-in, zero-dependency **POSIX `awk` PDF 1.4 Generator** (using standard PDF Base14 Type1 fonts: `Helvetica`, `Helvetica-Bold`, `Courier`, `Courier-Bold` with automatic line wrapping, multi-page pagination, bordered code boxes, dark terminal output rectangle, and exact byte-offset `xref` table).
     - Requires zero external packages (no 2GB TeXLive, no headless Chromium, works 100% offline in 120MB Termux).

5. **Step 5: Confirmation & Android Notification**
   - Prints exact file paths and sizes:
     - `/sdcard/Download/ByteC/lab1_LabReport.pdf`
     - `/sdcard/Download/ByteC/lab1_LabReport.doc`
   - Fires `termux-notification --title "ByteC Lab Report Ready" --content "Saved lab1_LabReport.pdf & .doc in Downloads/ByteC"`.

---

## 4. Beginner Documentation & Web Guide Overhaul (`docs/` & `README.md`)

### 4.1 Writing Style: Zero AI Slop + Beej's Guide Humor
- Eliminate all corporate filler, em-dashes, adverbs, and generic marketing copy.
- Write for a student who has never seen a terminal before:
  - Explain concepts using grounded, relatable analogies and dry programmer humor in the spirit of *Beej's Guide to C Programming*:
    - **What is a Terminal?** (Text messages with your phone's CPU, without the icons).
    - **What is a Compiler?** (Why your phone cannot read `printf` until Clang translates it into machine binary).
    - **The `scanf` Ampersand (`&`) Trap:** Why forgetting `&` in `scanf("%d", &age)` tells C to write the user's input into a random memory address, summoning the `Segmentation fault (core dumped)` Rite of Passage.
    - **How Not to Get Trapped in Neovim:** Clear visual diagram of `Normal Mode` (navigation/commands) vs `Insert Mode` (typing code), so nobody has to reboot their phone to exit the editor.
    - **Reading Compiler Errors Without Crying:** Why a missing `;` on Line 9 makes Clang complain about Line 10.

### 4.2 Curated Learning Hub (Web Docs + `bytec learn` / `bytec resources`)
Add a dedicated **"Where to Learn More (Free Books, Courses & Practice)"** section in `docs/index.html`, `README.md`, and `bytec resources`:

1. **C Programming (Zero to Hero)**
   - **Beej's Guide to C Programming** (`https://beej.us/guide/bgc/`) - Conversational, witty, and complete C reference for beginners.
   - **Learn-C.org** (`https://www.learn-c.org/`) - Bite-sized interactive C lessons from variables to pointers and linked lists.
   - **Harvard CS50x (Weeks 1-5)** (`https://cs50.harvard.edu/x/`) - Visual lectures on C, memory, pointers, and data structures.
   - **Comp.lang.c FAQ** (`https://c-faq.com/`) - Classic answers to every tricky pointer, array, and `scanf` question.
2. **C++ & Data Structures / Algorithms (DSA)**
   - **LearnCPP.com** (`https://www.learncpp.com/`) - Gold-standard structured C++20 tutorial for students.
   - **VisuAlgo** (`https://visualgo.net/`) - Step-by-step visual animations for Linked Lists, Stacks, Queues, Trees, and Sorting on mobile browsers.
   - **CP-Algorithms** (`https://cp-algorithms.com/`) - Deep-dive explanations of data structures and graph/tree algorithms.
   - **cppreference.com** (`https://en.cppreference.com/`) - Technical reference for C and C++ standard library headers.
3. **Linux, Terminal & Git for Beginners**
   - **Linux Journey** (`https://linuxjourney.com/`) - Grasshopper-level lessons on files, permissions, processes, and the shell.
   - **MIT Missing Semester** (`https://missing.csail.mit.edu/`) - Practical guide to the shell, Vim/Neovim, debugging, and Git.
   - **OverTheWire: Bandit** (`https://overthewire.org/wargames/bandit/`) - Learn Linux terminal commands by playing a beginner SSH game.
   - **ExplainShell** (`https://explainshell.com/`) - Paste any Linux command to see a diagram of what each flag does.
4. **Termux & Neovim Mastery**
   - **Termux Official Wiki** (`https://wiki.termux.com/wiki/Main_Page`) - Guide to Termux storage, touch keys, and package management.
   - **OpenVim Interactive Tutorial** (`https://www.openvim.com/`) - 15-minute interactive practice for Vim/Neovim movement keys.

---

## 5. Complete Phase-by-Phase Implementation Roadmap

### Phase 1: Installer, Storage Guard & Shell Hardening (`ByteC`)
- **Files:** `install.sh`, `shell/prompt.sh`, `shell/aliases.sh`, `config/termux.properties`, `config/clang-format`
- **Tasks:**
  1. Show live package installation progress instead of silent `/dev/null` redirection.
  2. Add interactive storage permission check loop (`termux-setup-storage` + verification of `/sdcard/C_Projects` and `/sdcard/Download/ByteC`).
  3. Install `termux-api` on Termux and deploy default `.clang-format` (`BreakBeforeBraces: Allman, IndentWidth: 4`).
  4. Fix `shell/prompt.sh` for both Bash and Zsh (`ZSH_VERSION` vs `BASH_VERSION`).
  5. Source `~/.bashrc` from `~/.bash_profile` / `~/.profile` if missing.
  6. Update `config/termux.properties` with complete C/C++ symbol touch bar (`#`, `<`, `>`, `&`, `*`, `=`, `{`, `}`, `(`, `)`, `;`, `"`, plus long-press `[`, `]`, `\`, `|`).
  7. Trigger `termux-notification` on install/update completion.

### Phase 2: Beginner CLI Suite (`explain`, `check`, `submit`, `code`, `run`, `fix`, `learn`, `bytec`)
- **Files:** `bin/bytec`, `bin/cnew`, `bin/crun`, `bin/cbuild`, `bin/cformat`, `bin/cdebug`, `bin/ctest`, `bin/explain`, `bin/submit`, `bin/code`, `bin/run`, `bin/check`, `bin/fix`, `bin/learn`
- **Tasks:**
  1. Implement `bin/explain` (25+ C/C++ compiler error patterns + runtime `SIGSEGV`/`SIGFPE`/`SIGABRT` crash forensics).
  2. Implement `bin/check` and upgrade `bin/ctest` (static bug scan + sanitizer/stack-protector + 5s infinite-loop watchdog + multi-case test runner).
  3. Implement `bin/submit` (Android `/sdcard/Download/ByteC` permission verification + student profile cache + interactive input-echo capture + zero-dependency `.doc` RTF and `.pdf` PDF 1.4 generators + `termux-notification`).
  4. Upgrade `bin/crun` (`run` alias, last-edited file memory `~/.bytec/.last_file`, safe argument handling, auto-calls `explain` on compile/crash error).
  5. Upgrade `bin/cnew` (`code` alias, interactive file/template picker when run with no args, saves `.last_file`).
  6. Fix `bin/cbuild` to detect multiple `main()` files in the same folder and prompt the user which file to build.
  7. Add interactive TUI Studio menu to `bin/bytec` (`bytec` with no args) + `bytec resources` + `bytec learn` (interactive C challenges).

### Phase 3: Template Library Expansion & Bug Fixes (`config/templates/*`)
- **Files:** `config/templates/*`
- **Tasks:**
  1. Fix `fileio.c` EOF infinite loop.
  2. Add new templates: `recursion.c`, `structures.c`, `searching.c`, `doublylinkedlist.c`, `graph.c`, `quicksort.c`, `stl.cpp`, `oop.cpp`.

### Phase 4: Neovim v2.0 VS Code Emulation (`0xOpCode/nvim`)
- **Files:** `/root/.config/nvim/lua/config/options.lua`, `keymaps.lua`, `lua/plugins/lsp.lua`, `snacks.lua`, `treesitter.lua`, `ui.lua`
- **Tasks:**
  1. Fix `Ctrl+C` (copy), `Ctrl+V` (paste), `Ctrl+X` (cut), `Ctrl+A` (select all), `Ctrl+/` (`<C-_>` toggle comment), `Ctrl+D` (word select), `Ctrl+Z` / `Ctrl+Y` (undo/redo), and Shift+Arrow selection.
  2. Pass explicit Allman style arguments to `conform.nvim` `clang-format` so `Ctrl+S` and `<Space>r` preserve Allman braces.
  3. Fix `<Space>r` / `<F5>` runner to compile into `$TMPDIR`, run `explain` on failure, and read final pause from `/dev/tty` (`read -r _ </dev/tty`) so `scanf` never closes the window early.
  4. Update Snacks dashboard to **ByteC Studio v2.0** with beginner action keys.
  5. Add Neovim 0.11 `vim.lsp.config` support, Termux clipboard integration, and trimmed Treesitter parser list.

### Phase 5: Beginner Documentation, Beej-Style Guide & Web Hub (`README.md`, `docs/*`, `VERSION`)
- **Files:** `VERSION` (`2.0.0`), `README.md`, `docs/index.html`, `docs/style.css`, `docs/app.js`
- **Tasks:**
  1. Bump `VERSION` to `2.0.0`.
  2. Rewrite `README.md` and `docs/index.html` with zero AI slop, beginner-first explanations, Beej-style humor, visual diagrams for Neovim modes & touch bar, full documentation for `code`, `run`, `explain`, `check`, `submit`, `learn`, and the curated **C / C++ / Linux / Termux Learning Resource Hub**.
