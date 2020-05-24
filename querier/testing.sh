#!/bin/bash

# testing.sh - testing the tse querier module

# Compiling common
cd ../common
make clean
make
cd -

# Variables
cc=./querier
fuzz=./fuzzquery
pagedir=../tse-output/toscrape-depth-1
index=../indexer/toscrape-depth-1-index

# wrong pageDirectory
$cc ../crawler $index

# wrong IndexFilename
$cc $pagedir ./crawler

# wrong number of arguments
$cc $pagedir $index ../crawler

# invalid queries
echo home_review | $cc $pagedir $index

echo and home review | $cc $pagedir $index

echo or home review | $cc $pagedir $index

echo home and or review | $cc $pagedir $index

echo home review and | $cc $pagedir $index


# fuzzquery
$fuzz $index 25 0 | $cc $pagedir $index




