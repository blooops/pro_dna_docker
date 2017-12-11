/*
 *	structure.h
 *
 *		Author: Rohit Roy
 */

#pragma once

#include <string>
#include <fstream>

// Defining the necessary macros:
#define FILE_IO_ERROR(thefilename) std::cout<<"FILE I/O ERROR :"<<__LINE__<<" : "<<__FILE__<<": \nTHE FOLLOWING FILE COULD NOT BE OPENED :"<<(thefilename)<<std::endl
#define ALLOC_ERROR() std::cout<<"MEMORY ALLOCATION ERROR :"<<__LINE__<<" : "<<__FILE__<<": \nTHERE WAS AN ERROR IN ALLOCATING HEAP MEMORY"<<std::endl
#define NULL_PTR_ERROR() std::cout<<"NULL POINTER REFERENCING ERROR :"<<__LINE__<<" : "<<__FILE__<<": \n THERE WAS AN ATTEMPT TO DEREFERENCE A NULL POINTER " << std::endl
enum class AtomType {
	UNDEF = -1
};

enum class ResidueType {
	UNDEF = -1
};

enum class MoleculeType {
	UNDEF = -1, PROTEIN = 0, DNA = 1
};

struct Coordinate {
	float x, y, z;
};

struct SpaceMatrix {
	int* matrix;
	int size;
	float resolution;
};

struct MoleculeFeatures {
	float diameter;
	Coordinate max, min;
};

class Atom {

	Coordinate m_coords;
	int m_id;
	AtomType m_type;

public:
	Atom();
	~Atom();
	Atom(Coordinate coords, int id, AtomType type);

	Coordinate getCoordinates();
	void setCoordinates(Coordinate coords);
	int getId();
	void setId(int id);
	AtomType getAtomType();
	void setAtomType(AtomType type);
};

class Residue {

	int m_upperAtomIndex;
	int m_id;
	int m_lowerAtomIndex;
	ResidueType m_type;
	Coordinate m_center;
	Atom* m_atoms;

public:
	Residue();
	Residue(int upperAtomIndex, int id, int lowerAtomIndex, ResidueType type, Atom* atoms);
	~Residue();
	int getUpperAtomIndex();
	void setUpperAtomIndex(int index);
	int getLowerAtomIndex();
	void setLowerAtomIndex(int index);
	int getId();
	void setId(int id);
	ResidueType getResidueType();
	void setResidueType(ResidueType type);
	Coordinate getCenter();
	void setCenter();
	int getNumberOfAtoms();
	Atom* getAtoms();
	void setAtoms(Atom* atoms);
};

class Molecule {
	Atom* m_atoms;
	Residue* m_residues;
	int m_numberOfAtoms;
	int m_numberOfResidues;
	Coordinate m_center;
	MoleculeType m_type;

public:
	Molecule();
	Molecule(Atom* atoms, Residue* residues, int nAtoms, int nResidues, MoleculeType type);
	~Molecule();
	MoleculeType getMoleculeType();
	void setMoleculeType(MoleculeType type);
	int getNumberOfResidues();
	void setNumberOfResidues(int nResidues);
	int getNumberOfAtoms();
	void setNumberOfAtoms(int nAtoms);
	Atom* getAtoms();
	Residue* getResidues();
	void setAtoms(Atom* atoms);
	void setResidues(Residue* residues);
	Coordinate getCenter();
	void setCenter();
	int readPDBToMolecule(std::string filename);
	int writeMoleculeToPDB(std::string filename);
	MoleculeFeatures alignMolecule();
	SpaceMatrix* createSpaceMatrix(int size, MoleculeFeatures features, int value);
};


