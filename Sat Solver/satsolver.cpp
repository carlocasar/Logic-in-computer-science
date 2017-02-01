#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <time.h>
using namespace std;

#define UNDEF -1
#define TRUE 1
#define FALSE 0

uint numVars;
uint numClauses;
vector<vector<int> > clauses;
vector<int> model;
vector<int> modelStack;
uint indexOfNextLitToPropagate;
uint decisionLevel;

unsigned long numDecisions, numPropagacions;
double timeIni,timeFi, timeTotal;

vector<vector<vector<int>* > > clausesPositive;
vector<vector<vector<int>* > > clausesNegative;

vector<int> heuristicValue;

void readClauses( ){
  // Skip comments
  char c = cin.get();
  while (c == 'c') {
    while (c != '\n') c = cin.get();
    c = cin.get();
  }  
  // Read "cnf numVars numClauses"
  string aux;
  cin >> aux >> numVars >> numClauses;
  clauses.resize(numClauses);  
  heuristicValue.resize(numVars +1, 0);
  clausesPositive.resize(numVars +1);
  clausesNegative.resize(numVars +1);
  decisionLevel = numDecisions = numPropagacions = indexOfNextLitToPropagate = 0;
  // Read clauses
  for (uint i = 0; i < numClauses; ++i) {
      int lit;
      while (cin >> lit and lit != 0){
	int var = abs(lit);
	++heuristicValue[var];

	if (lit > 0) clausesPositive[var].push_back(&clauses[i]);
	else         clausesNegative[var].push_back(&clauses[i]);

	clauses[i].push_back(lit);
      }
   }
}


int currentValueInModel(int lit){
  if (lit >= 0) return model[lit];
  else {
    if (model[-lit] == UNDEF) return UNDEF;
    else return 1 - model[-lit];
  }
}


void setLiteralToTrue(int lit){
  modelStack.push_back(lit);
  if (lit > 0) model[lit] = TRUE;
  else model[-lit] = FALSE;		
}

//En vez de visitar todas las clausulas solo visitar las que contienen la decision actual contraria.**********
inline bool propagate() {
    int lit_being_propagated = modelStack[indexOfNextLitToPropagate];

    uint var_being_propagated = abs(lit_being_propagated);
    vector<vector<int>* >& clauses_opposite = lit_being_propagated > 0 ?
                            clausesNegative[var_being_propagated] :
                            clausesPositive[var_being_propagated];
    for (int i = 0; i < clauses_opposite.size(); ++i) {
        vector<int>& clause = *clauses_opposite[i];
	bool someLitTrue = false;
        int numUndefs = 0;
        int lastLitUndef = 0;
        
        for (int j = 0; not someLitTrue and j < clause.size(); ++j) {
            int lit = clause[j];
            int val = currentValueInModel(lit);
            if (val == UNDEF) ++numUndefs; 
	    if (val == UNDEF) lastLitUndef = lit;
            someLitTrue = val == TRUE;
        }

        if (not someLitTrue and numUndefs < 2) {
            if (numUndefs == 1) setLiteralToTrue(lastLitUndef);
            else {
                ++heuristicValue[var_being_propagated];
                return true;
            }
        }
    }

    return false;

}

inline bool propagateGivesConflict() {
    while (indexOfNextLitToPropagate < modelStack.size()) {
        if (propagate()) return true;

        ++numPropagacions;
        ++indexOfNextLitToPropagate;
    }
    return false;
}


void backtrack(){
  cout.setf(ios::fixed); cout.precision(3);
  uint i = modelStack.size() -1;
  int lit = 0;
  while (modelStack[i] != 0){ // 0 is the DL mark
    lit = modelStack[i];
    model[abs(lit)] = UNDEF;
    modelStack.pop_back();
    --i;
  }
  // at this point, lit is the last decision
  modelStack.pop_back(); // remove the DL mark
  --decisionLevel;
  indexOfNextLitToPropagate = modelStack.size();
  setLiteralToTrue(-lit);  // reverse last decision
}

// Mejor heuristica para encontrar la siguiente decision.**********
// Heuristic for finding the next decision literal:
int getNextDecisionLiteral(){
  int maxvalue = -1;
  int max;
  for (uint i = 1; i <= numVars; ++i)
    if (model[i] == UNDEF && heuristicValue[i] > maxvalue){
         maxvalue = heuristicValue[i];
         max = i;
    }
  return maxvalue != -1 ? max : 0;
}

void checkmodel(){
  for (int i = 0; i < numClauses; ++i){
    bool someTrue = false;
    for (int j = 0; not someTrue and j < clauses[i].size(); ++j)
      someTrue = (currentValueInModel(clauses[i][j]) == TRUE);
    if (not someTrue) {
      cout << "Error in model, clause is not satisfied:";
      for (int j = 0; j < clauses[i].size(); ++j) cout << clauses[i][j] << " ";
      cout << endl;
      exit(1);
    }
  }  
}


int main(){ 
  timeIni = clock();
  readClauses(); // reads numVars, numClauses and clauses
  model.resize(numVars+1,UNDEF);
  indexOfNextLitToPropagate = 0;  
  decisionLevel = 0;
  
  // Take care of initial unit clauses, if any
  for (uint i = 0; i < numClauses; ++i)
    if (clauses[i].size() == 1) {
      int lit = clauses[i][0];
      int val = currentValueInModel(lit);
      if (val == FALSE) {cout << "Tiempo es: " << ((clock()) - timeIni)/1000000 << " Propagaciones: " << numPropagacions << " Decisiones: " << numDecisions << " UNSATISFIABLE" << endl; return 10;}
      else if (val == UNDEF) setLiteralToTrue(lit);
    }
  
  // DPLL algorithm
  while (true) {
    while ( propagateGivesConflict() ) {
      if ( decisionLevel == 0) { cout << "Tiempo es: " << ((clock()) - timeIni)/1000000 << " Propagaciones: " << numPropagacions << " Decisiones: " << numDecisions << " UNSATISFIABLE" << endl; return 10; }
      else backtrack();
    }
    int decisionLit = getNextDecisionLiteral();
    if (decisionLit == 0) { checkmodel(); cout << "Tiempo es: " << ((clock()) - timeIni)/1000000 << " Propagaciones: " << numPropagacions << " Decisiones: " << numDecisions << " SATISFIABLE" << endl; return 20; }
    // start new decision level:
    ++numDecisions;
    modelStack.push_back(0);  // push mark indicating new DL
    ++indexOfNextLitToPropagate;
    ++decisionLevel;
    setLiteralToTrue(decisionLit);    // now push decisionLit on top of the mark
  }
}  
