#!/bin/sh
# ==============================================================================
# ⚡ ByteC — Productivity Aliases & Auto-Update Daemon Hook
# ==============================================================================

# Quick C Shortcuts
alias c='cnew'
alias cr='crun'
alias nv='nvim'
alias cls='clear'
alias proj='cd ~/c_projects'

# Modern ls shortcuts if available
alias ll='ls -la'
alias la='ls -A'

# Trigger background silent update check on shell launch
if command -v bytec >/dev/null 2>&1; then
    (bytec check >/dev/null 2>&1 &)
fi
