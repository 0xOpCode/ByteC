#!/bin/sh
# ==============================================================================
# 🛡️ ByteC — 1-Click C/C++ Development Environment Installer
# Supports: Android Termux (Native) & Linux
# ==============================================================================

set -e

# ANSI Color Codes
CYAN='\033[1;36m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
RED='\033[1;31m'
NC='\033[0m' # No Color

echo "${CYAN}"
cat << 'EOF'
  ____        _       ____ 
 | __ ) _   _| |_ ___/ ___|
 |  _ \| | | | __/ _ \ |    
 | |_) | |_| | ||  __/ |___ 
 |____/ \__, |\__\___|\____|
        |___/               
EOF
echo "${GREEN}⚡ Starting ByteC 1-Click Installation...${NC}"
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
    pkg update -y
    pkg install -y clang make cmake gdb git curl neovim bash-completion
else
    if command -v apt-get >/dev/null 2>&1; then
        sudo apt-get update -y || apt-get update -y
        sudo apt-get install -y clang make cmake gdb git curl neovim || apt-get install -y clang make cmake gdb git curl neovim
    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -Sy --noconfirm clang make cmake gdb git curl neovim
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y clang make cmake gdb git curl neovim
    fi
fi

# 3. Setup ByteC Directory (~/.bytec)
echo "${YELLOW}⚙️ [2/6] Deploying ByteC CLI Tools...${NC}"
BYTEC_HOME="${HOME}/.bytec"

# If cloned locally or running from repository folder
SCRIPT_DIR="$(cd "$(dirname "$0")" 2>/dev/null && pwd)"
if [ -f "$SCRIPT_DIR/bin/crun" ]; then
    rm -rf "$BYTEC_HOME"
    cp -r "$SCRIPT_DIR" "$BYTEC_HOME"
else
    # Running via curl | bash
    if [ -d "$BYTEC_HOME/.git" ]; then
        git -C "$BYTEC_HOME" pull --ff-only
    else
        rm -rf "$BYTEC_HOME"
        git clone https://github.com/0xOpCode/ByteC.git "$BYTEC_HOME"
    fi
fi

chmod +x "$BYTEC_HOME"/bin/* "$BYTEC_HOME"/config/*.sh "$BYTEC_HOME"/shell/*.sh

# Symlink CLI commands to BIN_DIR
for cmd in gcc g++ cnew crun bytec; do
    if [ -w "$BIN_DIR" ]; then
        ln -sf "$BYTEC_HOME/bin/$cmd" "$BIN_DIR/$cmd"
    else
        mkdir -p "${HOME}/.local/bin"
        ln -sf "$BYTEC_HOME/bin/$cmd" "${HOME}/.local/bin/$cmd"
    fi
done

# 4. Configure Termux Touch UI & Hide MOTD
echo "${YELLOW}📱 [3/6] Configuring Termux Touch Keys & OLED Palette...${NC}"
# Silence default Termux welcome message
touch "${HOME}/.hushlogin"

if [ $IS_TERMUX -eq 1 ]; then
    mkdir -p "${HOME}/.termux"
    cp -f "$BYTEC_HOME/config/termux.properties" "${HOME}/.termux/termux.properties"
    cp -f "$BYTEC_HOME/config/colors.properties" "${HOME}/.termux/colors.properties"
    # Apply settings immediately
    command -v termux-reload-settings >/dev/null 2>&1 && termux-reload-settings || true
fi

# 5. Deploy Neovim C IDE Stack (0xOpCode/nvim)
echo "${YELLOW}💡 [4/6] Setting Up Neovim C IDE Stack (0xOpCode/nvim)...${NC}"
NVIM_DIR="${HOME}/.config/nvim"

if [ -d "$NVIM_DIR/.git" ]; then
    echo "  • Existing 0xOpCode Neovim repo found, pulling latest..."
    git -C "$NVIM_DIR" pull --ff-only || true
else
    if [ -d "$NVIM_DIR" ]; then
        BACKUP_NVIM="${HOME}/.config/nvim.backup.$(date +%s)"
        echo "  • Backing up old nvim config to $BACKUP_NVIM..."
        mv "$NVIM_DIR" "$BACKUP_NVIM"
    fi
    mkdir -p "${HOME}/.config"
    git clone https://github.com/0xOpCode/nvim.git "$NVIM_DIR"
fi

# Pre-fetch & sync lazy plugins in headless mode
echo "  • Pre-caching Neovim plugins in background..."
nvim --headless "+Lazy! sync" +qa >/dev/null 2>&1 || true

# 6. Shared Storage Link (Termux Phone Storage)
echo "${YELLOW}📂 [5/6] Setting Up C Projects Workspace...${NC}"
if [ $IS_TERMUX -eq 1 ]; then
    # Request Android storage permission
    command -v termux-setup-storage >/dev/null 2>&1 && termux-setup-storage || true
    
    if [ -d "/sdcard" ] && [ -w "/sdcard" ]; then
        mkdir -p "/sdcard/C_Projects"
        ln -sfn "/sdcard/C_Projects" "${HOME}/c_projects"
        echo "  • Linked ~/c_projects -> /sdcard/C_Projects (Safe from app uninstall)"
    else
        mkdir -p "${HOME}/c_projects"
    fi
else
    mkdir -p "${HOME}/c_projects"
fi

# 7. Shell Integration (.bashrc / .zshrc)
echo "${YELLOW}🐚 [6/6] Integrating Shell Helpers & Prompt...${NC}"
BASHRC="${HOME}/.bashrc"
touch "$BASHRC"

# Add PATH if ~/.local/bin was used
if [ "$BIN_DIR" = "${HOME}/.local/bin" ]; then
    if ! grep -q 'PATH.*\.local/bin' "$BASHRC"; then
        echo 'export PATH="${HOME}/.local/bin:$PATH"' >> "$BASHRC"
    fi
fi

# Remove old ByteC markers if any
sed -i '/# >>> ByteC Integration >>>/,/# <<< ByteC Integration <<</d' "$BASHRC" 2>/dev/null || true

# Append clean ByteC integration block
cat << 'EOF' >> "$BASHRC"
# >>> ByteC Integration >>>
[ -f "$HOME/.bytec/config/banner.sh" ] && "$HOME/.bytec/config/banner.sh"
[ -f "$HOME/.bytec/shell/aliases.sh" ] && . "$HOME/.bytec/shell/aliases.sh"
[ -f "$HOME/.bytec/shell/prompt.sh" ] && . "$HOME/.bytec/shell/prompt.sh"
# <<< ByteC Integration <<<
EOF

# Update .last_check timestamp
date +%s > "$BYTEC_HOME/.last_check"

echo ""
echo "${GREEN}================================================================${NC}"
echo "${GREEN}🎉 ByteC Installation Complete!${NC}"
echo "${GREEN}================================================================${NC}"
echo ""
echo "🚀 Quick Start:"
echo "  1. Restart your Termux or run: ${CYAN}source ~/.bashrc${NC}"
echo "  2. Create a new C program:     ${CYAN}cnew lab1.c${NC}"
echo "  3. Compile & run with 1 click: ${CYAN}crun lab1.c${NC} (or inside Neovim press ${CYAN}<Space> + r${NC})"
echo "  4. Check updates in future:    ${CYAN}bytec update${NC}"
echo ""
echo "Happy Coding! 🛡️ Built by 0xOpCode"
