#include "../../include/cdcl.hpp"

void parseDIMACS(std::string filename) {
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        // parse head of DIMACS
        std::getline(file, line);

        // skip comment lines
        while (line[0] == 'c') {
            // The line below shows the skipped comments.
            // std::cout << "Comment: " << line << std::endl;
            std::getline(file, line);
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }
        numOfVars = std::stoi(tokens[2]);
        numOfClauses = std::stoi(tokens[3]);
        numOfUnassigned = numOfVars;
        std::cout << "Number of Variables: " << numOfVars << std::endl;
        std::cout << "Number of Clauses: " << numOfClauses << std::endl <<"\n";

        // parse rest
        vars.resize(numOfVars + 1);  // vars in DIMACS are 1-indexed
        for (int i = 0; i < numOfVars + 1; i++) {
            Variable v;
            vars[i] = v;
        }
        std::vector<int> dummy;
        cnf.push_back(dummy);  // push dummy clause on cnf[0] to ensure 1-index.
        int count = 1;         // what clause are we processing?
        std::vector<int> clause;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (line[0] == 0) {
                continue;
            }

            int literal;
            while (iss >> literal && literal != 0) {
                // not precise if the literal appears multiple times in the
                // clause (unlikely)
                (literal > 0) ? vars[std::abs(literal)].pos_occ++ : vars[std::abs(literal)].neg_occ++;
                vars[std::abs(literal)].tot_occ++;

                clause.push_back(literal);

                // std::cout << "Literal: " << literal << std::endl;
            }

            if (literal == 0) {
                if (!clause.empty()) {
                    // std::cout << "Literal: " << clause.literals[0] << "in if" << std::endl;

                    clause[0] > 0 ? vars[std::abs(clause[0])].pos_watched.insert(count)
                                           : vars[std::abs(clause[0])].neg_watched.insert(count);
                    // if unit clause, push to unit queue
                    if (clause.size() == 1) {
                        // std::cout << "Literal: " << clause.literals[0] << "in if22" << std::endl;

                        if (!vars[std::abs(clause[0])].enqueued) {
                            unitQueue.push(clause[0]);
                            // std::cout << "Pushing " << clause.literals[0] << " on unit queue" << std::endl;
                            vars[std::abs(clause[0])].enqueued = true;
                        }
                    }

                    // else also link the second watched literal to their respective entry in variables

                    if (clause.size() > 1)
                        clause[1] > 0 ? vars[std::abs(clause[1])].pos_watched.insert(count)
                                               : vars[std::abs(clause[1])].neg_watched.insert(count);

                    cnf.push_back(clause);
                    // std::cout << "for clause " << count <<":";
                    // for (int i = 0; i < clause.literals.size(); i++) {
                    //     std::cout << " " << clause.literals[i];
                    // }
                    // std::cout << "\n";
  
                    clause = {};
                    count++;
                }
            }
        }

        file.close();
    } else {
        printf("Unable to open file");
    }
}