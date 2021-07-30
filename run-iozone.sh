#!/bin/bash

logfile_latency=iozone_latency.log

logfile_throughput=iozone_throughput.log

echo -n "" > $logfile


record_size_latency='4k'


#calculating latency
for number_threads in 1 2 4 8
do	
	for file_size in '4m' '2m' '1m' '500k'
	do
		iozone -T -t $number_threads -s$file_size -r$record_size_latency -i 0 -i 2 -l O_DIRECT -+z -O -b latency.xlsx| tee -a $logfile_latency
	done

done


#caculating throughput

for number_threads in 1 2 4 8
do	
	for file_size in '1g' '500m' '250m' '125m'
	do
		for records in '64k' '1m' '16m'
		do
			iozone -T -t $number_threads -s$file_size -r$records -i 0 -i 1 -i 2 -l O_DIRECT -+z -b throughput.xlsx| tee -a $logfile_throughput
		done	
	done

done



