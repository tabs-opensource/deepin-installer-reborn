#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Entry point for hook scripts.
# This script setups variables and function used in hook script.
# Also handles chroot environment.

# Folder path of hooks.
HOOKS_DIR=/tmp/installer

. "${HOOKS_DIR}/basic_utils.sh"

# Check arguments
if [ $# -lt 1 ]; then
  error "Usage: $0 hook-file"
fi

# Absolute path of hook_manager.sh in chroot env.
# This path is defined in service/backend/hooks_pack.cpp.
_SELF="${HOOKS_DIR}/hook_manager.sh"
_HOOK_FILE=$1
_IN_CHROOT=$2

# Defines absolute path to oem folder.
# /tmp/oem is reserved for debug.
if [ -d /tmp/oem ]; then
  # Debug mode
  OEM_DIR=/tmp/oem
elif [ -d /media/cdrom/oem ]; then
  # chroot mode
  OEM_DIR=/media/cdrom/oem
fi

# Mark $OEM_DIR as readonly constant.
readonly OEM_DIR

# Run hook file
case ${_HOOK_FILE} in
  */in_chroot/*)
    if [ "x${_IN_CHROOT}" = "xtrue" ]; then
      # Already in chroot env.
      # Host device is mounted at /target/deepinhost
      CONF_FILE="/deepinhost${CONF_FILE}"
      if [ ! -f "${CONF_FILE}" ]; then
        error "Config file ${CONF_FILE} does not exists."
      fi
      . "${_HOOK_FILE}"
      exit $?
    else
      # Switch to chroot env.
      chroot /target "${_SELF}" "${_HOOK_FILE}" 'true'
      exit $?
    fi
    ;;
  *)
    # Run normal hooks.
    if [ ! -f "${CONF_FILE}" ]; then
      error "Config file ${CONF_FILE} does not exists."
    fi
    . "${_HOOK_FILE}"
    exit $?
    ;;
esac
