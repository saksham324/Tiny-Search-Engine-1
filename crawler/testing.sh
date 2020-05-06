#!/bin/bash

# testing.sh - testing the tse crawler module

# Compiling common
cd ../common
make clean
make

# Compiling crawler
cd -
make clean
make

# Variables
testURL=http://old-www.cs.dartmouth.edu/~cs50/index.html
externalURL=www.google.com
out=output  # name of directory

# remove output directory if it exists
if [ -d $out ]
then
    rm -rf $out/
fi

# make the directory and test subdirectory
mkdir $out
mkdir $out/test

# 1 argument
./crawler

# 2 arguments
./crawler $testURL  

# 3 arguments
./crawler $testURL $out/test 

# 4 arguments + externalURL
./crawler $externalURL $out/test 2 

# non existent server
./crawler http://old-www.cs.dartmouth.gov $out 0

# non existent page
./crawler $testURL/index $out/test 2 

# negative maxDepth
./crawler $testURL $out -7

# cross-linked webpage - letters
testURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
depth=0

while [ $depth -lt 7 ]
do 
    dir=letters-depth-$depth

    # make directory to store output files
    mkdir $out/$dir
    ./crawler $testURL $out/$dir $depth 
    ((depth++))
done

# different seedURl
testURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
./crawler $testURL $out/test 2


# wikipedia playground
testURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
depth=0

while [ $depth -lt 3 ]
do
    dir=wikipedia-depth-$depth
    # make direcrory to store output files
    mkdir $out/$dir
    ./crawler $testURL $out/$dir $depth
    ((depth++))
done

# cleanup.
make clean
cd -
make clean
cd -