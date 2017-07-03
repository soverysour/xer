#!/bin/sh

valgrind $@ --leak-check=yes bin/Xer
