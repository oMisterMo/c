# c

This repo holds various examples. You'll find basic to complex programs.

## Files

- arguments.c
- arrays.c
- boolean.c
- enums.c
- files.c
- input.c
- memory.c
- pointers.c
- strings.c
- structs.c
- types.c

## Getting started

To compile c programs, you can use `cc`, `gcc`, `clang` or any other compiler
```
clang arrays.c
```

Now run the generated executable
```
./a.out
```

## Building `raylib` applications

> ### Prerequsites
>You must have raylib installed. This is done by downloading the binaries or using the package manager on your respective OS.
> More information is found on the official [github](https://github.com/raysan5/raylib) page.

### Running `raylib` programs

Navigate to the graphics directory 
```
cd graphics
```
Now build the app, replacing &lt;file&gt; with the path of the c program
```
cc <file> -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
And execute using
```
./a.out
```
