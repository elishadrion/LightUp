/* 426215 - Drion elisha
 *
 */
#include <iostream>
#include <vector>
#include "Solver.hpp"

std::vector<int> get_horizontal(int** capacities, int m, int n);
std::vector<int> get_vertical(int** capacities, int m, int n);
void handle_free_case(Solver& s, int** capacities, int m, int n, int i, int j);
void handle_nl_wall(Solver& s, int** capacities, int m, int n, int i, int j);
/**
 * Pretty prints the given matrix
 * @param  matrix: matrix to print
 * @param  m: matrix height
 * @param  n: matrix width
 */
void pretty_print(int** matrix, int m, int n) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << matrix[i][j] << "\t";
    }
    std::cout << std::endl;
  }
}

/**
 * Solves the given light-up instance.
 * @param capacities: instance capacities to solve, an `m` by `n` matrix.
 * @param m: height of the instance
 * @param n: width of the instance
 * @param find_all: if true, find all valid solutions
 */
void solve(int** capacities, int m, int n, bool find_all) {

    //Xij = -2 -> vide
    //Xij = -1 -> mur capacité infinie
    //Xij = 0..4 -> mur capacité
    Solver s;
    int vars[m][n];
    //Create a variable per case
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            vars[i][j] = s.newVar();
        }
    }
    //Iterate throughout all the grid
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            //When it's a free (lightable) case
            if (capacities[i][j] == -2) {
                handle_free_case(s, capacities, m, n, i, j);
            }
            else if (capacities[i][j] == -1) {
                handle_nl_wall(s, capacities, m, n, i, j);
            }

        }
    }

  // Fonction à compléter pour les questions 2 et 3 (et bonus 1)
}

void printlogical(int i, int j) {
    std::cout << "(~" << i << "V" << "~" << j << ")" << std::endl;
}

/**
 * Handles the construction of clauses for the free (lightable) case
 * @param s: Solver
 * @param var: index of variable representing the current case
 * @param capacities : the matrix representing the problem
 * @param i: y position of the case
 * @param j: x position of the case
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_free_case(Solver& s, int var, int** capacities, int m, int n, int i, int j) {
    vec<Lit> lits;
    //For each case (thus literal/variable in the SAT), we find
    //the other cases which share the vertical and horizontal lines
    std::vector<int> horizontals = get_horizontal(capacities,m,n,i);
    std::vector<int> verticals = get_vertical(capacities,m,n,j);

    for(int j = 1; j < horizontals.size(); ++j) {
        lits.push(~Lit(var));
    }

    // for (int j : horizontals) {
    //     lits.push(~Lit(var));
    //     lits.push(~Lit(j));
    //     s.addClause(lits);
    //     lits.clear();
    // }
    //
    // for (int j : verticals) {
    //     lits.push(~Lit(var));
    //     lits.push(~Lit(j));
    //     s.addClause(lits);
    //     lits.clear();
    // }
}

/**
 * Handles the constructor of clauses for a no-limit wall (0 to 4 lightbulbs)
 * @param s: Solver
 * @param capacities : the matrix representing the problem
 * @param i: y position of the case
 * @param j: x position of the case
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_nl_wall(Solver& s, int** capacities, int m, int n, int i, int j) {
    vec<Lit> lits;

}

/**
 * Returns all the horizontal lightable cases
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
 std::vector<int> get_horizontal(int** capacities, int m, int n) {
    std::vector<int> horizontals;
    for(int i = 0; i < m; ++i) {
      int j = -1;
      while(j+1 < n-1) {
        ++j;
        //One of the pair is a wall, we pass
        if (capacities[i][j] != -2 || capacities[i][j+1] != -2) {
          continue;
        }
        //pushes a pair of cases to bundle with in a clause
        horizontals.push_bacj(i*n+j);
        horizontals.push_bacj(i*n+j+1);
      }
    }
    return horizontals;
 }

/**
 * Returns all the vertical lightable cases
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
 std::vector<int> get_vertical(int** capacities, int m, int n) {
     std::vector<int> verticals;
     for (int j = 0; j < n; ++j) {
       int i = -1;
       while (i+1 < m-1) {
         ++i;
         //One of the pair is a wall, we pass
         if (capacities[i][j] != -2 || capacities[i+1][j] != -2) {
           continue;
         }
         //pushes a pair of cases to bundle with in a clause
         verticals.push_bacj(i*n+j);
         verticals.push_bacj((i+1)*n+j);
       }
     }
     return verticals;
 }

/**
 * Generates `l` instances of the light-up problem, each with a unique solution,
 * and prints them on the standard output.
 * @param instance: instance to solve, an `m` by `n` matrix.
 * @param m: height of each instance
 * @param n: width of each instance
 */
void generate(int m, int n, int l) {
  // Fonction à compléter pour la question bonus 2
}

/**
 * Prints program help message
 * @param program: program name
 */
void print_help(char* program) {
  std::cerr << "Usage:" << std::endl;
  std::cerr << program << " [-g HAUTEUR LARGEUR NOMBRE]" << std::endl;
}

int main(int argc, char** argv) {
  int m, n, l;
  bool gen = false;
  bool find_all = false;

  // Parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0) {
      gen = true;
      try {
        m = std::stoi(argv[i + 1]);
        n = std::stoi(argv[i + 2]);
        l = std::stoi(argv[i + 3]);
      } catch (const std::logic_error& e) {
        print_help(argv[0]);
        return EXIT_FAILURE;
      }
    } else if (strcmp(argv[i], "-a") == 0) {
      find_all=true;
     } else if ((strcmp(argv[i], "--help") == 0) ||
               (strcmp(argv[i], "-h") == 0)) {
      print_help(argv[0]);
      return EXIT_SUCCESS;
    }
  }

  if (gen) {
    generate(m, n, l);
  } else {
    // read instance on standard input
    std::cin >> m >> n;
    int** capacities = new int*[m];
    for (int i = 0; i < m; ++i) {
      capacities[i] = new int[n];
      for (int j = 0; j < n; ++j) {
        std::cin >> capacities[i][j];
      }
    }

    solve(capacities, m, n, find_all);
  }

  return EXIT_SUCCESS;
}
