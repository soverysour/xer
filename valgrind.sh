#!/bin/sh

cd bin
valgrind $1 --leak-check=yes ./Xer
