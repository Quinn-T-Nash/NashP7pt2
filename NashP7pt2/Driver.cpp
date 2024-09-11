//Quinn Nash
//qnash@cnm.edu
//File: NashP7pt2
//Purpose: Read any analyze data from a CSV file

#include "Functions.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

int main()
{
	TechCompanyInfo techData[NUM_COMPANIES];
	vector<SectorInfo> sectorNames; //vector of structs to parse companies by sector

	stringstream ssReport;

	Header(ssReport);
	ssReport << setw(90) << "Analysis of the top 50 US Tech Companies for 2022-2023" 
		<< endl << endl;

	bool goodOpen{ true };
	//if data was opened, perform analysis
	if (goodOpen = ReadData(techData))
	{

		FillSectorInfo(techData, sectorNames);

		MostEmployeesBubbleSort(sectorNames, ssReport);

		DetermineSectorSizeEmpAndComp(sectorNames, ssReport);

		CalcIncomeTax(sectorNames, ssReport, techData);

		CalcRevOverMarkCap(sectorNames, ssReport);
		
		WriteReport(ssReport);

	}
	else
	{
		cout << FILENAME << "could not be opened" << endl;
	}

	return 0;

}