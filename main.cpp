#include <iostream>

#include "circuit.h"

int main() {
  // Creating multiset of {node, resistence}
  Circuit circuit(9);

  circuit.connect(0, 1, 2);

  circuit.connect(1, 2, 6);
  circuit.connect(2, 3, 2);

  circuit.connect(3, 4, 10);
  circuit.connect(3, 7, 20);
  circuit.connect(3, 8, 20);

  circuit.connect(4, 5, 10);
  circuit.connect(4, 6, 20);
  circuit.connect(4, 7, 20);

  circuit.connect(5, 6, 10);
  circuit.connect(6, 7, 10);
  circuit.connect(7, 8, 10);

  std::cout << circuit.calculate_total_resistence() << std::endl;
  return 0;
}
