/*	docker.h
 * 	Author : Rohit Roy
 *
 */
#pragma once
#include <ctime>
#include "structure.h"
#include "log.h"


#define PROTEIN_FILL_VALUE 1
#define DNA_FILL_VALUE -15

class Docker {
private:
	Molecule* m_protein;
	Molecule* m_dna;
	SpaceMatrix* m_proteinMatrix;
	SpaceMatrix* m_dnaMatrix;
	int m_rotationalStep;
	bool m_cudaFlag;
	bool m_initFlag;
	bool m_checkFlag;
	int m_matrixLength;
	bool m_logFlag;
	MoleculeFeatures m_proteinFeatures;
	MoleculeFeatures m_dnaFeatures;
	std::clock_t m_start;
	Logger* m_log;

public:
	~Docker();
	Docker();
	void setCudaFlag(bool flag);
	bool getCudaFlag() const;
	void setInitFlag(bool flag);
	bool getInitFlag() const;
	int getMatrixLength();
	void initProcedure(std::string proteinFileName, std::string dnaFileName, int size);
	void startTimer();
	double getElapsedTime();
	void setUpLogging(std::string filename);
	void checkConstraints();
	void log(std::string);

};

// Non parallel function definitions:

void generate_surface(SpaceMatrix* space, int thickness, int core_value);

// Parallel function definitions:

void p_generate_surface(SpaceMatrix* space, int thickness, int core_value);
