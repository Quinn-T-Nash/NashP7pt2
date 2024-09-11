//Quinn Nash
//qnash@cnm.edu
//File: NashP7pt2
//Purpose: Read any analyze data from a CSV file

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

const int NUM_COMPANIES{ 50 };

const string FILENAME{ "Top50USTechCompanies2022_2023.csv" };

struct TechCompanyInfo
{
	string name{ "" };
	string sector{ "" };
	int yearFounded{ 0 };
	float annualRevBillions{ 0.0 };
	float marketCapTrillions{ 0.0 };
	float incomeTaxBillions{ 0.0 };
	int numEmployees{ 0 };
};

struct SectorInfo
{
	string sectorTitle{ "" };
	vector<TechCompanyInfo> companyInfo;
};

//WriteHeader
void Header(stringstream& r_ssReport);

//Reads in the data from const FILENAME and fills array returns true if opened
bool ReadData(TechCompanyInfo techData[]);

//Fills vector of structs for Sector Info to separate company info based on common sectors
void FillSectorInfo(TechCompanyInfo techData[], vector<SectorInfo> &r_sectorNames);

//push_back a unique sector to the vector r_sectorNames (called in FillSectorInfo)
void IncreaseSectorVectorSize(int i, TechCompanyInfo techData[], vector<SectorInfo>& r_sectorNames);

//Finds the company from each sector with the most employees, sends results to stringstream
void MostEmployeesBubbleSort(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport);

//Find the Sector with the most companies, and the sector with the most employees
void DetermineSectorSizeEmpAndComp(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport);

/*
Find the total income tax each sector paid. Find the 
highest and lowest ratio of income tax divide by revenue for each sector
(ignore compnaies who paid negative income tax and place them in a list)
*/
void CalcIncomeTax(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport, TechCompanyInfo techData[]);

//finds company in each sector with highest tax divide by rev ratio
void FillHighestTaxRevRatioVectors(vector<SectorInfo>& r_sectorNames, vector<int>& r_indexOfCompanyWithHighestRatio,
	vector<float>& r_sectorHighestTaxRevRatio);

//finds company in each sector with lowest tax divide by rev ratio
void FillLowestTaxRevRatioVectors(vector<SectorInfo>& r_sectorNames, vector<int>& r_indexOfCompanyWithLowestRatio,
	vector<float>& r_sectorLowestTaxRevRatio);

//find the Average ratio of income tax divide by revenue for all the top 50 companies
void AverageTaxRevRatioAll(TechCompanyInfo techData[], float *p_avgRatio);

//Display the companies who paid negative tax, and suggest what they should have paid
void ShameNegativeTaxPayers(vector<TechCompanyInfo>& r_negativeTaxPayers, float avgRatio,
	stringstream &r_ssReport);

//find the higest revenue divide by market cap for each company
void CalcRevOverMarkCap(vector<SectorInfo>& r_sectorNames, stringstream& r_ss_Report);

//write report to file, specified by the user
void WriteReport(stringstream& r_ssReport);


#endif 