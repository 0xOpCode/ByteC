#!/bin/sh
# ==============================================================================
# ByteC: Minimal Git-Aware Shell Prompt (Bash and Zsh)
# ==============================================================================

parse_git_branch() {
    BRANCH=$(git symbolic-ref --short HEAD 2>/dev/null)
    if [ -z "$BRANCH" ]; then
        BRANCH=$(git rev-parse --short HEAD 2>/dev/null)
    fi
    if [ -n "$BRANCH" ]; then
        printf " (%s)" "$BRANCH"
    fi
}

set_bytec_prompt() {
    EXIT_CODE=$?

    C_CYAN='\[\033[1;36m\]'
    C_YELLOW='\[\033[1;33m\]'
    C_GREEN='\[\033[1;32m\]'
    C_RED='\[\033[1;31m\]'
    C_RESET='\[\033[0m\]'

    if [ "$EXIT_CODE" -eq 0 ]; then
        ARROW="${C_GREEN}❯${C_RESET}"
    else
        ARROW="${C_RED}❯${C_RESET}"
    fi

    GIT_INFO=$(parse_git_branch)
    PS1="${C_CYAN}\w${C_YELLOW}${GIT_INFO}${C_RESET} ${ARROW} "
}

set_bytec_zsh_prompt() {
    EXIT_CODE=$?
    GIT_INFO=$(parse_git_branch)
    if [ "$EXIT_CODE" -eq 0 ]; then
        PROMPT="%B%F{cyan}%~%f%F{yellow}${GIT_INFO}%f %F{green}❯%f%b "
    else
        PROMPT="%B%F{cyan}%~%f%F{yellow}${GIT_INFO}%f %F{red}❯%f%b "
    fi
}

if [ -n "$ZSH_VERSION" ]; then
    eval 'case " ${precmd_functions[*]} " in *" set_bytec_zsh_prompt "*) ;; *) precmd_functions+=(set_bytec_zsh_prompt) ;; esac'
elif [ -n "$BASH_VERSION" ]; then
    PROMPT_COMMAND="set_bytec_prompt"
else
    PROMPT_COMMAND="set_bytec_prompt"
fi
