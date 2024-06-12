#!/bin/bash
for file in out/*/crashes/*
do
    ./bin_* < $file
    echo $file
done
