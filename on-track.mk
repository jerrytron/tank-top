# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Add include paths.
INCLUDE_DIRS += applications/on-track/inc
INCLUDE_DIRS += applications/on-track/lib/AdafruitNeopixel/inc
INCLUDE_DIRS += applications/on-track/lib/ElapsedTime/inc
INCLUDE_DIRS += applications/on-track/lib/Flashee/inc
INCLUDE_DIRS += applications/on-track/lib/SparkButton/inc
INCLUDE_DIRS += applications/on-track/lib/Tone/inc

# C source files included in this build.
CSRC +=

# C++ source files included in this build.
CPPSRC += $(call target_files,./applications/on-track/,*.cpp)

# ASM source files included in this build.
ASRC +=