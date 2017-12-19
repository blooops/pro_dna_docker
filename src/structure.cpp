/*
 * structure.cpp
 *
 *      Author: Rohit Roy
 */

#include <iostream>
#include <algorithm>
#include <fstream>
#include "structure.h"

// Function definitions for all Atom class public functions/constructors/destructors:

Atom::Atom() {
	m_coords.x = m_coords.y = m_coords.z = 0.0f;
	m_id = -1;
	m_type = AtomType::UNDEF;
}

Atom::Atom(Coordinate coords, int id, AtomType type) {
	m_coords = coords;
	m_id = id;
	m_type = type;
}

Atom::~Atom() {
	;
}

Coordinate Atom::getCoordinates() const {
	return m_coords;
}

void Atom::setCoordinates(Coordinate coords) {
	m_coords = coords;
}

int Atom::getId() const {
	return m_id;
}

void Atom::setId(int id) {
	m_id = id;
}

AtomType Atom::getAtomType() const {
	return m_type;
}

void Atom::setAtomType(AtomType type) {
	m_type = type;
}

// Function definitions for all Residue class public functions/constructors/destructors:

Residue::Residue() {
	m_upperAtomIndex = m_lowerAtomIndex = -1;
	m_type = ResidueType::UNDEF;
	m_center = {0.0f, 0.0f, 0.0f};
	m_id = -1;
	m_atoms = nullptr;
}

Residue::Residue(int upperAtomIndex, int id, int lowerAtomIndex,
		ResidueType type, Atom* atoms) {
	m_upperAtomIndex = upperAtomIndex;
	m_lowerAtomIndex = lowerAtomIndex;
	m_type = type;
	m_id = id;
	m_atoms = atoms;
	setCenter();
}

Residue::~Residue() {
	;
}

int Residue::getUpperAtomIndex() const {
	return m_upperAtomIndex;
}

void Residue::setUpperAtomIndex(int index) {
	m_upperAtomIndex = index;
}

int Residue::getLowerAtomIndex() const {
	return m_lowerAtomIndex;
}

void Residue::setLowerAtomIndex(int index) {
	m_lowerAtomIndex = index;
}

int Residue::getId() const {
	return m_id;
}

void Residue::setId(int id) {
	m_id = id;
}

ResidueType Residue::getResidueType() const {
	return m_type;

}

void Residue::setResidueType(ResidueType type) {
	m_type = type;
}

Coordinate Residue::getCenter() {
	return m_center;
}

void Residue::setCenter() {
	// Calculates the center of the residue based on the coordinates of the atoms that are a part of this residue
	m_center = {0.0f, 0.0f, 0.0f};
	if(m_lowerAtomIndex == -1 || m_upperAtomIndex==-1)
	return;

	for(int i = m_lowerAtomIndex; i <= m_upperAtomIndex; i++) {
		m_center.x += m_atoms[i].getCoordinates().x;
		m_center.y += m_atoms[i].getCoordinates().y;
		m_center.z += m_atoms[i].getCoordinates().z;
	}
	int n = getNumberOfAtoms();
	m_center.x /= n;
	m_center.y /= n;
	m_center.z /= n;
}

int Residue::getNumberOfAtoms() {
	return m_upperAtomIndex - m_lowerAtomIndex + 1;
}

Atom* Residue::getAtoms() const {
	return m_atoms;
}

void Residue::setAtoms(Atom* atoms) {
	m_atoms = atoms;
}

// Function definitions for all Molecule class public functions/constructors/destructors:

Molecule::Molecule() {
	m_atoms = nullptr;
	m_residues = nullptr;
	m_numberOfAtoms = m_numberOfResidues = 0;
	m_type = MoleculeType::UNDEF;
	m_center = {0.0f, 0.0f, 0.0f};
}

Molecule::Molecule(Atom* atoms, Residue* residues, int nAtoms, int nResidues,
		MoleculeType type) {
	m_atoms = atoms;
	m_residues = residues;
	m_numberOfAtoms = nAtoms;
	m_numberOfResidues = nResidues;
	m_type = type;
	setCenter();
}

Molecule::~Molecule() {
	if (m_atoms != nullptr)
		delete[] m_atoms;
	if (m_residues != nullptr)
		delete[] m_residues;
	m_atoms = nullptr;
	m_residues = nullptr;
}

MoleculeType Molecule::getMoleculeType() const {
	return m_type;
}

void Molecule::setMoleculeType(MoleculeType type) {
	m_type = type;
}

int Molecule::getNumberOfResidues() const {
	return m_numberOfResidues;
}

void Molecule::setNumberOfResidues(int nResidues) {
	m_numberOfResidues = nResidues;
}

int Molecule::getNumberOfAtoms() const {
	return m_numberOfAtoms;
}

void Molecule::setNumberOfAtoms(int nAtoms) {
	m_numberOfAtoms = nAtoms;
}

Coordinate Molecule::getCenter() const {
	return m_center;
}

Atom* Molecule::getAtoms() const {
	return m_atoms;
}

Residue* Molecule::getResidues() const {
	return m_residues;
}

void Molecule::setAtoms(Atom* atoms) {
	m_atoms = atoms;
}

void Molecule::setResidues(Residue* residues) {
	m_residues = residues;
}

void Molecule::setCenter() {
	m_center = {0.0f, 0.0f, 0.0f};

	if(m_atoms == nullptr) {
		NULL_PTR_ERROR();
		return;
	}

	for(int i = 0; i < m_numberOfAtoms; i++) {
		m_center.x += m_atoms[i].getCoordinates().x;
		m_center.y += m_atoms[i].getCoordinates().y;
		m_center.z += m_atoms[i].getCoordinates().z;
	}
	m_center.x /= m_numberOfAtoms;
	m_center.y /= m_numberOfAtoms;
	m_center.z /= m_numberOfAtoms;
}

int Molecule::readPDBToMolecule(std::string filename) {
	if (m_atoms != nullptr || m_residues != nullptr) // check that the two heaps of atoms and residues are free
		return -1;

	// First create file streams and ensure that they work:
	std::ifstream stream(filename);
	if (stream) { /////////////////////////////////////////////////////////////////////////////////////////////////
		FILE_IO_ERROR(filename);
		stream.close();
		return -1;
	}

	// Having checked that the file opens, now allocate memory to atoms and residues as needed:
	int nAtoms, nResidues;
	stream >> nAtoms >> nResidues;
	setNumberOfAtoms(nAtoms);
	setNumberOfResidues(nResidues);
	m_atoms = new Atom[nAtoms];
	m_residues = new Residue[nResidues];

	// Checking if heap allocation was successful:
	if (m_atoms != nullptr && m_residues == nullptr) {
		ALLOC_ERROR();
		delete[] m_atoms;
		m_atoms = nullptr;
		return -1;
	}

	else if (m_atoms == nullptr && m_residues != nullptr) {
		ALLOC_ERROR();
		delete[] m_residues;
		m_residues = nullptr;
		return -1;
	}

	else if (m_atoms == nullptr && m_residues == nullptr) {
		ALLOC_ERROR();
		return -1;
	}

	// Else there's no error. Read in the data for the Atoms from the file stream:
	for (int i = 0; i < m_numberOfAtoms; i++) {
		Coordinate t_coords;
		int t_id, t_type;
		stream >> t_id >> t_coords.x >> t_coords.y >> t_coords.z >> t_type;
		m_atoms[i].setId(t_id);
		m_atoms[i].setCoordinates(t_coords);
		m_atoms[i].setAtomType((AtomType) t_type);
	}

	// Read in the data for the Residues from the file stream:
	for (int i = 0; i < m_numberOfResidues; i++) {
		int t_id, t_type, t_upperIndex, t_lowerIndex;
		stream >> t_id >> t_type >> t_upperIndex >> t_lowerIndex;
		m_residues[i].setId(t_id);
		m_residues[i].setResidueType((ResidueType) t_type);
		m_residues[i].setUpperAtomIndex(t_upperIndex);
		m_residues[i].setLowerAtomIndex(t_lowerIndex);
		m_residues[i].setAtoms(m_atoms);
		m_residues[i].setCenter();
	}

	// Clean up after everything has been read properly
	stream.close();

	return 0;
}

int Molecule::writeMoleculeToPDB(std::string filename) { /////////////////////////////////////////////////////////
	return 0;
}

MoleculeFeatures Molecule::alignMolecule() {
	MoleculeFeatures features;
	features.min = (Coordinate ) { 0.0f, 0.0f, 0.0f };
	features.max = (Coordinate ) { 0.0f, 0.0f, 0.0f };
	features.diameter = 0.0f;

	if (getAtoms() == nullptr || getResidues() == nullptr) {
		NULL_PTR_ERROR();
		return features;
	}

	// Finding minimum and maximum coordinates to find diameter
	for (int i = 0; i < m_numberOfAtoms; i++) {
		if (features.min.x > m_atoms[i].getCoordinates().x)
			features.min.x = m_atoms[i].getCoordinates().x;

		if (features.min.y > m_atoms[i].getCoordinates().y)
			features.min.y = m_atoms[i].getCoordinates().y;

		if (features.min.z > m_atoms[i].getCoordinates().z)
			features.min.z = m_atoms[i].getCoordinates().z;

		if (features.max.x < m_atoms[i].getCoordinates().x)
			features.max.x = m_atoms[i].getCoordinates().x;

		if (features.max.y < m_atoms[i].getCoordinates().y)
			features.max.y = m_atoms[i].getCoordinates().y;

		if (features.max.z < m_atoms[i].getCoordinates().z)
			features.max.z = m_atoms[i].getCoordinates().z;
	}

	features.diameter = std::max(features.max.x - features.min.x,
			std::max(features.max.y - features.min.y,
					features.max.z - features.min.z)) + 3.0f; // 3.0f offset to ensure that the diameter encompasses the molecule

	// Adding necessary values of translation to make all coordinates positive
	float min_x = std::min(0.0f, features.min.x);
	float min_y = std::min(0.0f, features.min.y);
	float min_z = std::min(0.0f, features.min.z);

	for (int i = 0; i < m_numberOfAtoms; i++) {
		m_atoms[i].setCoordinates(
				(Coordinate ) { m_atoms[i].getCoordinates().x - min_x,
								m_atoms[i].getCoordinates().y - min_y,
								m_atoms[i].getCoordinates().z - min_z });
	}

	features.max.x += (-min_x);
	features.min.x += (-min_x);
	features.max.y += (-min_y);
	features.min.y += (-min_y);
	features.max.z += (-min_z);
	features.min.z += (-min_z);

	// Updating the center of molecule and individual residues
	for (int i = 0; i < m_numberOfResidues; i++)
		m_residues[i].setCenter();

	return features;

}

SpaceMatrix* Molecule::createSpaceMatrix(int size, MoleculeFeatures features,
		int value) {
	// Check that the atoms is initialized properly
	if (m_atoms == nullptr) {
		NULL_PTR_ERROR();
		return nullptr;
	}

	// Everything's fine. Allocate memory to Matrix
	SpaceMatrix* space;
	space = new SpaceMatrix;

	if (space == nullptr) {
		ALLOC_ERROR();
		return nullptr;
	}

	space->size = size;
	space->matrix = new int[space->size * space->size * space->size]();

	if (space->matrix == nullptr) {
		ALLOC_ERROR();
		delete space;
		return nullptr;
	}

	// Allocation is fine. Create space matrix
	float resolution = features.diameter / size;
	space->resolution = resolution;

	for (int i = 0; i < m_numberOfAtoms; i++) {
		Coordinate t_coords = m_atoms[i].getCoordinates();
		t_coords.x /= resolution;
		t_coords.y /= resolution;
		t_coords.z /= resolution;
		int idx = (int) t_coords.x;
		int idy = (int) t_coords.y;
		int idz = (int) t_coords.z;
		space->matrix[idx + idy * size + idz * size * size] = value;
	}

	// All allocation was fine. Now return the pointer to the matrix
	return space;
}

