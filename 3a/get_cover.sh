#!/bin/bash
for file in out/*/queue/*
do
    ./bin_asan < $file
    echo $file
done
