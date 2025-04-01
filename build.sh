#!/bin/bash

clear
echo -e "\n[INFO]: BUILD STARTED..!\n"
set -e

#init submodules
git submodule init && git submodule update

export KERNEL_ROOT="$(pwd)"
export ARCH=arm64
export KBUILD_BUILD_USER="@CuriousNom"

# Setup necessary directories
rm -rf out
rm -rf build
rm -rf error.log
mkdir -p "${KERNEL_ROOT}/out" "${KERNEL_ROOT}/build"

# Export toolchain paths
export PATH="${PATH}:${HOME}/toolchains/neutron-clang/bin"
export NEUTRON_PATH="${HOME}/toolchains/neutron-clang/bin"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${HOME}/toolchains/neutron-clang/lib"

# Set cross-compile environment variables
export BUILD_CC="${HOME}/toolchains/neutron-clang/bin/clang"

# Build options for the kernel
export BUILD_OPTIONS="
-C ${KERNEL_ROOT} \
O=${KERNEL_ROOT}/out \
-j$(nproc --all) \
ARCH=arm64 \
CROSS_COMPILE=aarch64-linux-gnu- \
CLANG_TRIPLE=aarch64-linux-gnu- \
CC=${BUILD_CC} \
LLVM=1 \
LLVM_IAS=1 \
AR=${NEUTRON_PATH}/llvm-ar \
NM=${NEUTRON_PATH}/llvm-nm \
LD=${NEUTRON_PATH}/ld.lld \
STRIP=${NEUTRON_PATH}/llvm-strip \
OBJCOPY=${NEUTRON_PATH}/llvm-objcopy \
OBJDUMP=${NEUTRON_PATH}/llvm-objdump \
READELF=${NEUTRON_PATH}/llvm-readelf \
HOSTCC=${NEUTRON_PATH}/clang \
HOSTCXX=${NEUTRON_PATH}/clang++ \
"

# Enable ccache for speed up compiling
export CCACHE_DIR="$HOME/.cache/ccache_akernel"
export CC="ccache clang"
export CXX="ccache clang++"
export PATH="/usr/lib/ccache:$PATH"
echo "CCACHE_DIR: [$CCACHE_DIR]"

build_kernel(){
    # Make default configuration.
    make ${BUILD_OPTIONS} gki_defconfig vendor/pineapple_GKI.config

    # Build the kernel
    make ${BUILD_OPTIONS} Image 2> >(tee -a error.log >&2) || exit 1

    # Copy the built kernel to the build directory
    cp "${KERNEL_ROOT}/out/arch/arm64/boot/Image" "${KERNEL_ROOT}/build"

    echo -e "\n[INFO]: BUILD FINISHED..!"
}
build_kernel
