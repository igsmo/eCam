# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/components/bootloader/subproject"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix/tmp"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix/src"
  "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ttoft/OneDrive/Dokumenter/QRposing/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
