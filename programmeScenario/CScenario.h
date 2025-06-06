#include "CBDD.h"
#include <Arduino.h>

class CScenario
{
private:
  int idScenario;
  byte tabTrame[255];
  CBDD* baseDeDonnees;

public:
  CScenario(int idScenario);
  ~CScenario();
  void executerScenario(int idScenario);
  int getIdScenario();
};