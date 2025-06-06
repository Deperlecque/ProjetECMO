#include <Arduino.h>

class CPotentiometre
{
private:
  int positionCible;
  int positionActuelle;

public:
  CPotentiometre();
  ~CPotentiometre();
  void deplacerPotentiometre(int positionCible);
};