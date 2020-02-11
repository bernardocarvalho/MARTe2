#!/bin/bash

echo "Turning kernel trace off"

echo 0 > $DIR/tracing_on

