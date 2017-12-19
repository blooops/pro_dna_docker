/*	docker.h
 * 	Author : Rohit Roy
 *
 */
#pragma once
#include <ctime>
#include "structure.h"
#include "log.h"

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
	Logger m_log;
	MoleculeFeatures m_proteinFeatures;
	MoleculeFeatures m_dnaFeatures;

public:
	~Docker();
	Docker();
	void setCudaFlag(bool flag);
	bool getCudaFlag() const;
	void setInitFlag(bool flag);
	bool getInitFlag() const;
	int getMatrixLength();
	void initProcedure(std::string proteinFileName, std::string dnaFileName);
	void startTimer();
	void endTimer();
	void logProcedureDetails(std::string filename);
	void checkConstraints();

};

// Non parallel function definitions:

void generate_surface(SpaceMatrix* space, int thickness, int core_value);

// Parallel function definitions:

void p_generate_surface(SpaceMatrix* space, int thickness);
