# c

This repo holds a range of programs I used to learn the c programming language. Running applications in the `basics/` and `others/` directory should run without any external libaraies.

Programs found in the `graphics/` directory will require `raylib`.

## Notes

- [W3 School](https://www.w3schools.com/c/index.php)
- [Tsoding Daily](https://www.youtube.com/@TsodingDaily)
- [Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)

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
cc <file> -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra -Werror -std=c99
```
And execute using
```
./a.out
```
