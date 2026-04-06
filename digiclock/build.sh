#! /usr/sbin/env bash

set -xe

gcc -Wall -o digiclock main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

