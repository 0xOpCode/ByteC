#!/bin/sh
# ==============================================================================
# ByteC: Productivity Aliases and Update Hook
# ==============================================================================

# Beginner and Power-User Workflow Shortcuts
alias start='bytec'
alias c='code'
alias r='run'
alias cr='crun'
alias cb='cbuild'
alias cf='fix'
alias cdbg='debug'
alias ct='check'
alias sub='submit'
alias exp='explain'
alias nv='nvim'
alias cls='clear'
alias proj='cd ~/c_projects'
alias doc='bytec doctor'
alias sheet='bytec cheatsheet'
alias tpl='bytec template'
alias resources='bytec resources'

# Directory listing shortcuts
alias ll='ls -la'
alias la='ls -A'

# Trigger background silent update check on shell launch
if command -v bytec >/dev/null 2>&1; then
    (bytec check >/dev/null 2>&1 &)
fi
