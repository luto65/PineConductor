cmake -DARM_NONE_EABI_TOOLCHAIN_PATH=/home/luca/gcc-arm-none-eabi-10.3-2021.10 -DNRF5_SDK_PATH=/home/luca/nRF5_SDK_15.3.0_59ac345 -DBUILD_DFU=1 -DENABLE_USERAPPS="Apps::Pulser,Apps::Transposer,Apps::GregorianModes" -DENABLE_WATCHFACES="WatchFace::Conductor"