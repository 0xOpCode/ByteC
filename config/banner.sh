#!/bin/sh
# ==============================================================================
# ByteC: Startup Terminal Banner
# ==============================================================================

CYAN='\033[0;36m'
GREEN='\033[0;32m'
GRAY='\033[0;90m'
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
echo "${GREEN} ByteC: C and C++ Environment for Android Termux${NC}"
echo "${GRAY} cnew <file.c>  crun <file.c>  bytec doctor  bytec help${NC}"
echo ""
