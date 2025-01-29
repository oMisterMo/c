#!/bin/bash

# Create output directory if it doesn't exist
mkdir -p graphics/out

# Loop through all C files in the current directory
for file in graphics/*.c; do
    # Get the base name of the file (without extension)
    base=$(basename "$file" .c)


    if [ "$file" == "graphics/test.c" ]; then
        continue
    fi
    echo "Building $file" 

    # Compile the C file and place the output in the 'out' directory
    gcc "$file" -o "graphics/out/$base" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
done

echo "Compilation finished! All output files are in the 'out' directory."
