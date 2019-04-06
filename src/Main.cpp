/* 426215 - Drion elisha
 *
 */
#include <iostream>
#include <vector>
#include <string>
#include "Solver.hpp"

std::vector<int> get_horizontal(int** capacities, int m, int n);
std::vector<int> get_vertical(int** capacities, int m, int n);
void handle_free_case(Solver& s, int** capacities, int m, int n);
void handle_walls(Solver& s, int** capacities, int m, int n);

/*
 * Credit : https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c/28698654
 */
void get_combinations(int offset, int k, std::vector<int>& vec,  std::vector<int>& combinations, std::vector<int>& temp) {
    if (k == 0) {
        combinations.insert(combinations.end(), temp.begin(), temp.end());
        return;
    }
    for (int i = offset; i <= vec.size() - k; ++i) {
        temp.push_back(vec[i]);
        get_combinations(i+1, k-1, vec, combinations, temp);
        temp.pop_back();
    }
}

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
    handle_free_case(s, capacities, m, n);
    handle_walls(s, capacities, m, n);

  // Fonction à compléter pour les questions 2 et 3 (et bonus 1)
}

/**
 * Handles the construction of clauses for all the free (lightable) cases
 * @param s: Solver
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_free_case(Solver& s, int** capacities, int m, int n) {
    vec<Lit> lits;
    //For each case (thus literal/variable in the SAT), we find
    //the other cases which share the vertical and horizontal lines
    std::vector<int> horizontals = get_horizontal(capacities,m,n);
    std::vector<int> verticals = get_vertical(capacities,m,n);
    for(int i = 0; i < horizontals.size(); i+=2) {
        lits.push(~Lit(horizontals[i]));
        lits.push(~Lit(horizontals[i+1]));
        s.addClause(lits);
        lits.clear();
    }
    for(int i = 0; i < verticals.size(); i+=2) {
        lits.push(~Lit(verticals[i]));
        lits.push(~Lit(verticals[i+1]));
        s.addClause(lits);
        lits.clear();
    }
}


std::vector<int> get_adjacent_cases(int** capacities, int m, int n, int i, int j) {
    std::vector<int> adjacents;
    if (i-1 >= 0 && capacities[i-1][j] == -2) adjacents.push_back((i-1)*n+j);
    if (i+1 < m && capacities[i+1][j] == -2) adjacents.push_back((i+1)*n+j);
    if (j-1 >= 0 && capacities[i][j-1] == -2) adjacents.push_back(i*n+j-1);
    if (j+1 < n && capacities[i][j+1] == -2) adjacents.push_back(i*n+j+1);
    return adjacents;
}

void handle_edge_case(Solver& s, std::vector<int>& adjacents) {
    vec<Lit> lits;
    for (int adj : adjacents) {
        lits.push(Lit(adj));
        s.addClause(lits);
        lits.clear();
    }
}

void handle_one_capacity(Solver& s, std::vector<int>& adjacents) {
    vec<Lit> lits;
    //Edge case where only one case is free
    if (adjacents.size() == 1) {
        handle_edge_case(s, adjacents);
        return;
    }
    std::vector<int> combinations;
    std::vector<int> temp;
    get_combinations(0, 2, adjacents, combinations, temp);
    //for(int k = 0; k < combinations.size())
    // for(int i = 0; i < adjacents.size(); ++i) {
    //     for (int j = i+1; j < adjacents.size(); ++j) {
    //         lits.push(~Lit(adjacents[i]));
    //         lits.push(~Lit(adjacents[j]));
    //         s.addClause(lits);
    //         lits.clear();
    //     }
    // }
    for (int adj : adjacents) lits.push(Lit(adj));
    s.addClause(lits);
    lits.clear();
}

void handle_two_capacity(Solver& s, std::vector<int> adjacents) {
    vec<Lit> lits;
    //Edge case where only two cases are free
    if (adjacents.size() == 2) {
        handle_edge_case(s, adjacents);
        return;
    }
    for(int i = 0; i < adjacents.size(); ++i) {
        for (int j = i+1; j < adjacents.size(); ++j) {
            lits.push(~Lit(adjacents[i]));
            lits.push(~Lit(adjacents[j]));
            s.addClause(lits);
            lits.clear();
        }
    }
    for (int adj : adjacents) lits.push(Lit(adj));
    s.addClause(lits);
    lits.clear();
}

/**
 * Handles the constructor of clauses for all the walls (0 to 4 lightbulbs)
 * @param s: Solver
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_walls(Solver& s, int** capacities, int m, int n) {
    vec<Lit> lits;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::vector<int> adjacents = get_adjacent_cases(capacities, m, n, i, j);
            //undefined capacity => we don't bother
            int c = capacities[i][j];
            if (c == -1) continue;
            //capacity of 0, we can't place any lightbulb around it
            else if (c == 0) {
                for (int adj : adjacents) lits.push(~Lit(adj));
                s.addClause(lits);
                lits.clear();
            }
            //capacity of 1 : k! clauses
            else if (c == 1) {
                handle_one_capacity(s, adjacents);
            }
            else if (c == 2) {
                handle_two_capacity(s, adjacents);
            }
        }
    }
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
        horizontals.push_back(i*n+j);
        horizontals.push_back(i*n+j+1);
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
         verticals.push_back(i*n+j);
         verticals.push_back((i+1)*n+j);
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
