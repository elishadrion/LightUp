# Compile
Run `make` to compile

# Solve instance
To solve an instance, run with `./lightup` then provide instance on standard input (or pipe instance into `./lightup`, e.g.: `cat exemples/7_easy0.txt | ./lightup`).
## All solutions
To output all possible solutions for the given instance, add the `-a` flag, e.g.: `cat exemples/7_easy0.txt | ./lightup -a`

# Generate instances
Run with `./lightup -g HEIGHT WIDTH COUNT` to generate `COUNT` instances of dimensions `HEIGHT` x `WIDTH`

# Instances
## Basic instances
Instances with capacities from -1 to 1 are provided in the `exemples` directory. These should all be supported for questions 1 to 3.
## Bonus instances
Instances with capacities from -1 to 4 are provided in the `exemples-bonus` directory.
