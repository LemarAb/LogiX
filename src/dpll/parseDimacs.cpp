#include "../../include/cnf.hpp"

void parseDIMACS(std::string filename) {
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        // parse head of DIMACS
        std::getline(file, line);

        // skip comment lines and empty lines
        while (line[0] == 'c' || line.empty()) std::getline(file, line);

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }
        numOfVars = std::stoi(tokens[2]);
        numOfClauses = std::stoi(tokens[3]);

        std::cout << "Number of Variables: " << numOfVars << "\n";
        std::cout << "Number of Clauses: " << numOfClauses << "\n\n";
        // parse rest
        vars.resize(numOfVars + 1);  // vars in DIMACS are 1-indexed
        for (int i = 0; i < numOfVars + 1; i++) {
            Variable v;
            vars[i] = v;
        }
        Clause dummy;
        dummy.literals = {};
        dummy.active = -1;
        clauses.push_back(dummy);  // push dummy clause on cnf[0] to ensure 1-index.
        int count = 1;             // what clause are we processing?
        Clause clause;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (line[0] == 0) {
                continue;
            }

            int literal;
            while (iss >> literal && literal != 0) {
                clause.literals.push_back(literal);
                literal > 0 ? vars[std::abs(literal)].static_pos_occ.insert(count)
                            : vars[std::abs(literal)].static_neg_occ.insert(count);
            }

            if (literal == 0) {
                if (!clause.literals.empty()) {
                    
                    clause.active = clause.literals.size();
                    // if unit clause, push to unit queue
                    if (clause.literals.size() == 1) {
                        
                        if (!vars[std::abs(clause.literals[0])].enqueued) {
                            toPropagate.push(clause.literals[0]);
                            
                            vars[std::abs(clause.literals[0])].enqueued = true;
                        }
                    }

                    
                    clauses.push_back(clause);
                    clause = {};
                    count++;
                }
            }
        }
        file.close();
    } else {
        printf("Unable to open file");
    }

    for (int i = 1; i <= numOfVars; i++) {
        vars[i].pos_occ = vars[i].static_pos_occ;
        vars[i].neg_occ = vars[i].static_neg_occ;
    }

    // push pure literals to prop queue
    for (int i = 1; i <= numOfVars; i++) {
        if ((vars[i].static_neg_occ.size() * vars[i].static_pos_occ.size()) == 0) {
            vars[std::abs(i)].enqueued = true;
            toPropagate.push(std::abs(i));
        }
    }
}