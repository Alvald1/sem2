#!/bin/bash
for file in out/*/queue/*
do
    ./bin_* < $file
    echo $file
done
