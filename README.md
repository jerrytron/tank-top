Tank Top README

**BUILDING**
make PRODUCT_FIRMWARE_VERSION=100 APP=tank-top TARGET_DIR=applications/tank-top/build DEBUG_BUILD=y v=1

**UPLOADING FIRMWARE**
dfu-util -d 1d50:607f -a 0 -s 0x08005000:leave -D tank-top.bin