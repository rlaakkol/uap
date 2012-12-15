#!/bin/bash

for i in `seq 1 $1`
do
	./tester $i $2 $3 &
	sleep $4
done
