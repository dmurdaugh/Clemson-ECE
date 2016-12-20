#!/bin/sh
# ECE 223, Spring 2016
# Denzel Murdaugh
# script for MP4 to generate data for performance analysis
#
# either use chmod to make this file executable
#     chmod 755 testscript.sh
# or call the program sh
#     sh testscript.sh
#
echo TEST 1 Large queue size and medium variance for packet size
date
seed=`date +%s`
numpackets=1000000
queuesize=500
meanpktsize=1500
stdpktsize=40
sources=10
arrivalrates="120 48 24 16 13.3 12.6 12 10 5"
for rate in $arrivalrates ; do
    ./lab4 -l $queuesize -p $numpackets -a $rate -m $meanpktsize -s $stdpktsize -r $seed
done
#
#
# TEST 2 Large queue size and large variance for packet size
echo TEST 2 Large queue size and large variance for packet size
largestdpktsize=500
date
for rate in $arrivalrates ; do
    ./lab4 -l $queuesize -p $numpackets -a $rate -m $meanpktsize -s $largestdpktsize -r $seed
done
#
# TEST 3 Small queue size 
echo TEST 3 Small queue size 
date
smallqueuesize=10
for rate in $arrivalrates ; do
    ./lab4 -l $smallqueuesize -p $numpackets -a $rate -m $meanpktsize -s $stdpktsize -r $seed
done
#
# TEST 4 1 source 10 transmitters
echo TEST 4 1 source 10 transmitters
date
smallqueuesize=10
for rate in $arrivalrates ; do
    ./lab4 -l $queuesize -p $numpackets -a $rate -m $meanpktsize -s $stdpktsize -r $seed -o $sources
done
# TEST 5 10 sources 1 transmitter
echo TEST 5 10 source 1 transmitters
date
smallqueuesize=10
for rate in $arrivalrates ; do
    ./lab4 -l $queuesize -p $numpackets -a $rate -m $meanpktsize -s $stdpktsize -r $seed -i $sources
done
echo "end"
