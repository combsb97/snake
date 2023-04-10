# Snake in C with ncurses

This project is a terminal graphics implementation of the classic game, Snake. Written in C this project uses the ncurses library.

## Getting Started

### Dependencies

* ncurses

### Executing program

* compile
```
$ gcc snake.c -o snake -lncurses
```
* run (Linux)
```
$ ./snake
```

## Authors

Ben Combs @combsb97
Liban Dirir

## Version History

* 0.1
    * Initial Release
    * Snake has static length and initial position
    * Snake does not collide with itself nor the boundaries
    * Length score displayed at bottom of snake pit
    * No food/trophies
