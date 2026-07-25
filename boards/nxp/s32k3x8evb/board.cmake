# Copyright 2026 NXP
# SPDX-License-Identifier: Apache-2.0

board_runner_args(jlink "--device=S32K358" "--reset-after-load")

# Every J-Link operation on S32K3 must skip SEGGER's on-core ECC RAM init
# (S32K3xx_NoRAMInit.JLinkScript, licensed by SEGGER for internal use only,
# hence not committed). Pick it up automatically when the user has placed it
# in the board support directory.
set(noraminit_script ${CMAKE_CURRENT_LIST_DIR}/support/S32K3xx_NoRAMInit.JLinkScript)
if(EXISTS ${noraminit_script})
  board_runner_args(jlink "--tool-opt=-JLinkScriptFile ${noraminit_script}")
else()
  message(WARNING
    "S32K3xx_NoRAMInit.JLinkScript not found in ${CMAKE_CURRENT_LIST_DIR}/support. "
    "Flashing or debugging this board with J-Link WITHOUT this script corrupts "
    "the running firmware (see the board documentation). Download it from "
    "https://kb.segger.com/NXP_S32K3xx or pass it manually via "
    "--tool-opt=-JLinkScriptFile.")
endif()

include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
