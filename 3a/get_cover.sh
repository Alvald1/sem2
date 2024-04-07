#!/bin/bash
for file in out/*/queue/*
do
    ./bin_cov < $file
    echo $file
done
echo 'e test' > 1
./bin_cov < 1
