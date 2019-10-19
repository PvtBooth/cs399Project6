// StockDataGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <unordered_map>
#include <set>

namespace fs = std::filesystem;

int main()
{
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> dataList;
	std::set<std::string> dates;
	std::list<std::string> stocks;
	for (auto& p : fs::directory_iterator("data"))
	{
		std::ifstream in_file;
		in_file.open(p);
		if (!in_file.is_open())
		{
			std::cout << "ERROR READING FILE" << std::endl;
			return -1;
		}

		const std::string stockName = p.path().stem().string();
		if (dataList.find(stockName) != dataList.end())
			continue;
		stocks.push_back(stockName);
		dataList.insert(std::make_pair(stockName, std::unordered_map<std::string, std::string>()));
		std::string line, day, adj_price;
		while (std::getline(in_file, line)) // read whole line into line
		{
			std::istringstream iss(line); // string stream
			std::getline(iss, day, ','); // read first part up to comma, ignore the comma
			if (day.size() < 9)
				continue;
			dates.insert(day);
			std::getline(iss, adj_price, ','); // read first part up to comma, ignore the comma
			std::getline(iss, adj_price, ','); // read first part up to comma, ignore the comma
			std::getline(iss, adj_price, ','); // read first part up to comma, ignore the comma
			std::getline(iss, adj_price, ','); // read first part up to comma, ignore the comma

			dataList[stockName].insert(std::make_pair(day, adj_price));
		}
	}

	std::ofstream out_file;
	out_file.open("DATA.csv");

	// FIRST ROW
	out_file << "Date, ";
	auto pre_end = stocks.end(); pre_end--;
	for (auto it = stocks.begin(); it != stocks.end(); ++it)
	{
		if (it == pre_end)
			out_file << *it << std::endl;
		else
			out_file << *it << ", ";
	}

	// ALL OTHER ROWS
	for (auto it = dates.begin(); it != dates.end(); ++it)
	{
		out_file << *it << ", ";
		auto pre_end = stocks.end(); pre_end--;
		for (auto it2 = stocks.begin(); it2 != stocks.end(); ++it2)
		{
			if (it2 == pre_end)
				out_file << dataList[*it2][*it] << std::endl;
			else
				out_file << dataList[*it2][*it] << ", ";
		}
	}
	
	out_file.close();

	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
