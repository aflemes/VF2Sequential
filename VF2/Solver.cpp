#include "Solver.h"

void Solver::init(bool _isOutput)
{
	ofstream fout;
	fout.open("time.txt");
	fout.close();

	queryPath.clear();
	DBGraph.clear();
	QueryGraph.clear();
	outputPath.clear();

	isOutput = _isOutput;
}

void Solver::Dataset() {
	dbPath = "Data/mygraphdb.min.data";
	queryPath.push_back("Data/Q4.min.my");

	//dbPath = "Data/mygraphdb.data";
	//queryPath.push_back("Data/Q4.my");
	//queryPath.push_back("Data/Q8.my");
	//queryPath.push_back("Data/Q12.my");
	//queryPath.push_back("Data/Q16.my");
	//queryPath.push_back("Data/Q20.my");
	//queryPath.push_back("Data/Q24.my");

	// Test data set
	/*dbPath = "db.txt";
	queryPath.push_back("query.txt");*/
}

void Solver::ReadFile(string path, vector<Graph> &vec)
{
	vec.clear();

	ifstream fin;
	fin.open(path.c_str());

	Graph tmp;
	string buff;
	int n;
	int m, l;
	int p, q;
	while (getline(fin, buff))
	{
		//cout << buff << endl;
		//getchar();
		if (buff.size() == 0) continue;
		if (buff == "t # -1")
		{
			vec.push_back(tmp);
			break;
		}
		if (buff[0] == 't')
		{
			sscanf_s(buff.c_str(), "t # %d", &n);
			if (n == 0) continue;
			vec.push_back(tmp);
			tmp.init();
		}
		else if (buff[0] == 'v')
		{
			sscanf_s(buff.c_str(), "v %d %d", &m, &l);
			tmp.addv(m, l);
		}
		else if (buff[0] == 'e')
		{
			sscanf_s(buff.c_str(), "e %d %d %d", &p, &q, &l);
			tmp.adde(p, q, l);
		}
		else puts("Error!");
	}

	fin.close();
}

void Solver::ReadDB(string path)
{
	ReadFile(path, DBGraph);
}

void Solver::ReadQuery(string path)
{
	ReadFile(path, QueryGraph);
}

void Solver::input()
{
	ofstream fout;
	
	// Standard data set
	Dataset();

	// Output path: ./Output/ans_.txt
	_rmdir("Output");
	_mkdir("Output");
	string tt = "Output/ans";	
	for (int i = 0;i < (int)queryPath.size();i++)
		outputPath.push_back(tt + (char)(i + '1') + ".txt");

	
	for (int i = 0;i < (int)outputPath.size();i++)
	{
		fout.open(outputPath[i].c_str());
		fout.close();
	}

	ReadDB(dbPath);

	puts("Read Data Finished!");
}

void Solver::PrintQueryAns(int id, int cnt)
{
	ofstream fout;	
	fout.open(outputPath[id].c_str(), ios::app);

	fout << "+++++" << endl;
	fout << "Query ID: " << vf2.QueryID << endl;
	fout << "Count: " << vf2.match.size() << endl;

	// Not output match details
	fout << "Match List:" << endl;
	for (int i = 0;i < (int)vf2.match.size();i++)
		fout << vf2.QueryID << " " << vf2.match[i] << endl;

	// Output match details
	for (int i = 0;i < (int)vf2.match.size();i++)
	{
		fout << "-----" << endl;
		fout << "Graph ID: " << vf2.match[i] << endl;
		fout << "pat" << "\t" << "g" << endl;
		// Not output match details
		/*for (int j = 0;j < (int)vf2.match[i].s.size();j++)
			fout << vf2.match[i].s[i].first << "\t" << vf2.match[i].s[i].second << endl;*/
		fout << "-----" << endl;
	}

	fout << "+++++" << endl;

	fout << endl;

	fout.close();
}

void Solver::solve()
{
	int matches;
	time_t stTime, edTime;

	vf2.init(DBGraph);

	ofstream ftimeout;
	ftimeout.open("time.txt", ios::app);

	//Quantity of file input in Dataset
	for (int i = 0;i < (int)queryPath.size();i++)
	{
		ReadQuery(queryPath[i]);

		stTime = clock();
		
		for (int j = 0;j < (int)QueryGraph.size();j++)
		{
			cout << "Query " << queryPath[i] << ": " << j ;
			matches = vf2.vf2(QueryGraph[j], j, isOutput);
			
			if (isOutput) 
				PrintQueryAns(i, matches);

			cout << " Matchs found " << matches << endl;
		}
		
		edTime = clock();
		double dur = (double)(edTime - stTime) / 1000.0;

		ftimeout << queryPath[i] << " Time: " << dur << endl;
		cout << queryPath[i] << " Time: " << dur << endl;
	}

	ftimeout.close();
}

void Solver::output()
{
	//
}