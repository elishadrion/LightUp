# LightUp
Solving LightUp (Akari) problems using minisat.

# Rules
You can find the rules on [Wikipedia](https://en.wikipedia.org/wiki/Light_Up_(puzzle)).
Light Up is played on a rectangular grid of white and black cells. The player places light bulbs in white cells such that no two bulbs shine on each other, until the entire grid is lit up. A bulb sends rays of light horizontally and vertically, illuminating its entire row and column unless its light is blocked by a black cell. A black cell may have a number on it from 0 to 4, indicating how many bulbs must be placed adjacent to its four sides; for example, a cell with a 4 must have four bulbs around it, one on each side, and a cell with a 0 cannot have a bulb next to any of its sides. An unnumbered black cell may have any number of light bulbs adjacent to it, or none. Bulbs placed diagonally adjacent to a numbered cell do not contribute to the bulb count.

## How to use

### Input format
The first entry should be the size of the grid, followed by the grid itself.
The following values mean :
* `-2` a free case
* `-1` a wall with an undefined capacity (0 to 4)
* `0` to `4` a wall with the value as the defined capacity

```
7 7
-2 -2 -2 -2 -2 -2 -2
-2 -2 -1 -2 -1 -2 -2
-2 -1 -2 -2 -2 -1 -2
-2 -2 -2 -2 -2 -2 -2
-2 -1 -2 -2 -2 0 -2
-2 -2 -1 -2 -1 -2 -2
-2 -2 -2 -2 -2 -2 -2
```

### Output
To display a solution :
`cat path/to/problem | path/to/lightup`
To display all the possible solutions, add `-a`at the end :
`cat path/to/problem | path/to/lightup -a`

For each solution, it will first list the positions of the lightbulbs, then the grid itself with `X` as the lightbulbs.

Example :
`cat exemples/instance1 | ./lightup`
```
1 3
2 5
3 2
4 1
5 4
0       -       X       -       -
-       -       -       -       X
-       X       1       0       -
X       -       -       -       -
-       -       -1      X       -1
```
