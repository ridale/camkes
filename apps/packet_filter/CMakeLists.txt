#
# Copyright 2019, Penten
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#

cmake_minimum_required(VERSION 3.7.2)

project(filter C)

includeGlobalComponents()

set(LibEthdriverNumPreallocatedBuffers 32 CACHE STRING "" FORCE)

# This only works on sabre
if(
    NOT
    "${KernelArch}"
    STREQUAL
    "arm"
    OR
        NOT
        "${KernelARMPlatform}"
        STREQUAL
        "sabre"
)
    message(FATAL_ERROR "filter application is only supported on Arm sabre")
endif()

set(CAmkESDTS ON CACHE BOOL "" FORCE)

DeclareCAmkESComponent(Epit 
            SOURCES components/Epit/src/main.c )

DeclareCAmkESComponent(SDCard 
            SOURCES components/SDCard/src/main.c)

DeclareCAmkESComponent(TPM 
            SOURCES components/TPM/src/main.c)

DeclareCAmkESComponent(Filter 
            SOURCES components/Filter/src/main.c
            components/Eth/src/packbuf.c
            INCLUDES components/Eth/include
            )

DeclareCAmkESComponent(Eth 
            SOURCES components/Eth/src/main.c
            components/Eth/src/eth.c
            components/Eth/src/packbuf.c
            INCLUDES components/Eth/include
            LIBS ethdrivers)

DeclareCAmkESComponent(UsbEth 
            SOURCES components/UsbEth/src/main.c
            components/Eth/src/packbuf.c
            INCLUDES components/Eth/include
            LIBS usbdrivers)

DeclareCAmkESRootserver(filter.camkes)
