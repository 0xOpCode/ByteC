#!/bin/sh
# ==============================================================================
# 🚀 ByteC — Fast Minimal Git-Aware Shell Prompt
# Zero bloat, instant rendering, battery friendly
# ==============================================================================

# Parse git branch fast without spawning git status
parse_git_branch() {
    BRANCH=$(git symbolic-ref --short HEAD 2>/dev/null)
    if [ -n "$BRANCH" ]; then
        echo " ( $BRANCH)"
    fi
}

set_bytec_prompt() {
    EXIT_CODE=$?
    
    # Colors
    C_CYAN='\[\033[1;36m\]'
    C_GREEN='\[\033[1;32m\]'
    C_RED='\[\033[1;31m\]'
    C_YELLOW='\[\033[1;33m\]'
    C_GRAY='\[\033[0;90m\]'
    C_RESET='\[\033[0m\]'

    # Arrow color based on last command exit status
    if [ $EXIT_CODE -eq 0 ]; then
        ARROW="${C_GREEN}❯${C_RESET}"
    else
        ARROW="${C_RED}❯${C_RESET}"
    fi

    GIT_INFO=$(parse_git_branch)
    PS1="${C_CYAN}\w${C_YELLOW}${GIT_INFO}${C_RESET} ${ARROW} "
}

PROMPT_COMMAND="set_bytec_prompt"
