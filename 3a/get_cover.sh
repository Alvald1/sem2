#!/bin/bash
for file in out/*/queue/*
do
    ./bin_cov < $file
    echo $file
done