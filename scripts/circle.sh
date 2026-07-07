#!/usr/bin/env bash

unload() {
    pkill -f twx
    exit 1
}
trap unload SIGINT

i=0
n=$1
while ((i < n)); do
    bin/twx&
    ((i++))
    sleep $2
done

wait
