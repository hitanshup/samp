## WHAT IS IT?

* This is a game called snake. Goal of the game is to score more points without dying.

* The snake can score points by eating the fruits. Eating fruit increases your score by 10 points and increases the size of your snake by 1 block.

* The snake dies by either hitting the corner of the screen, or by eating itself(by touching it's own block).

* Snake starts with 5 blocks by default where each block is of the size 10 by 10 px.

* A fruit is also the same size as the block.


## DEVELOPMENT ENVIRONMENT

* In order to run this game you need to have C++ compilerinstalled. This can run on g++ 4.94 or later.

* You should also have an XServer installed(Windows XMing, macOS XQuartz, or Linux).

* This game has been developed using C++ and Xlib.


## HOW TO PLAY IT?

* To start the game, you need to first compile it using the following commands from the current direcetory:

```
cd src
make
```

* Then you can start playing the game using the command `./snake 30 5` where 30 represents the Frame rate(Frames Per Second) ranging from 25 - 60 while 5 represents the speed of the snake ranging from 1 - 10 with 1 being slowest and 10 being the fastest.

* You can also start the game with defaut arguments using the following commands from the current directory:

```
cd src
make run
```

* You can play this game by navigating the snake(left and right) using the keys `a` and `d`.

* You can pause, restart or quit the game at any time using the `p`, `r` or `q` keys respectively.

* After the game is restarted the score will reset.

* After the game is paused, you can resume the game by pressing `e`, restart by pressing `r` or quit by pressing `q`.

* Once the game is over, you can restart by pressing `r` or quit by pressing `q`.
