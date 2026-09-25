#!/bin/sh
# ==============================================================================
# ByteC: 1-Click C and C++ Development Environment Installer
# Supports: Android Termux (Native) and Linux
# ==============================================================================

set -e

# ANSI Color Codes using raw escapes
CYAN="$(printf '\033[1;36m')"
GREEN="$(printf '\033[1;32m')"
YELLOW="$(printf '\033[1;33m')"
RED="$(printf '\033[1;31m')"
NC="$(printf '\033[0m')"

echo "${CYAN}"
cat << 'EOF'
  ____        _       ____ 
 | __ ) _   _| |_ ___/ ___|
 |  _ \| | | | __/ _ \ |    
 | |_) | |_| | ||  __/ |___ 
 |____/ \__, |\__\___|\____|
        |___/               
EOF
echo "${GREEN}Starting ByteC installation...${NC}"
echo ""

# 1. Environment Detection
IS_TERMUX=0
if [ -n "$PREFIX" ] && [ -d "$PREFIX" ] && echo "$PREFIX" | grep -q "com.termux"; then
    IS_TERMUX=1
    echo "${GREEN}📱 Detected Android Termux Environment.${NC}"
    BIN_DIR="$PREFIX/bin"
else
    echo "${GREEN}🐧 Detected Standard Linux Environment.${NC}"
    BIN_DIR="/usr/local/bin"
    [ ! -w "$BIN_DIR" ] && BIN_DIR="${HOME}/.local/bin"
    mkdir -p "$BIN_DIR"
fi

# 2. Package Installation
echo "${YELLOW}📦 [1/6] Installing C/C++ Toolchain & Dependencies...${NC}"
if [ $IS_TERMUX -eq 1 ]; then
    pkg update -y || true
    # Note: clang in Termux includes clang-format. Do not include nonexistent clang-tools.
    pkg install -y git clang make cmake gdb curl neovim bash-completion tar || true

    # Verify essential packages
    for tool in git clang curl tar; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            pkg install -y "$tool" || apt-get install -y "$tool" || true
        fi
    done
else
    if command -v apt-get >/dev/null 2>&1; then
        sudo apt-get update || apt-get update || true
        sudo apt-get install -y clang clang-format make cmake gdb git curl neovim tar || apt-get install -y clang clang-format make cmake gdb git curl neovim tar || true
    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -Sy --noconfirm clang make cmake gdb git curl neovim tar || pacman -Sy --noconfirm clang make cmake gdb git curl neovim tar || true
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y clang clang-tools-extra make cmake gdb git curl neovim tar || dnf install -y clang clang-tools-extra make cmake gdb git curl neovim tar || true
    else
        echo "${YELLOW}⚠️ Warning: Unsupported package manager. Ensure clang, make, and git are installed.${NC}"
    fi
fi

# 3. Setup ByteC Directory (~/.bytec)
echo "${YELLOW}⚙️ [2/6] Deploying ByteC CLI Tools & Templates...${NC}"
BYTEC_HOME="${HOME}/.bytec"
mkdir -p "$BYTEC_HOME"

SCRIPT_DIR="$(cd "$(dirname "$0")" 2>/dev/null && pwd)"
if [ -f "$SCRIPT_DIR/bin/crun" ]; then
    # Running from local cloned directory (exclude .git)
    mkdir -p "$BYTEC_HOME/bin" "$BYTEC_HOME/config" "$BYTEC_HOME/shell"
    cp -rf "$SCRIPT_DIR/bin/"* "$BYTEC_HOME/bin/"
    cp -rf "$SCRIPT_DIR/config/"* "$BYTEC_HOME/config/"
    cp -rf "$SCRIPT_DIR/shell/"* "$BYTEC_HOME/shell/"
    [ -f "$SCRIPT_DIR/VERSION" ] && cp -f "$SCRIPT_DIR/VERSION" "$BYTEC_HOME/"
    [ -f "$SCRIPT_DIR/README.md" ] && cp -f "$SCRIPT_DIR/README.md" "$BYTEC_HOME/"
else
    # Running via curl | bash
    DEPLOYED=0
    if command -v git >/dev/null 2>&1; then
        if [ -d "$BYTEC_HOME/.git" ]; then
            git -C "$BYTEC_HOME" pull --ff-only 2>/dev/null && DEPLOYED=1 || true
        else
            rm -rf "$BYTEC_HOME"
            git clone https://github.com/0xOpCode/ByteC.git "$BYTEC_HOME" 2>/dev/null && DEPLOYED=1 || true
        fi
    fi

    # Fallback to curl archive if git clone failed or git is missing
    if [ $DEPLOYED -eq 0 ] || [ ! -f "$BYTEC_HOME/bin/crun" ]; then
        echo "  • Fetching ByteC archive via curl..."
        rm -rf "$BYTEC_HOME"
        mkdir -p "$BYTEC_HOME"
        curl -sL https://github.com/0xOpCode/ByteC/archive/refs/heads/main.tar.gz | tar -xz -C "$BYTEC_HOME" --strip-components=1 || true
    fi
fi

chmod +x "$BYTEC_HOME"/bin/* "$BYTEC_HOME"/config/*.sh "$BYTEC_HOME"/shell/*.sh 2>/dev/null || true

# Symlink CLI commands to BIN_DIR
for cmd in gcc g++ cnew crun cbuild cformat cdebug ctest bytec; do
    if [ -w "$BIN_DIR" ]; then
        ln -sf "$BYTEC_HOME/bin/$cmd" "$BIN_DIR/$cmd"
    else
        mkdir -p "${HOME}/.local/bin"
        ln -sf "$BYTEC_HOME/bin/$cmd" "${HOME}/.local/bin/$cmd"
    fi
done

# 4. Configure Termux Touch UI & Hide MOTD
echo "${YELLOW}📱 [3/6] Configuring Termux Touch Keys & OLED Palette...${NC}"
touch "${HOME}/.hushlogin"

if [ $IS_TERMUX -eq 1 ]; then
    mkdir -p "${HOME}/.termux"
    cp -f "$BYTEC_HOME/config/termux.properties" "${HOME}/.termux/termux.properties"
    cp -f "$BYTEC_HOME/config/colors.properties" "${HOME}/.termux/colors.properties"
    command -v termux-reload-settings >/dev/null 2>&1 && termux-reload-settings || true
fi

# 5. Deploy Neovim C IDE Stack (0xOpCode/nvim)
echo "${YELLOW}💡 [4/6] Setting Up Neovim C IDE Stack (0xOpCode/nvim)...${NC}"
NVIM_DIR="${HOME}/.config/nvim"

NVIM_DEPLOYED=0
if command -v git >/dev/null 2>&1; then
    if [ -d "$NVIM_DIR/.git" ]; then
        echo "  • Existing 0xOpCode Neovim repo found, pulling latest..."
        git -C "$NVIM_DIR" pull --ff-only 2>/dev/null && NVIM_DEPLOYED=1 || true
    else
        if [ -d "$NVIM_DIR" ]; then
            BACKUP_NVIM="${HOME}/.config/nvim.backup.$(date +%s)"
            echo "  • Backing up old nvim config to $BACKUP_NVIM..."
            mv "$NVIM_DIR" "$BACKUP_NVIM"
        fi
        mkdir -p "${HOME}/.config"
        git clone https://github.com/0xOpCode/nvim.git "$NVIM_DIR" 2>/dev/null && NVIM_DEPLOYED=1 || true
    fi
fi

# Fallback if git is not available or clone failed
if [ $NVIM_DEPLOYED -eq 0 ] && [ ! -d "$NVIM_DIR/lua" ]; then
    echo "  • Fetching Neovim configuration via curl..."
    mkdir -p "$NVIM_DIR"
    curl -sL https://github.com/0xOpCode/nvim/archive/refs/heads/main.tar.gz | tar -xz -C "$NVIM_DIR" --strip-components=1 || true
fi

if command -v nvim >/dev/null 2>&1; then
    echo "  • Syncing Neovim plugins in background..."
    nvim --headless "+Lazy! sync" +qa >/dev/null 2>&1 || true
fi

# 6. Shared Storage Link (Termux Phone Storage)
echo "${YELLOW}📂 [5/6] Setting Up C Projects Workspace...${NC}"
if [ $IS_TERMUX -eq 1 ]; then
    echo "  • Requesting phone storage permission (Tap 'Allow' on Android dialog if prompted)..."
    command -v termux-setup-storage >/dev/null 2>&1 && termux-setup-storage || true
    
    sleep 1
    
    if [ -d "/sdcard" ]; then
        mkdir -p "/sdcard/C_Projects" 2>/dev/null || true
        ln -sfn "/sdcard/C_Projects" "${HOME}/c_projects" 2>/dev/null || mkdir -p "${HOME}/c_projects"
        echo "  • Linked ~/c_projects -> /sdcard/C_Projects (Safe from app uninstall)"
    else
        mkdir -p "${HOME}/c_projects"
    fi
else
    mkdir -p "${HOME}/c_projects"
fi

# 7. Shell Integration (.bashrc / .zshrc)
echo "${YELLOW}🐚 [6/6] Integrating Shell Helpers & Prompt...${NC}"

setup_rc() {
    RC_FILE="$1"
    [ ! -f "$RC_FILE" ] && touch "$RC_FILE"

    if [ "$BIN_DIR" = "${HOME}/.local/bin" ]; then
        if ! grep -q 'PATH.*\.local/bin' "$RC_FILE"; then
            echo 'export PATH="${HOME}/.local/bin:$PATH"' >> "$RC_FILE"
        fi
    fi

    # Remove old ByteC markers
    sed -i '/# >>> ByteC Integration >>>/,/# <<< ByteC Integration <<</d' "$RC_FILE" 2>/dev/null || true

    # Append fresh ByteC block
    cat << 'EOF' >> "$RC_FILE"
# >>> ByteC Integration >>>
[ -f "$HOME/.bytec/config/banner.sh" ] && "$HOME/.bytec/config/banner.sh"
[ -f "$HOME/.bytec/shell/aliases.sh" ] && . "$HOME/.bytec/shell/aliases.sh"
[ -f "$HOME/.bytec/shell/prompt.sh" ] && . "$HOME/.bytec/shell/prompt.sh"
# <<< ByteC Integration <<<
EOF
}

setup_rc "${HOME}/.bashrc"
[ -f "${HOME}/.zshrc" ] && setup_rc "${HOME}/.zshrc"

date +%s > "$BYTEC_HOME/.last_check" 2>/dev/null || true

echo ""
echo "${GREEN}================================================================${NC}"
echo "${GREEN}ByteC Installation Complete${NC}"
echo "${GREEN}================================================================${NC}"
echo ""

# Run doctor check
if [ -x "$BYTEC_HOME/bin/bytec" ]; then
    "$BYTEC_HOME/bin/bytec" doctor
fi

echo ""
echo "Quick Start:"
echo "  1. Reload shell:               ${CYAN}source ~/.bashrc${NC}"
echo "  2. Interactive tutorial:       ${CYAN}bytec tutorial${NC}"
echo "  3. Create your first program:  ${CYAN}cnew lab1.c${NC}"
echo "  4. Compile and run:            ${CYAN}crun lab1.c${NC}"
echo ""
echo "Maintained by 0xOpCode"
