#! /usr/bin/env bash

set -xe

gcc -Wall -o digiclock main.c --std=c11 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

