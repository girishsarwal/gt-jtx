# How to build from source #

1. Checkout source in a new folder - Here's how to:

## Windows ##
  1. Setup the AVR compilation toolchain
  1. Do _**one**_ of the following:
    * If you have Atmel Studio: Open up gt-jtx.atsln in Atmel Studio and hit build. You will end up with the hex file in the release folder
    * If you have VMLab: Open up the VMLab projet and hit build. You will end up with the hex file in the same folder as the project
    * If you have WinAVR/GCC - use the makefile to make the build.

## Linux ##
  1. Setup the compilation toolchain setup
  1. Do _**one**_ of the following:
    * Use wine to run VMLab and build. Simulation does not work properly
    * use the makefile to make the build using GCC

## Building from source and programing the ATMEGA - m8 version the first time ##

<a href='http://www.youtube.com/watch?feature=player_embedded&v=xcH1Vaep3ds' target='_blank'><img src='http://img.youtube.com/vi/xcH1Vaep3ds/0.jpg' width='425' height=344 /></a>