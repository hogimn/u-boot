#
# (C) Copyright 2014
#
# Gemini IT <gemini525@nate.com>
#
#
#
# MDS2450 has 1 bank of 64 MB DRAM
# 3000'0000 to 3400'0000 : TEXT_BASE=0x33e00000
#
# Linux-Kernel is expected to be at 3000'8000, entry 3000'8000
# optionally with a ramdisk at 3080'0000
#
# we load ourself to 33e0'0000 without MMU
# with MMU, load address is changed to 0xc3e0_0000
#
# download area is 3000'0000
#


ifndef TEXT_BASE
#TEXT_BASE = 0xc3e00000
TEXT_BASE = 0x33000000
endif

