//Quinn Nash
//qnash@cnm.edu
//File: NashP7pt2
//Purpose: Read any analyze data from a CSV file

#include "Functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

//WriteHeader
void Header(stringstream& r_ssReport)
{
	r_ssReport << "Name: Quinn Nash" << endl <<
		"Email: qnash@cnm.edu" << endl <<
		"Data: Statistics on the Top 50 US tech companies" << endl <<
		"Data supplied by : https://www.kaggle.com/datasets " << endl
		<< "Exact URL: " << endl <<
		"https://www.kaggle.com/datasets/lamiatabassum/top-50-us-tech-companies-2022-2023-dataset "
		<< endl << endl;
}

//Reads in the data from const FILENAME and fills array
bool ReadData(TechCompanyInfo techData[])
{
	bool goodOpen{ true };
	ifstream fileInTechData;
	string lineInfo;
	int row{ 0 }, col{ 0 };

	//open and read file, check good open
	fileInTechData.open(FILENAME);

	if (fileInTechData.is_open())
	{
		//first getline for the header line, do nothing with the header data
		getline(fileInTechData, lineInfo);

		//start a loop to read each row
		while (!fileInTechData.eof())
		{
			getline(fileInTechData, lineInfo);

			//convert lineInfo into stringstream to parse the data
			stringstream ssLineInfo(lineInfo);

			//loop through the ss and fill array with info
			//parse the csv where the ',' brings us to next column
			while (getline(ssLineInfo, lineInfo, ','))
			{
				if (col == 0)
				{
					techData[row].name = lineInfo;
				}
				else if (col == 2)
				{
					techData[row].sector = lineInfo;
				}
				else if (col == 4)
				{
					techData[row].yearFounded = stoi(lineInfo);
				}
				else if (col == 5)
				{
					techData[row].annualRevBillions = stof(lineInfo);
				}
				else if (col == 6)
				{
					techData[row].marketCapTrillions = stof(lineInfo);
				}
				else if (col == 8)
				{
					techData[row].incomeTaxBillions = stof(lineInfo);
				}
				else if (col == 9)
				{
					techData[row].numEmployees = stoi(lineInfo);
				}
				//increament col
				col++;
			}
			//move into next row or next company
			row++;
			//reset column to 0 for next iteration
			col = 0;
		}

		cout << FILENAME << " opened and read properly" << endl << endl;

		//close file
		fileInTechData.close();
	}
	else
	{
		//set good open to false and inform user file did not open
		goodOpen = false;
		cout << "Could not open " << FILENAME << endl << endl;
	}


	return goodOpen;
}


//Fills vector struct by unique sector/sectorTitle and groups together companies
//that have the same sector/sectorTitle
void FillSectorInfo(TechCompanyInfo techData[], vector<SectorInfo>& r_sectorNames)
{
	for (int i = 0; i < NUM_COMPANIES; i++)
	{
		//if vector is empty, we need to fill it with the first sector
		if (r_sectorNames.size() == 0)
		{
			IncreaseSectorVectorSize(i, techData, r_sectorNames);
		}
		else
		{
			//set size variable for when vector size increases mid loop
			int tempSectorNameSize = static_cast<int>(r_sectorNames.size());
			for (int j = 0; j < tempSectorNameSize; j++)
			{
				//if companies share a sector, they are grouped together in the
				//vector r_sectorNames at the same index
				if (techData[i].sector == r_sectorNames[j].sectorTitle)
				{
					r_sectorNames[j].companyInfo.push_back(techData[i]);
					/*
					the record at row i in our data, shares a sector
					with one of the sectors already in our vector r_sectorNames,
					So record at row i has been placed and 
					we move onto the next record to place
					(if we don't increment we get repeats)
					*/
					j = tempSectorNameSize;
					continue;
				}
				/*
				we reached the end of r_sectorNames without a match
				means we have a new sector name to add
				(subtract 1 to match  0 index to how .size() counts)
				*/
				else if (j == r_sectorNames.size() - 1)
				{
					IncreaseSectorVectorSize(i, techData, r_sectorNames);
				}
			}
		}
	}

}

//push_back a unique sector to the vector r_sectorNames
void IncreaseSectorVectorSize(int i, TechCompanyInfo techData[], vector<SectorInfo>& r_sectorNames)
{
	//create a temp variable to hold sectorTitle and companyInfo
	SectorInfo tempSector;
	tempSector.sectorTitle = techData[i].sector;
	tempSector.companyInfo.push_back(techData[i]);

	//place the temp vairable into the vector<SectorInfo>
	r_sectorNames.push_back(tempSector);
}


//Finds the company from each sector with the most employees, sends results to stringstream
void MostEmployeesBubbleSort(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport)
{
	r_ssReport << setw(92) << "The companies in each sector with the most employees are:" << endl << endl;

	//loop through each unique sector aka each index of r_sectornames
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		if (r_sectorNames[i].companyInfo.size() == 1)
		{
			//if one company, do nothing, no need to sort
		}
		else
		{
			//loop through each company in the given sector
			for (int j = 0; j < r_sectorNames[i].companyInfo.size() - 1; j++)
			{
				//else bubblesort order companies most employees to least employes in r_sectorNames
				for (int k = 0; k < r_sectorNames[i].companyInfo.size() - j - 1; k++)
				{
					if (r_sectorNames[i].companyInfo[k].numEmployees < r_sectorNames[i].companyInfo[k + 1].numEmployees)
					{
						swap(r_sectorNames[i].companyInfo[k], r_sectorNames[i].companyInfo[k + 1]);					
					}
				}
			}
		}
		//Cout the info to the user
		r_ssReport << "Sector: " << r_sectorNames[i].sectorTitle <<
			"\n - " << r_sectorNames[i].companyInfo[0].name << " founded in " <<
			r_sectorNames[i].companyInfo[0].yearFounded << "\n\tTotal employees: " <<
			r_sectorNames[i].companyInfo[0].numEmployees << endl << endl;
	}
}

//Find the Sector with the most companies, and the sector with the most employees
void DetermineSectorSizeEmpAndComp(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport)
{
	int indexEmp{ 0 }, indexComp{ 0 }, tempLargest{ 0 }, mostCompanies{ 0 };

	//create a fill vector with total employees in each sector
	//same sector has same index number in r_sectorNames and sectorTotalEmp
	vector<int> sectorTotalEmp;
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		int totalEmps{ 0 };

		for (int j = 0; j < r_sectorNames[i].companyInfo.size(); j++)
		{
			totalEmps += r_sectorNames[i].companyInfo[j].numEmployees;
		}
		
		sectorTotalEmp.push_back(totalEmps);
	}

	//find the index of sectorTotalEmp that is the largest
	tempLargest = sectorTotalEmp[0];
	for (int i = 0; i < sectorTotalEmp.size(); i++)
	{
		if (sectorTotalEmp[i] > tempLargest)
		{
			tempLargest = sectorTotalEmp[i];
			indexEmp = i;
		}
	}

	//find the sector with the most companies
	mostCompanies = static_cast<int>( r_sectorNames[0].companyInfo.size() );
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		if (static_cast<int>( r_sectorNames[i].companyInfo.size() ) > mostCompanies)
		{
			mostCompanies = static_cast<int>(r_sectorNames[i].companyInfo.size());
			indexComp = i;
		}
	}

	r_ssReport << "Sector with the most EMPLOYEES:\n" << 
		setw(51) << r_sectorNames[indexEmp].sectorTitle <<
		" with " << tempLargest << " total employees." << endl;

	r_ssReport << "Sector with the most COMPANIES:\n" <<
		setw(51) << r_sectorNames[indexComp].sectorTitle <<
		" with " << mostCompanies << " total companies.\n" << endl << endl;
}

/*
Find the total income tax each sector paid. Find the
highest and lowest ratio of income tax divide by revenue for each sector
(ignore compnaies who paid negative income tax and place them in a list)
*/
void CalcIncomeTax(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport, TechCompanyInfo techData[])
{
	
	vector<TechCompanyInfo> negativeTaxPayers;
	vector<float> sectorTaxSum, sectorHighestTaxRevRatio, sectorLowestTaxRevRatio;
	vector<int> indexOfCompanyWithHighestRatio, indexOfCompanyWithLowestRatio;
	float avgRatio{ 0.0 };

	//fill vetor with taxes paid by each sector(ignore and place negatives)
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		//taxes paid only in this for loop and resets to 0 each sector
		float taxesPaid{ 0.0 };
		
		for (int j = 0; j < r_sectorNames[i].companyInfo.size(); j++)
		{
			if (r_sectorNames[i].companyInfo[j].incomeTaxBillions < 0)
			{
				negativeTaxPayers.push_back(r_sectorNames[i].companyInfo[j]);
			}
			else
			{
				taxesPaid += r_sectorNames[i].companyInfo[j].incomeTaxBillions;
			}
		}

		sectorTaxSum.push_back(taxesPaid);
	}

	FillHighestTaxRevRatioVectors(r_sectorNames, indexOfCompanyWithHighestRatio, sectorHighestTaxRevRatio);

	//set decimal precision to 0.00 in stringstream
	r_ssReport.setf(ios::fixed);
	r_ssReport.precision(2);

	r_ssReport << setw(100) << "Companies in each sector with the Highest Ratio of Income Tax divided by Revenue:\n" << endl;

	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		r_ssReport << r_sectorNames[i].sectorTitle << ":\n\t"
			<< r_sectorNames[i].companyInfo[indexOfCompanyWithHighestRatio[i]].name << " paid " <<
			sectorHighestTaxRevRatio[i] << "% of its total Revenue as Income Tax" << endl;
	}

	FillLowestTaxRevRatioVectors(r_sectorNames, indexOfCompanyWithLowestRatio, sectorLowestTaxRevRatio);

	r_ssReport << endl << setw(100) << "Companies in each sector with the Lowest Ratio of Income Tax divided by Revenue:\n" << endl;

	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		r_ssReport << r_sectorNames[i].sectorTitle << ":\n\t"
			<< r_sectorNames[i].companyInfo[indexOfCompanyWithLowestRatio[i]].name << " paid " <<
			sectorLowestTaxRevRatio[i] << "% of its total Revenue as Income Tax" << endl;
	}

	r_ssReport << endl << 
		setw(100) << "If the same company paid both highest and lowest, it is the only company in that sector." << endl;

	AverageTaxRevRatioAll(techData, &avgRatio);

	ShameNegativeTaxPayers(negativeTaxPayers, avgRatio, r_ssReport);

}

void FillHighestTaxRevRatioVectors(vector<SectorInfo>& r_sectorNames, vector<int>& r_indexOfCompanyWithHighestRatio,
	vector<float>& r_sectorHighestTaxRevRatio)
{
	//variables decalred in loop as they only exisit in the loop

	//find the compnay in each sector with highest Ratio of incometax divided by revenue
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		int companyIndex{ -1 };
		float ratioTaxRev{ 0.0 }, percentTaxRev{ 0.0 };
		// make tempRev = 1 so we get 0/1=0 and not 0/0 for condition check of if in the else
		float tempTax{ 0.0 }, tempRev{ 1.0 };

		for (int j = 0; j < r_sectorNames[i].companyInfo.size(); j++)
		{

			if (r_sectorNames[i].companyInfo.size() == 1)
			{
				tempTax = r_sectorNames[i].companyInfo[0].incomeTaxBillions;
				tempRev = r_sectorNames[i].companyInfo[0].annualRevBillions;
				companyIndex = 0;
			}
			else
			{
				//tempTax2/Rev2 only exisit in this else block to check for highest ratio
				float tempTax2 = r_sectorNames[i].companyInfo[j].incomeTaxBillions;
				float tempRev2 = r_sectorNames[i].companyInfo[j].annualRevBillions;

				if ((tempTax / tempRev) < (tempTax2 / tempRev2))
				{
					tempTax = tempTax2;
					tempRev = tempRev2;
					companyIndex = j;
				}
			}
		}
		//place index of the company with the highest ratio 
		//index of vector matches sector indexes, int value at index tells which index 
		//the company is at in r_sectorNames.companyInfo[]
		r_indexOfCompanyWithHighestRatio.push_back(companyIndex);

		//place highest ratio in vector for higest ratio
		ratioTaxRev = tempTax / tempRev;
		percentTaxRev = ratioTaxRev * 100;
		r_sectorHighestTaxRevRatio.push_back(percentTaxRev);
	}
}

//finds company in each sector with lowest tax divide by rev ratio
void FillLowestTaxRevRatioVectors(vector<SectorInfo>& r_sectorNames, vector<int>& r_indexOfCompanyWithLowestRatio,
	vector<float>& r_sectorLowestTaxRevRatio)
{
	//variables decalred in loop as they only exisit in the loop

	//find the compnay in each sector with lowest Ratio of incometax divided by revenue
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		int companyIndex{ -1 };
		float ratioTaxRev{ 0.0 }, percentTaxRev{ 0.0 };
		// make tempTax 1 and tempRev 1 so 1/1 is ways highest in first comparison
		float tempTax{ 1.0 }, tempRev{ 1.0 };

		for (int j = 0; j < r_sectorNames[i].companyInfo.size(); j++)
		{

			if (r_sectorNames[i].companyInfo.size() == 1)
			{
				tempTax = r_sectorNames[i].companyInfo[0].incomeTaxBillions;
				tempRev = r_sectorNames[i].companyInfo[0].annualRevBillions;
				companyIndex = 0;
			}
			else
			{
				//tempTax2/Rev2 only exisit in this else block to check for lowest ratio
				float tempTax2 = r_sectorNames[i].companyInfo[j].incomeTaxBillions;
				float tempRev2 = r_sectorNames[i].companyInfo[j].annualRevBillions;

				//ignore all ratios that would be negative
				if ((tempTax2 / tempRev2) < 0)
				{
					//go to next company in sector or next interation of j for loop
					continue;
				}
				else if ((tempTax / tempRev) > (tempTax2 / tempRev2))
				{
					tempTax = tempTax2;
					tempRev = tempRev2;
					companyIndex = j;
				}
			}
		}
		//place index of the company with the highest ratio 
		//index of vector matches sector indexes, int value at index tells which index 
		//the company is at in r_sectorNames.companyInfo[]
		r_indexOfCompanyWithLowestRatio.push_back(companyIndex);

		//place highest ratio in vector for higest ratio
		ratioTaxRev = tempTax / tempRev;
		percentTaxRev = ratioTaxRev * 100;
		r_sectorLowestTaxRevRatio.push_back(percentTaxRev);
	}
}

//find the Average ratio of income tax divide by revenue for all the top 50 companies
void AverageTaxRevRatioAll(TechCompanyInfo techData[], float* p_avgRatio)
{
	float totalRev{ 0.0 }, totalTax{ 0.0 };

	for (int i = 0; i < NUM_COMPANIES; i++)
	{
		totalRev += techData[i].annualRevBillions;
	}

	for (int i = 0; i < NUM_COMPANIES; i++)
	{
		totalTax += techData[i].incomeTaxBillions;
	}

	*p_avgRatio = totalTax / totalRev;
}

//Display the companies who paid negative tax, and suggest what they should have paid
void ShameNegativeTaxPayers(vector<TechCompanyInfo>& r_negativeTaxPayers, float avgRatio,
	stringstream &r_ssReport)
{
	r_ssReport << "\n\n" << setw(62) << "The Average Income Tax paid by all " << NUM_COMPANIES <<
		" top companies was " << avgRatio << "%" << endl << endl
		<< setw(85) << "But The following companies paid negative Income Tax...\n"
		<< setw(65) << "SHAME!!!!!!!\n" << endl;
	for (int i = 0; i < r_negativeTaxPayers.size(); i++)
	{
		r_ssReport << r_negativeTaxPayers[i].name << ", Shame on you!" << endl;
		r_ssReport << setw(25) << "-You should have paid " <<
			r_negativeTaxPayers[i].annualRevBillions * avgRatio << " BILLION USD in Income Tax." << endl;
	}

	r_ssReport << endl << endl;
}

//find the higest revenue divide by market cap for each company
void CalcRevOverMarkCap(vector<SectorInfo>& r_sectorNames, stringstream& r_ssReport)
{
	vector<float> sectorRevMarkCapRatio;
	vector<int> sectorHighestCompanyIndex;

	int companyIndex{ -1 };
	float ratioRevMarkCap{ 0.0 }, percentRevMarkCap{ 0.0 };
	// make tempRev 0 so 0/1 is always lowest ratio
	float tempRev{ 0.0 }, tempMarkCap{ 1.0 };

	for (int i = 0; i < r_sectorNames.size(); i ++ )
	{
		for (int j = 0; j < r_sectorNames[i].companyInfo.size(); j++)
		{
			if (r_sectorNames[i].companyInfo.size() == 1)
			{
				tempRev = r_sectorNames[i].companyInfo[0].annualRevBillions;
				tempMarkCap = r_sectorNames[i].companyInfo[0].marketCapTrillions;
				companyIndex = 0;
			}
			else
			{
				//temp 2 variabes only exisit in this else block to check for hioghest ratio
				float tempRev2 = r_sectorNames[i].companyInfo[j].annualRevBillions;
				float tempMarkCap2 = r_sectorNames[i].companyInfo[j].marketCapTrillions;
				
				//if higher ratio found, set higher ratio variabhles and index
				if ((tempRev / tempMarkCap) < (tempRev2 / tempMarkCap2))
				{
					tempRev = tempRev2;
					tempMarkCap = tempMarkCap2;
					companyIndex = j;
				}
			}
		}

		//Adjust tempMarkCap for trillions into billions so we can write ratio in billions
		tempMarkCap = tempMarkCap * 1000;

		//place index of companyInfo
		sectorHighestCompanyIndex.push_back(companyIndex);

		//place what the ratio is
		ratioRevMarkCap = tempRev / tempMarkCap;
		percentRevMarkCap = ratioRevMarkCap * 100;
		sectorRevMarkCapRatio.push_back(percentRevMarkCap);
	}

	r_ssReport << setw(100) << "Companies from each sector with the highest ratio of Revenue divided by Market Cap:\n" << endl;
	for (int i = 0; i < r_sectorNames.size(); i++)
	{
		r_ssReport << r_sectorNames[i].sectorTitle << ":\n\t"
			<< r_sectorNames[i].companyInfo[sectorHighestCompanyIndex[i]].name
			<< " with " << sectorRevMarkCapRatio[i] << "%" << endl;
	}

}

//write report to file, specified by the user
void WriteReport(stringstream& r_ssReport)
{
	bool goodOpen{ true };

	ofstream writtenReport;
	string reportFile{ "" };
	
	//repeat until we get a file that can be written to
	do
	{
		cout << "What file would you like to write the report too? " << endl;
		cout << "Must be a .txt file: i.e TestFile.txt" << endl;
		cout << "-Please enter the name of the file below..." << endl;
		getline(cin, reportFile);

		writtenReport.open(reportFile);

		if (writtenReport.is_open())
		{
			//if file was opened, write the ss we built to the file
			writtenReport << r_ssReport.str();

			writtenReport.close();

			//cout to console as well
			cout << "When you open your file, the report should look as follows:\n" << endl;
			cout << r_ssReport.str();
		}
		else
		{
			goodOpen = false;
			cout << reportFile << " could not open" << endl;
			cout << "Try a different filename to write your report" << endl;
		}
	} while (goodOpen == false);
}