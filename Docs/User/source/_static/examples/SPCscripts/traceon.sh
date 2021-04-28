#!/bin/bash

echo "Turning kernel tracing on"

# Pin the writeback workqueue to CPU0
echo 1 > /sys/bus/workqueue/devices/writeback/cpumask

DIR=/sys/kernel/debug/tracing
echo > $DIR/trace
echo 0 > $DIR/tracing_on
# Uncomment the below for more details on what disturbs the CPU
echo 0 > $DIR/events/irq/enable
echo 0 > $DIR/events/sched/sched_switch/enable
echo 0 > $DIR/events/sched/sched_kthread_stop/enable
echo 0 > $DIR/events/workqueue/workqueue_queue_work/enable
echo 0 > $DIR/events/workqueue/workqueue_execute_start/enable
echo 0 > $DIR/events/timer/hrtimer_expire_entry/enable
echo 0 > $DIR/events/timer/hrtimer_expire_exit/enable
echo 0 > $DIR/events/timer/tick_stop/enable
echo 0 > $DIR/events/rcu/rcu_utilization/enable
echo 1 > $DIR/events/context_tracking/user_enter/enable
echo 1 > $DIR/events/context_tracking/user_exit/enable
echo hwlat > $DIR/current_tracer
echo 4 > $DIR/tracing_cpumask
echo 1 > $DIR/tracing_on

