#!/bin/bash

for file in boards/20x20/*.txt; do
    filename=$(basename "$file" .txt)

    if [[ "$filename" =~ ^[0-9]+$ ]]; then
        if [ "$filename" -le 10 ]; then
            echo "$filename"
            ./poly "$file"
        fi
    fi
done