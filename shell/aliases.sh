#!/bin/sh
# ==============================================================================
# ByteC: Productivity Aliases and Update Hook
# ==============================================================================

# Fast C Workflow Shortcuts
alias c='cnew'
alias cr='crun'
alias cb='cbuild'
alias cf='cformat'
alias cdbg='cdebug'
alias ct='ctest'
alias nv='nvim'
alias cls='clear'
alias proj='cd ~/c_projects'
alias doc='bytec doctor'
alias sheet='bytec cheatsheet'
alias tpl='bytec template'

# Modern directory listing
alias ll='ls -la'
alias la='ls -A'

# Trigger background silent update check on shell launch
if command -v bytec >/dev/null 2>&1; then
    (bytec check >/dev/null 2>&1 &)
fi
