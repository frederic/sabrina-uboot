# sabrina-uboot

U-Boot source tree for Chromecast with Google TV target (sabrina).

## branches
- QTS2.200918.025 : imported from original GPL archive
- QTS2.200918.025-USBboot : By default, U-Boot will attempt to boot from USB drive after waiting 10 seconds. If physical button is hold during U-boot booting, USB burning mode is entered

## setup
```
$ export CROSS_COMPILE=.../gcc-linaro-aarch64-none-elf-4.8-2013.11_linux/bin/aarch64-none-elf-
$ export CROSS_COMPILE_T32=.../gcc-linaro-arm-none-eabi-4.8-2013.11_linux/bin/arm-none-eabi-
```
## build
```
$ make sm1_sabrina_v1_defconfig
$ make -j8
```
