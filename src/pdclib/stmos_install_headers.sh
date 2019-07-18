#!/bin/bash
# Installs header files to arm-stmos directory (~/arm-stmos)

cp -Rv include/* $HOME/arm-stmos/usr/include
cp -Rv platform/stmos/include/* $HOME/arm-stmos/usr/include

