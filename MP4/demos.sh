#!/bin/sh
# ECE 223, Spring 2016
# A example shell script to generate some demonstrations of the system
# behavior for MP4 
#
# either use chmod to make this file executable
#     chmod 755 demos.sh 
# or call the program sh
#     sh  demos.sh
#
echo TEST 1: default settings no dropped packets
./lab4 -v

echo TEST 2: queue size 2 with dropped packets
./lab4 -v -p 15 -q fifo -r 1 -l 2 -a 3 -m 1500 -s 40

echo TEST 3: five input streams and output transmitters
./lab4 -v -i 5 -o 5 -a 1 -p 20

echo TEST 4: matching queueing theory
./lab4 -p 2000000 -a 16

echo TEST 4: busy system
./lab4 -p 1000000 -i 10 -o 100 -a 0.126

echo "end"

