#!/usr/bin/env bash
set -e
echo "Creating ./bin directory..."
mkdir -p ./bin

make

echo "Running RecompModTool..."
./RecompModTool ./mod.toml ./bin

echo "Zipping output file into ./bin..."
zip -j ./bin/no_bubble_textboxes.zip ./bin/no_bubble_textboxes.nrm

echo "Complete"