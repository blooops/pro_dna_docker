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
	if (m_dnaMatrix != nullptr)
		delete[] m_dnaMatrix->matrix;
	if (m_proteinMatrix != nullptr)
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
	m_log = new Logger;
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

void Docker::initProcedure(std::string proteinFileName, std::string dnaFileName,
		int size) {
	// Initialize everything to get ready for the docking procedure to begin.

	m_protein = new Molecule;
	m_dna = new Molecule;

	if (m_protein == nullptr || m_dna == nullptr) {
		ALLOC_ERROR();
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

	// Align molecules:
	m_proteinFeatures = m_protein->alignMolecule();
	m_dnaFeatures = m_dna->alignMolecule();

	if (m_proteinFeatures.diameter == 0.0f || m_dnaFeatures.diameter == 0.0f) {
		m_checkFlag = false;
		return;
	}

	// Create Space Matrices
	m_dnaMatrix = new SpaceMatrix;
	m_proteinMatrix = new SpaceMatrix;
	if (m_dnaMatrix == nullptr || m_proteinMatrix == nullptr) {
		ALLOC_ERROR();
		delete m_dnaMatrix;
		delete m_proteinMatrix;
		m_checkFlag = false;
		m_initFlag = false;
		return;
	}

	// create the space discretized matrices:
	float diameter = std::max(m_dnaFeatures.diameter,
			m_proteinFeatures.diameter);
	float resolution = diameter / size;

	m_dnaMatrix->resolution = m_proteinMatrix->resolution = resolution;
	m_dnaMatrix->size = m_proteinMatrix->size = size;

	m_dnaMatrix->matrix = new int[size * size * size];
	m_proteinMatrix->matrix = new int[size * size * size];

	if (m_dnaMatrix == nullptr || m_proteinMatrix == nullptr) {
		ALLOC_ERROR();
		delete[] m_dnaMatrix->matrix;
		delete[] m_proteinMatrix->matrix;
		m_initFlag = m_checkFlag = false;
		return;
	}

	// Allocation was fine. Initialize matrices with 0
	for (int i = 0; i < size * size * size; i++)
		m_dnaMatrix->matrix[i] = m_proteinMatrix->matrix[i] = 0;

	// Assign fill values to locations of atoms in the molecules
	for (int i = 0; i < m_protein->getNumberOfAtoms(); i++) {
		int x, y, z;
		x = (int) m_protein->getAtoms()[i].getCoordinates().x / resolution;
		z = (int) m_protein->getAtoms()[i].getCoordinates().z / resolution;
		y = (int) m_protein->getAtoms()[i].getCoordinates().y / resolution;
		m_proteinMatrix->matrix[INDEX(x, y, z, m_proteinMatrix->size)] =
		PROTEIN_FILL_VALUE;
	}

	for (int i = 0; i < m_dna->getNumberOfAtoms(); i++) {
		int x, y, z;
		x = (int) m_dna->getAtoms()[i].getCoordinates().x / resolution;
		z = (int) m_dna->getAtoms()[i].getCoordinates().z / resolution;
		y = (int) m_dna->getAtoms()[i].getCoordinates().y / resolution;
		m_dnaMatrix->matrix[INDEX(x, y, z, m_dnaMatrix->size)] =
		PROTEIN_FILL_VALUE;
	}

	// Fill values allocated. Generating surfaces
	// If need be generate_surface(m_proteinMatrix, 0, PROTEIN_FILL_VALUE);
	generate_surface(m_dnaMatrix, 0, DNA_FILL_VALUE);

	// Setting up flags
	m_checkFlag = true;
	m_initFlag = true;
}

void Docker::checkConstraints() { //(TODO)
	// Checks whether the currently loaded PDB data satisfies the constraints to run
	// the docker (Error catching to prevent excessive memory leaks in case something's
	// not right.
}

void Docker::startTimer() {
	m_start = std::clock();
}

double Docker::getElapsedTime() {
	return (std::clock() - m_start) / (double) CLOCKS_PER_SEC;
}

void Docker::setUpLogging(std::string filename) {
	m_log->setLogFileName(filename);
	m_log->openStream(filename);
	m_log->setLogLevel(LogLevel::MAX);
	m_log->setFileLogFlag();
}

void Docker::log(std::string str) {
	m_log->write(str);
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

	// YZ Plane
	for (int y = 0; y < space->size; y++) {
		for (int z = 0; z < space->size; z++) {
			if (space->matrix[INDEX(0, y, z, space->size)] != 0)
				space->matrix[INDEX(0, y, z, space->size)] = 1;
		}
	}

	// XZ Plane
	for (int x = 0; x < space->size; x++) {
		for (int z = 0; z < space->size; z++) {
			if (space->matrix[INDEX(x, 0, z, space->size)] != 0)
				space->matrix[INDEX(x, 0, z, space->size)] = 1;
		}
	}

	// XY Plane
	for (int y = 0; y < space->size; y++) {
		for (int x = 0; x < space->size; x++) {
			if (space->matrix[INDEX(x, y, 0, space->size)] != 0)
				space->matrix[INDEX(x, y, 0, space->size)] = 1;
		}
	}
}
