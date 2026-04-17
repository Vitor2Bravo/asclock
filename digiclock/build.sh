#! /usr/sbin/env bash

set -xe

gcc -Wall -o digiclock main.c ./includes/cma_args.c --std=c11 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

