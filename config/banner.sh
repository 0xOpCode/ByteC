#!/bin/sh
# ==============================================================================
# ByteC: Startup Terminal Banner
# ==============================================================================

CYAN="$(printf '\033[0;36m')"
GREEN="$(printf '\033[0;32m')"
GRAY="$(printf '\033[0;90m')"
NC="$(printf '\033[0m')"

printf "%s\n" "${CYAN}"
cat << 'EOF'
  ____        _       ____
 | __ ) _   _| |_ ___/ ___|
 |  _ \| | | | __/ _ \ |
 | |_) | |_| | ||  __/ |___
 |____/ \__, |\__\___|\____|
        |___/
EOF
printf "%s\n" "${GREEN} ByteC v2.0: Pocket C and C++ Studio for Android Termux${NC}"
printf "%s\n\n" "${GRAY} code <file.c>  run  explain  check  submit  learn  bytec${NC}"
