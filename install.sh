#!/bin/sh
# ==============================================================================
# ByteC: 1-Click C and C++ Development Environment Installer
# Supports: Android Termux (Native) and Linux
# ==============================================================================

set -e

# ANSI Color Codes
CYAN="$(printf '\033[1;36m')"
GREEN="$(printf '\033[1;32m')"
YELLOW="$(printf '\033[1;33m')"
RED="$(printf '\033[1;31m')"
NC="$(printf '\033[0m')"

SUCCESS=0
cleanup_on_exit() {
    EXIT_CODE=$?
    if [ "$SUCCESS" -ne 1 ] && [ "$EXIT_CODE" -ne 0 ]; then
        echo ""
        echo "${RED}Installation stopped at line $LINENO with exit code $EXIT_CODE.${NC}"
        echo "${YELLOW}Troubleshooting steps:${NC}"
        if [ "$IS_TERMUX" -eq 1 ]; then
            echo "  • Mirror issues: Run ${CYAN}termux-change-repo${NC} and choose a working mirror."
            echo "  • Refresh packages: Run ${CYAN}pkg update -y${NC}"
        else
            echo "  • Check network connectivity and package manager permissions."
        fi
        echo "  • Re-run installer: ${CYAN}curl -sL https://raw.githubusercontent.com/0xOpCode/ByteC/main/install.sh | bash${NC}"
    fi
}
trap cleanup_on_exit EXIT

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
echo "${YELLOW}📦 [1/6] Installing C/C++ Toolchain & Core Utilities...${NC}"

install_tool() {
    TOOL_NAME="$1"
    CMD_NAME="${2:-$1}"
    if command -v "$CMD_NAME" >/dev/null 2>&1; then
        echo "  • $TOOL_NAME: Found"
        return 0
    fi

    echo "  • Installing $TOOL_NAME..."
    if [ "$IS_TERMUX" -eq 1 ]; then
        export DEBIAN_FRONTEND=noninteractive
        pkg install -y -o Dpkg::Options::="--force-confnew" "$TOOL_NAME" >/dev/null 2>&1 || \
        apt-get install -y -o Dpkg::Options::="--force-confnew" "$TOOL_NAME" >/dev/null 2>&1 || return 1
    elif command -v apt-get >/dev/null 2>&1; then
        sudo apt-get install -y "$TOOL_NAME" >/dev/null 2>&1 || apt-get install -y "$TOOL_NAME" >/dev/null 2>&1 || return 1
    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -S --noconfirm "$TOOL_NAME" >/dev/null 2>&1 || pacman -S --noconfirm "$TOOL_NAME" >/dev/null 2>&1 || return 1
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y "$TOOL_NAME" >/dev/null 2>&1 || dnf install -y "$TOOL_NAME" >/dev/null 2>&1 || return 1
    fi
    return 0
}

# Update package repository index
if [ "$IS_TERMUX" -eq 1 ]; then
    echo "  • Refreshing Termux package repository..."
    pkg update -y >/dev/null 2>&1 || apt-get update -y >/dev/null 2>&1 || true
fi

# Essential tools for native Termux: clang, make, cmake, git, curl, tar
for tool in curl tar git clang make cmake; do
    install_tool "$tool" "$tool" || true
done

# Validate essential tools
MISSING_CORE=""
for tool in curl tar git clang make cmake; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        install_tool "$tool" "$tool" || true
        if ! command -v "$tool" >/dev/null 2>&1; then
            MISSING_CORE="$MISSING_CORE $tool"
        fi
    fi
done

if [ -n "$MISSING_CORE" ]; then
    echo "${RED}Critical error: Missing essential package(s):${MISSING_CORE}${NC}"
    if [ "$IS_TERMUX" -eq 1 ]; then
        echo "${YELLOW}Termux package mirror failed. Resolve with:${NC}"
        echo "  1. Run: ${CYAN}termux-change-repo${NC} (Select 'Main repository' and pick a working mirror)"
        echo "  2. Run: ${CYAN}pkg update -y${NC}"
        echo "  3. Run: ${CYAN}pkg install -y${MISSING_CORE}${NC}"
    fi
    exit 1
fi

# Optional tools: Neovim and GDB
for opt in neovim gdb; do
    if ! command -v "$opt" >/dev/null 2>&1; then
        install_tool "$opt" "$opt" || true
    fi
done

# 3. Configure Git Identity
echo "${YELLOW}🔧 [2/6] Configuring Git Identity...${NC}"
if command -v git >/dev/null 2>&1; then
    CURRENT_NAME="$(git config --global user.name 2>/dev/null || true)"
    CURRENT_EMAIL="$(git config --global user.email 2>/dev/null || true)"

    if [ -z "$CURRENT_NAME" ] || [ -z "$CURRENT_EMAIL" ]; then
        # Generate 4-digit identifier
        RANDOM_ID="$(head -c 2 /dev/urandom 2>/dev/null | od -An -tu2 | tr -d ' ')"
        [ -z "$RANDOM_ID" ] && RANDOM_ID="$(date +%s | tail -c 5)"
        [ -z "$RANDOM_ID" ] && RANDOM_ID="1024"

        if [ -z "$CURRENT_NAME" ]; then
            AUTO_NAME="ByteCDev_${RANDOM_ID}"
            git config --global user.name "$AUTO_NAME"
            echo "  • Generated Git user.name: $AUTO_NAME"
        else
            echo "  • Preserved Git user.name: $CURRENT_NAME"
        fi

        if [ -z "$CURRENT_EMAIL" ]; then
            AUTO_EMAIL="dev_${RANDOM_ID}@bytec.local"
            git config --global user.email "$AUTO_EMAIL"
            echo "  • Generated Git user.email: $AUTO_EMAIL"
        else
            echo "  • Preserved Git user.email: $CURRENT_EMAIL"
        fi

        git config --global init.defaultBranch main 2>/dev/null || true
        echo "  • Customization command: git config --global user.name \"Your Name\""
    else
        echo "  • Existing Git identity found: $CURRENT_NAME <$CURRENT_EMAIL>"
    fi
fi

# 4. Setup ByteC Directory (~/.bytec)
echo "${YELLOW}⚙️ [3/6] Deploying ByteC CLI Tools & Templates...${NC}"
BYTEC_HOME="${HOME}/.bytec"
mkdir -p "$BYTEC_HOME"

SCRIPT_DIR="$(cd "$(dirname "$0")" 2>/dev/null && pwd)"
if [ -f "$SCRIPT_DIR/bin/crun" ]; then
    echo "  • Copying files from local repository..."
    mkdir -p "$BYTEC_HOME/bin" "$BYTEC_HOME/config" "$BYTEC_HOME/shell"
    cp -rf "$SCRIPT_DIR/bin/"* "$BYTEC_HOME/bin/"
    cp -rf "$SCRIPT_DIR/config/"* "$BYTEC_HOME/config/"
    cp -rf "$SCRIPT_DIR/shell/"* "$BYTEC_HOME/shell/"
    [ -f "$SCRIPT_DIR/VERSION" ] && cp -f "$SCRIPT_DIR/VERSION" "$BYTEC_HOME/"
    [ -f "$SCRIPT_DIR/README.md" ] && cp -f "$SCRIPT_DIR/README.md" "$BYTEC_HOME/"
else
    DEPLOYED=0
    if command -v git >/dev/null 2>&1; then
        echo "  • Fetching ByteC repository..."
        if [ -d "$BYTEC_HOME/.git" ]; then
            git -C "$BYTEC_HOME" pull --ff-only 2>/dev/null && DEPLOYED=1 || true
        else
            rm -rf "$BYTEC_HOME"
            git clone --depth=1 https://github.com/0xOpCode/ByteC.git "$BYTEC_HOME" 2>/dev/null && DEPLOYED=1 || true
        fi
    fi

    # Fallback to curl archive if git clone failed or git is missing
    if [ "$DEPLOYED" -eq 0 ] || [ ! -f "$BYTEC_HOME/bin/crun" ]; then
        echo "  • Downloading ByteC archive via curl..."
        rm -rf "$BYTEC_HOME"
        mkdir -p "$BYTEC_HOME"
        curl -sL https://github.com/0xOpCode/ByteC/archive/refs/heads/main.tar.gz | tar -xz -C "$BYTEC_HOME" --strip-components=1 2>/dev/null || true
    fi
fi

# Verify deployment succeeded
if [ ! -f "$BYTEC_HOME/bin/crun" ]; then
    echo "${RED}Error: Failed to unpack ByteC files to $BYTEC_HOME.${NC}"
    echo "Check network connection or manual archive extraction:"
    echo "  curl -sL https://github.com/0xOpCode/ByteC/archive/refs/heads/main.tar.gz | tar -xz"
    exit 1
fi

chmod +x "$BYTEC_HOME"/bin/* 2>/dev/null || true
[ -d "$BYTEC_HOME/config" ] && chmod +x "$BYTEC_HOME"/config/*.sh 2>/dev/null || true
[ -d "$BYTEC_HOME/shell" ] && chmod +x "$BYTEC_HOME"/shell/*.sh 2>/dev/null || true

# Symlink CLI commands to BIN_DIR
for cmd in gcc g++ cnew crun cbuild cformat cdebug ctest bytec; do
    if [ -w "$BIN_DIR" ]; then
        ln -sf "$BYTEC_HOME/bin/$cmd" "$BIN_DIR/$cmd"
    else
        mkdir -p "${HOME}/.local/bin"
        ln -sf "$BYTEC_HOME/bin/$cmd" "${HOME}/.local/bin/$cmd"
    fi
done

# 5. Configure Termux Touch UI & Hide MOTD
echo "${YELLOW}📱 [4/6] Configuring Termux Touch Keys & OLED Palette...${NC}"
touch "${HOME}/.hushlogin"

if [ "$IS_TERMUX" -eq 1 ]; then
    mkdir -p "${HOME}/.termux"
    if [ -f "$BYTEC_HOME/config/termux.properties" ]; then
        cp -f "$BYTEC_HOME/config/termux.properties" "${HOME}/.termux/termux.properties"
    fi
    if [ -f "$BYTEC_HOME/config/colors.properties" ]; then
        cp -f "$BYTEC_HOME/config/colors.properties" "${HOME}/.termux/colors.properties"
    fi
    command -v termux-reload-settings >/dev/null 2>&1 && termux-reload-settings >/dev/null 2>&1 || true
fi

# 6. Deploy Neovim C IDE Stack (0xOpCode/nvim)
echo "${YELLOW}💡 [5/6] Setting Up Neovim C IDE Stack...${NC}"
NVIM_DIR="${HOME}/.config/nvim"

if command -v nvim >/dev/null 2>&1; then
    NVIM_DEPLOYED=0
    if command -v git >/dev/null 2>&1; then
        if [ -d "$NVIM_DIR/.git" ]; then
            echo "  • Updating Neovim configuration..."
            git -C "$NVIM_DIR" pull --ff-only 2>/dev/null && NVIM_DEPLOYED=1 || true
        else
            if [ -d "$NVIM_DIR" ]; then
                BACKUP_NVIM="${HOME}/.config/nvim.backup.$(date +%s)"
                echo "  • Moving existing configuration to $BACKUP_NVIM..."
                mv "$NVIM_DIR" "$BACKUP_NVIM"
            fi
            mkdir -p "${HOME}/.config"
            echo "  • Cloning Neovim configuration..."
            git clone --depth=1 https://github.com/0xOpCode/nvim.git "$NVIM_DIR" 2>/dev/null && NVIM_DEPLOYED=1 || true
        fi
    fi

    # Fallback to curl archive if git clone failed
    if [ "$NVIM_DEPLOYED" -eq 0 ] && [ ! -d "$NVIM_DIR/lua" ]; then
        echo "  • Downloading Neovim configuration via curl..."
        mkdir -p "$NVIM_DIR"
        curl -sL https://github.com/0xOpCode/nvim/archive/refs/heads/main.tar.gz | tar -xz -C "$NVIM_DIR" --strip-components=1 2>/dev/null || true
    fi

    if [ -d "$NVIM_DIR/lua" ]; then
        echo "  • Syncing Neovim plugins in background..."
        nvim --headless "+Lazy! sync" +qa >/dev/null 2>&1 || true
    fi
else
    echo "  • Neovim not installed. Nano and vim remain functional editors."
fi

# 7. Workspace Storage & Shell Integration
echo "${YELLOW}📂 [6/6] Finalizing Workspace & Shell Integration...${NC}"

if [ "$IS_TERMUX" -eq 1 ]; then
    if [ ! -d "/sdcard" ] && command -v termux-setup-storage >/dev/null 2>&1; then
        echo "  • Requesting phone storage access..."
        termux-setup-storage </dev/null >/dev/null 2>&1 || true
        sleep 1
    fi

    if [ -d "/sdcard" ]; then
        mkdir -p "/sdcard/C_Projects" 2>/dev/null || true
        ln -sfn "/sdcard/C_Projects" "${HOME}/c_projects" 2>/dev/null || mkdir -p "${HOME}/c_projects"
        echo "  • Linked ~/c_projects -> /sdcard/C_Projects"
    else
        mkdir -p "${HOME}/c_projects"
        echo "  • Created directory ~/c_projects"
    fi
else
    mkdir -p "${HOME}/c_projects"
    echo "  • Created directory ~/c_projects"
fi

setup_rc() {
    RC_FILE="$1"
    [ ! -f "$RC_FILE" ] && touch "$RC_FILE"

    if [ "$BIN_DIR" = "${HOME}/.local/bin" ]; then
        if ! grep -q 'PATH.*\.local/bin' "$RC_FILE" 2>/dev/null; then
            echo 'export PATH="${HOME}/.local/bin:$PATH"' >> "$RC_FILE"
        fi
    fi

    # Remove existing ByteC markers
    sed -i '/# >>> ByteC Integration >>>/,/# <<< ByteC Integration <<</d' "$RC_FILE" 2>/dev/null || true

    # Append ByteC block
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

SUCCESS=1

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
