# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Add include paths.
INCLUDE_DIRS += applications/afterglow/inc
INCLUDE_DIRS += applications/afterglow/lib/AdafruitNeopixel/inc
INCLUDE_DIRS += applications/afterglow/lib/BubbleDisplay/inc
INCLUDE_DIRS += applications/afterglow/lib/ElapsedTime/inc
INCLUDE_DIRS += applications/afterglow/lib/SparkButton/inc
INCLUDE_DIRS += applications/afterglow/lib/Flashee/inc

# C source files included in this build.
CSRC +=

# C++ source files included in this build.
CPPSRC += $(call target_files,./applications/afterglow/,*.cpp)

# ASM source files included in this build.
ASRC +=