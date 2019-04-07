/* 426215 - Drion Elisha
 *
 */
#include <iostream>
#include <vector>
#include <string>
#include "Solver.hpp"

std::vector<int> get_horizontal(int** capacities, int m, int n);
std::vector<int> get_vertical(int** capacities, int m, int n);
void handle_no_sharing_cases(Solver& s, int** capacities, int m, int n);
void handle_all_lighted_up(Solver& s, int** capacities, int m, int n);
void handle_walls(Solver& s, int** capacities, int m, int n);

/*
 * Credit : https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c/28698654
 */
void get_combinations(int offset, int k, std::vector<int>& vec,  std::vector<int>& combinations, std::vector<int>& temp) {
    if (vec.empty() || vec.size() < k) return;
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
    //Create a variable per case
    for (int i = 0; i < m*n; ++i) s.newVar();
    handle_no_sharing_cases(s, capacities, m, n);
    handle_all_lighted_up(s, capacities, m, n);
    handle_walls(s, capacities, m, n);


    if (s.solve()) { // la formule est satisfaisable

        std::cout << "La formule est satisfaisable avec la valuation \n" ;
        for (int i = 0 ; i < m*n ; i++) { // r´ecup´eration de la valuation
          if (s.model[i] == l_True) {
            std::cout << "la variable " << i << " est mise à vraie\n";
          }
          else {
            std::cout << "la variable " << i << " est mise à faux\n";
          }
        }
      }

      else {
        std::cout << "La formule n’est pas satisfaisable\n" ;
      }

  // Fonction à compléter pour les questions 2 et 3 (et bonus 1)
}

/**
 * Handles the construction of clauses for forbiding lightbulbs
 * sharing a column or row
 * @param s: Solver
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_no_sharing_cases(Solver& s, int** capacities, int m, int n) {
    //For each case (thus literal/variable in the SAT), we find
    //the other cases which share the vertical and horizontal lines
    std::vector<int> horizontals = get_horizontal(capacities,m,n);
    std::vector<int> verticals = get_vertical(capacities,m,n);
    for(int i = 0; i < horizontals.size(); i+=2) {
        s.addBinary(~Lit(horizontals[i]), ~Lit(horizontals[i+1]));
    }
    for(int i = 0; i < verticals.size(); i+=2) {
        s.addBinary(~Lit(verticals[i]), ~Lit(verticals[i+1]));
    }
}

bool is_in_vec(std::vector<int>& vec, int check) {
    for (int k : vec ) {
        if (k == check) return true;
    }
    return false;
}

void handle_all_lighted_up(Solver& s, int** capacities, int m, int n) {
    //"Cross" formed by the row and the column we want lighted up
    vec<Lit> lits;
    for (int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            //For every case, we get the entire cross
            //Starting with the entire row and then the entire column
            lits.push(Lit(i*n+j));
            int k = j+1;
            //From current position to rightmost of the row
            while (k < n && capacities[i][k] == -2) {
                lits.push(Lit(i*n+k));
                ++k;
            }
            k = j-1;
            //From current position to leftmost of the row
            while (k >= 0 && capacities[i][k] == -2) {
                lits.push(Lit(i*n+k));
                --k;
            }
            //Entire column
            k = i+1;
            //From current position to bottom of column
            while (k < m && capacities[k][j] == -2) {
                lits.push(Lit(k*n+j));
                ++k;
            }
            k = i-1;
            //From current position to top of column
            while (k >= 0 && capacities[k][j] == -2) {
                lits.push(Lit(k*n+j));
                --k;
            }
            s.addClause(lits);
            lits.clear();
        }
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

/**
 * Handles the construction of clauses for a wall with only
 * c (its capacity) free cases around it
 * @param s: Solver
 * @param adjacents : the cases adjacent to the wall
 */
void handle_edge_case(Solver& s, std::vector<int>& adjacents) {
    for (int adj : adjacents) {
        s.addUnit(Lit(adj));
    }
}

/**
 * Handles the construction of clauses for a wall with a capacity of 1
 * @param s: Solver
 * @param adjacents : the cases adjacent to the wall
 */
void handle_one_capacity(Solver& s, std::vector<int>& adjacents) {
    vec<Lit> lits;
    std::vector<int> combinations, temp;
    //Edge case where only one case is free
    if (adjacents.size() == 1) {
        handle_edge_case(s, adjacents);
        return;
    }
    //Get all the possible combinations of pairs of cases
    get_combinations(0, 2, adjacents, combinations, temp);
    for(int k = 1; k < combinations.size(); ++k) {
        s.addBinary(~Lit(combinations[k-1]), ~Lit(combinations[k]));
    }
    //Enforce placing a lightbulb on a case
    for (int adj : adjacents) lits.push(Lit(adj));
    s.addClause(lits);
}

/**
 * Handles the construction of clauses for a wall with a capacity of 2
 * @param s: Solver
 * @param adjacents : the cases adjacent to the wall
 */
void handle_two_capacity(Solver& s, std::vector<int> adjacents) {
    vec<Lit> lits;
    //Edge case where only two cases are free
    if (adjacents.size() == 2) {
        handle_edge_case(s, adjacents);
        return;
    }
    for(int i = 0; i < adjacents.size(); ++i) {
        for (int j = i+1; j < adjacents.size(); ++j) {
            s.addBinary(~Lit(adjacents[i]), ~Lit(adjacents[i]));
        }
    }
    for (int adj : adjacents) lits.push(Lit(adj));
    s.addClause(lits);
    lits.clear();
}

/**
 * Handles the construction of clauses for all the walls (0 to 4 lightbulbs)
 * @param s: Solver
 * @param capacities : the matrix representing the problem
 * @param m: height of each instance
 * @param n: width of each instance
 */
void handle_walls(Solver& s, int** capacities, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::vector<int> adjacents = get_adjacent_cases(capacities, m, n, i, j);
            //undefined capacity => we don't bother
            int c = capacities[i][j];
            if (c == -1) continue;
            //capacity of 0, we can't place any lightbulb around it
            else if (c == 0) {
                for (int adj : adjacents) s.addUnit(~Lit(adj));
            }
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
    std::vector<int> horizontals, temp, input;
    //Iterate through all rows
    for(int i = 0; i < m; ++i) {
        int j = 0;
        while (j < n) {
            //Wall case
            if (capacities[i][j] != -2) {
                get_combinations(0, 2, input, horizontals, temp);
                input.clear();
            } else {
                input.push_back(i*n+j);
            }
            ++j;
        }
        //To not forget to add
        get_combinations(0, 2, input, horizontals, temp);
        input.clear();
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
     std::vector<int> verticals, temp, input;
     //Iterate throughout all the columns
     for(int j = 0; j < n; ++j) {
         int i = 0;
         while (i < m) {
             //Wall case
             if (capacities[i][j] != -2) {
                 get_combinations(0, 2, input, verticals, temp);
                 input.clear();
             } else {
                 input.push_back(i*n+j);
             }
             ++i;
         }
         //To not forget to add
         get_combinations(0, 2, input, verticals, temp);
         input.clear();
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
