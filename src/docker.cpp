/*	docker.cpp
 * 	Author : Rohit Roy
 *
 */

#include "docker.h"
#define INDEX(x, y, z, size) ((x) + (y) * (size) + (z) * (size) * (size))

// Defining Docker class member functions here:
Docker::~Docker() {
	delete m_protein;
	delete m_dna;
	if(m_dnaMatrix != nullptr)
		delete[] m_dnaMatrix->matrix;
	if(m_proteinMatrix != nullptr)
		delete[] m_proteinMatrix->matrix;
}

Docker::Docker() {
	m_protein = nullptr;
	m_dna = nullptr;
	m_rotationalStep = 0;
	m_matrixLength = 0;
	m_cudaFlag = false;
	m_logFlag = false;
	m_initFlag = false;
	m_checkFlag = false;
	m_proteinMatrix = nullptr;
	m_dnaMatrix = nullptr;
}

void Docker::setCudaFlag(bool flag) {
	m_cudaFlag = flag;
}

bool Docker::getCudaFlag() const {
	return m_cudaFlag;
}

void Docker::setInitFlag(bool flag) {
	m_initFlag = flag;
}

bool Docker::getInitFlag() const {
	return m_initFlag;
}

int Docker::getMatrixLength() {
	return m_matrixLength;
}

void Docker::initProcedure(std::string proteinFileName,
		std::string dnaFileName) {
	// Initialize everything to get ready for the docking procedure to begin.

	m_protein = new Molecule;
	m_dna = new Molecule;

	if (m_protein == nullptr || m_dna == nullptr) {
		m_initFlag = false;
		return;
	}
	if (m_protein->readPDBToMolecule(proteinFileName) == -1
			|| m_dna->readPDBToMolecule(dnaFileName) == -1) {
		m_initFlag = false;
		return;
	}

	// All molecules and their respective PDB files have been created. Now check once more if everything is fine:
	checkConstraints();
	if (m_checkFlag == false) {

		return;
	}

	// Align and create space matrices:
	m_proteinFeatures = m_protein->alignMolecule();
	m_dnaFeatures = m_dna->alignMolecule();

	if (m_proteinFeatures.diameter == 0.0f || m_dnaFeatures.diameter == 0.0f) {
		m_checkFlag = false;
		return;
	}


}

void Docker::checkConstraints() {

}

void Docker::startTimer() {

}

void Docker::endTimer() {

}

void Docker::logProcedureDetails(std::string filename) {

}

// Check the neighbours of an element to see if the element is a surface element
bool check_neighbours(int thickness, int* matrix, int size, int x, int y, int z,
		int core_value) {
	if (matrix[INDEX(x, y, z, size)] != 0) {
		int neighbourhood = 1;
		for (int p = -neighbourhood; p <= neighbourhood; p++) {
			for (int q = -neighbourhood; q <= neighbourhood; q++) {
				for (int r = -neighbourhood; r <= neighbourhood; r++) {
					if (matrix[INDEX(x + p, y + q, z + r, size)] != core_value)
						return true;
				}
			}
		}
		return false;
	}
	return false;
}

// Set the surface molecules to the value 1
void generate_surface(SpaceMatrix* space, int thickness, int core_value) {
	// checking the inner elements
	for (int x = 1; x < space->size - 1; x++) {
		for (int y = 1; y < space->size - 1; y++) {
			for (int z = 1; z < space->size - 1; z++) {
				int element = space->matrix[INDEX(x, y, z, space->size)];
				if (element != 0
						&& check_neighbours(thickness, space->matrix,
								space->size, x, y, z, core_value))
					space->matrix[INDEX(x, y, z, space->size)] = 1;
			}
		}
	}

	// setting the outer elements to surface elements by definition
}
