# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Add include paths.
INCLUDE_DIRS += applications/tank-top/inc
INCLUDE_DIRS += applications/tank-top/lib/AdafruitNeopixel/inc
INCLUDE_DIRS += applications/tank-top/lib/ElapsedTime/inc
INCLUDE_DIRS += applications/tank-top/lib/Flashee/inc
INCLUDE_DIRS += applications/tank-top/lib/SparkButton/inc
INCLUDE_DIRS += applications/tank-top/lib/Tone/inc
INCLUDE_DIRS += applications/tank-top/lib/FiniteStateMachine/inc
INCLUDE_DIRS += applications/tank-top/lib/Nunchuk/inc

# C source files included in this build.
CSRC +=

# C++ source files included in this build.
CPPSRC += $(call target_files,./applications/tank-top/,*.cpp)

# ASM source files included in this build.
ASRC +=