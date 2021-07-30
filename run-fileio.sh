#!/bin/bash

logfile_throughput=fileio_throughput.log
logfile_latency=fileio_latency.log

measure='latency'
for num_threads in 1 2 4 8
do
	for dataset_latency in '4m' '2m' '1m' '500k'
	do
		for record_size_latency in '4k'
		do
			for access_pattern_latency in 'WR' 'RR'
			do
    			./fileio $num_threads $dataset_latency $access_pattern_latency $record_size_latency $measure | tee -a $logfile_latency
    		done
    	done
    done
done

measure='throughput'
for num_threads in 1 2 4 8
do
	for dataset_throughput in '1g' '500m' '250m' '125m'
	do
		for record_size_throughput in '64k' '1m' '16m'
		do
			for access_pattern_throughput in 'WS' 'RS' 'WR' 'RR'
			do
    			./fileio $num_threads $dataset_throughput $access_pattern_throughput $record_size_throughput $measure | tee -a $logfile_throughput
    		done
    	done
    done
done