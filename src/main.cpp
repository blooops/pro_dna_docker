#include "structure.h"
#include <iostream>

int main() {
	Atom* atoms;
	atoms = new Atom[10];
	for(int i = 0; i < 10; i++) {
		atoms[i].setId(i+1);
		atoms[i].setCoordinates({0.1f, 0.0f, 0.0f});
	}

	Residue r(8, 1, 2, ResidueType::UNDEF, atoms);
	std::cout<< r.getCenter().x << " " << r.getNumberOfAtoms() << std::endl;

	delete[] atoms;
	std::cin.get();
	return 0;
}
