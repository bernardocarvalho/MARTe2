#!/bin/bash

echo "Turning kernel trace off"

DIR=/sys/kernel/debug/tracing

echo 0 > $DIR/tracing_on

