#!/bin/bash

echo "Turning kernel tracing on"

# Pin the writeback workqueue to CPU0
echo 1 > /sys/bus/workqueue/devices/writeback/cpumask

DIR=/sys/kernel/debug/tracing
echo > $DIR/trace
echo 0 > $DIR/tracing_on
# Uncomment the below for more details on what disturbs the CPU
echo 1 > $DIR/events/irq/enable
echo 1 > $DIR/events/sched/sched_switch/enable
echo 1 > $DIR/events/workqueue/workqueue_queue_work/enable
echo 1 > $DIR/events/workqueue/workqueue_execute_start/enable
echo 1 > $DIR/events/timer/hrtimer_expire_entry/enable
echo 1 > $DIR/events/timer/tick_stop/enable
echo nop > $DIR/current_tracer
echo 1 > $DIR/tracing_on

