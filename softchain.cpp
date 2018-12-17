//g++ -o backchain.exe backchain7.cpp -m64 -std=c++11 -Os -fexpensive-optimizations
/*
 * Airlock Blockchain v1.0
 * Author: A. David Pulse, Jr.
 * Last Edit: 2/26/2018
 * Copyright 2018
*/
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <ratio>
#include <ctime>
#include <cstring>
#include <bitset>
#include <random>
#include <math.h>
#include <iomanip>
#include <iterator>
#include <set>
#include <ctype.h>
#include <exception>
#ifndef RAND_MAX
RAND_MAX = 57
#endif

using namespace std;

class air {

	public:
	const int byte_size = 256;
	const int MAX_ALL = 7;
	int MAX_Q = MAX_ALL;
	int SYS_ARCH_WA = 60;
	int HEX_TAX = 0;
	const string accts = "wifi.out";
	string transfile = "transact.bcw";
	string backups = "backups.ini";
	bool quiet = 0;
	vector<int> answers, results, mistakes, miscounts;
	vector<long long int> tachometer;
	std::chrono::system_clock::time_point t0;
	int clntSocket, clntBSocket, clntSock, clntBSock;
	const long MAX = 10000;
	long smtc = MAX;
	long wallets = 0;
	string all_star = "";
	string token = "";
	string buffer = "";
	string hex4 = "0123456789abcdefghiklmnopqstuvwxyzABDEFGHIJKLMNOPQRTUVXY!@#$%&-_+*";
	string curr_wallet = "";
	void createTransaction(string, fstream&);
	bool makePrep(fstream&);
	bool idleMethod(string);
	bool airAlgo(long long int);
	inline bool recIdle(string line) {
		long long int total = 0;
		long long int powr = 1;
		for (int i = 0 ; i < line.length() ; i++) {
			powr = 1;
			for (int u = 0 ; u < i ; u++)
				powr *= byte_size;
			total = total + (powr * line.at(i));
		}
		long long int bitbyte = total;
		tachometer.push_back(bitbyte);
	}
	void callMistakes(int,int);
	bool dcmpChain(string);
	bool airOut(fstream&);
//	bool findWallet(long, bool);
	long countWallet(int);
	void restoreShell();
	void Restore(string);
	void Backup(string);
	void moveBackup(string, string);
	bool newWallet(long);
	bool checkRemote(fstream&);
	void remoteShell(string);
	long cleanUpAddrs(string);
	long long int stoh4(string lin);
	const string cache = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	air();
	~air();

} x;

air::air() {
	t0 = chrono::system_clock::now();
}

air::~air() {
}

long long int air::stoh4(string lin) {
	long long int q = 0;
	int y = 0;
	for (int i = lin.length() ; i > 0 ; i--) {
		for (char x : hex4) {
			if (x == lin[i-1]) {
				q <<= 6;
				q = q + y%64;
				break;
			}
			y++;
		}
		y = 0;
	}
	return q;
}

long air::cleanUpAddrs(string fn = "") {
	set<string> hold {};
	string tmpplace = "";
	if (fn == "")
		fn = accts;
	fstream fsn (fn.c_str(), ios::in);
	long cnt = countWallet(1);
	while (!fsn.eof()) {
		do {
			if (isdigit(fsn.peek()))
				tmpplace.append((size_t)1, fsn.get());
			else if (fsn.peek() != ';')
				fsn.ignore();
		} while (!fsn.eof() && fsn.peek() != ';');
		fsn.ignore();
		hold.insert(tmpplace);
		tmpplace = "";
	}

	fsn.close();
	fsn.open(fn.c_str(), ios::out | ios::trunc);
	for (set<string>::iterator i = hold.begin() ; i != hold.end() ; i++) {
		fsn << *i << ';';
	}
	if (quiet == 0 && fn == accts && cnt != hold.size()-1) {
		cout << "Set included " << cnt - hold.size() << " Duplicates. Returned new count of ";
	}
	return cnt-hold.size();
}

void air::remoteShell(string json) {
	fstream tokn (x.curr_wallet, ios::in | ios::out | ios::app | ios::binary);
	if (tokn.is_open()) {
		send(x.clntSock, "diff", 4, 0);
		checkRemote(tokn);
		tokn.seekg(0);
		createTransaction(json, tokn);
	}
	if (tokn.is_open())
		tokn.close();
	return;
}

bool air::checkRemote(fstream& servWallet) {

	servWallet.seekg(0);
	string tmpSw = "";
	while (servWallet.peek() != EOF)
		tmpSw.append((size_t)1,servWallet.get());
	ssize_t numBytesSent = 0;
	while (numBytesSent < tmpSw.length()) {
		string sc = "";
		if (tmpSw.length() - numBytesSent - 10000 <= 0)
			sc = tmpSw.substr(numBytesSent, tmpSw.length());
		else
			sc = tmpSw.substr(numBytesSent, 10000);
		numBytesSent += send(x.clntSock, sc.c_str(), sc.length(), 0);
	}
	return 1;
}

void air::restoreShell() {
	set<string> dates;
	char yn = 'n';
	int go = 0;
	cout << "Airlock Repair / Backup\n";
	cout << "\nAre you sure? You're going to erase all of your wallets.\n";
	cout << "This could take awhile, also.. please enter 'Y' (uppercase) to begin, or anything else to quit\n";
	cin >> yn;
	if (yn != 'Y')
		exit(0);
	cout << "Verbose? (y/n)" << flush;
	cin >> yn;
	if (yn == 'n')
		quiet = 1;
	long cnt = countWallet(0);
	cout << "\nErasing all wallets!!\n";
	while (cnt > go) {
		try {
//			findWallet(go, 1);
			fstream fin (curr_wallet.c_str(), ios::in);
			if (fin.is_open()) {
				fin.close();
				remove(curr_wallet.c_str());
			}
			go++;
			fin.close();
		} catch (...) {
			go++;
		}
	}
	cout << "Wallets will be restored in ascending order.";
	while (1) {
		string date = "";
		cout << "\nEnter Date (YEARMMDD 8 digits) to add to repair state (0 to stop)\n> ";
		cin >> date;
		int xm = 0;
		if (date == "0") { }
		else if (date.length() != 8) {
			cout << "Date argument must be YEARMMDD (8 digits)\n";
			continue;
		}
		for (auto r : date) {
			if (!isdigit(r))
				xm = -1;
		}
		if (xm == (-1)) {
			cout << "Filename syntax error: YEARMMDD\n";
			continue;
		}
		cout << "You Entered " << date << " Correct?\n";
		cout << "(y/n, q to quit without restore, s to restore)\n> ";
		cin >> yn;
		if (yn == 'n' || yn == 'N')
			continue;
		else if (yn == 'y' || yn == 'Y') {}
		else if (yn == 's' || yn == 'S') { break; }
		else if (yn == 'q' || yn == 'Q') return;
		else { cout << "Unknown command, file not added"; continue; }
		dates.insert(date);
	}

	for (set<string>::iterator i = dates.begin() ; i != dates.end() ; i++)
		Restore(*i);

	fstream tmp ("CURRFILE", ios::out);
	tmp << "";
	tmp.close();
	return;
}

void air::moveBackup(string date, string curr = "CURRFILE") {
	fstream fin (curr.c_str(), ios::in);
	if (!fin.is_open()) {
		cout << "No Transaction Data File";
		return;
	}
	fstream fout (date.c_str(), ios::in | ios::app);
	if (!fout.is_open()) {
		rename(curr.c_str(), date.c_str());
		return;
	}
	else
		fout.close();
	fout.open(date.c_str(), ios::out | ios::app);
	fout << fin.rdbuf();
	fin.close();
	fout.close();
	remove(curr.c_str());
	return;
}

void air::Backup(string date = "CURRFILE") {
	fstream fin(transfile.c_str(), ios::in);
	if (!fin.is_open()) {
		cout << "No Transaction Data File\n";
		return;
	}
	fstream fout(date.c_str(), ios::out | ios::app);
	fout << fin.rdbuf();
	fin.close();
	fout.close();
	return;
}

void air::Restore(string n) {

	if (n.length() != 8) {
		cout << '\n';
		return;
	}
	fstream fin (n.c_str(), ios::in);
	if (fin.is_open()) {
		bool success = dcmpChain(n);
		cout << "\nTransactions in File " << n << " Restored To Wallets\n" << flush;
	}
	else
		cout << "\nFile " << n << " Does not exist\n" << flush;

	fin.close();
	return;
}

long air::countWallet(int i = 0) {
	fstream fin (accts.c_str(), ios::in);
	long It = 0;
	while (1) {
		do {
			if (fin.peek() == ';')
				It++;
			fin.ignore();
		} while (!fin.eof());
		if (i == 0 && (fin.peek() == EOF || fin.eof())) {
			cout << It << " Wallets Exist" << flush;
			return It;
		}
		else if (fin.peek() == EOF || fin.eof()) {
			fin.close();
			return It;
		}
	}
	return It;
}

bool air::airOut(fstream& fout) {
	int j = 0, i = 0, h = 0, r = 0, t = 0;
	HEX_TAX = 0;
	int y = 0;
	int HEX_TACK = 0;
	string out_str {};
	while (t < tachometer.size()) {
		do {
			out_str.push_back(hex4.at(tachometer[t]%64));
			tachometer[t] >>= 6;
		} while (tachometer[t] > 0);
		for (auto r : out_str)
			fout << r;
		if (out_str.length() > 0) {
			fout << ';';
			fout << hex << mistakes[t] << 'j' << miscounts[t] << 'r';
		}
		t++;
		out_str.clear();
	}
	return 0;
}

bool air::airAlgo (long long int Ti) {
	long long int n = Ti%byte_size, m = 0;
	results.push_back(Ti%byte_size);
	int i = 0;
	while (results.size() < MAX_Q) {
		int j = 0;
		for (int x : results) {
			long long powr = 1;
			for (int u = 0 ; u < j ; u++)
				powr *= byte_size;
			m = m + (powr * x);
			j++;
		}
		n = Ti;
		n -= m;
		m = 0;

		for (int o = 0 ; o < results.size() ; o++)
			n = n/byte_size;
		results.push_back(n%byte_size);
	}
	return 1;
}

bool air::idleMethod(string line) {
	unsigned long long int liner = 0;
	answers.clear();
	for (auto x : line)
		answers.push_back((int)x);

	bool p = recIdle(line);
	airAlgo(tachometer[tachometer.size()-1]);
	int r = 0;
	bitset<8> mistake = 0, miscount = 0;
	for (int x : results) {
		if ((char)x == (char)answers[r]) { }
		else if ((char)(x + 1) == (char)answers[r])
			mistake[r] = 1;
		else if ((char)(x - 1) == (char)answers[r])
			miscount[r] = 1;
		else {
			for (int e : answers)
				cout << e << " ";
			cout << endl;
			for (int e : results)
				cout << e << " ";
			cout << endl;
			cout << "Byte error: " << x << "!=" << answers[r] << " " << mistakes.size() << " " << tachometer[tachometer.size()-1] << flush;
			return 0;
		}
		r++;
	}
	mistakes.push_back(mistake.to_ulong());
	miscounts.push_back(miscount.to_ulong());
	return 1;
}

bool air::makePrep(fstream& fout) {

	string limes = "", h = "";

	while (all_star != "") {
		MAX_Q = MAX_ALL;
		if (MAX_ALL > all_star.length())
			MAX_Q = all_star.length();
		limes = all_star.substr(0,MAX_Q);

		h = "";
		bool p = idleMethod(limes);
		if (!p) {
			cout << "Error";
			exit(0);
		}
		results.clear();
		all_star = all_star.substr(MAX_Q, all_star.length());
		if (all_star == "") {
			return 1;
		}
	}
	return 1;
}

void air::callMistakes(int mist, int misc) {
	if (mist == 0 && misc == 0)
		return;
	bitset<8> redo = mist, doover = misc;
	for (int r = 0 ; r < MAX_Q ; r++) {
		if (doover[r] == 1)
			results[r]--;
		if (redo[r] == 1)
			results[r]++;
	}
	return;
}

bool air::dcmpChain(string tmpstr = "") {
	Backup();
	if (tmpstr == "") {
		tmpstr = transfile;
	}
	string line = "", width = "";
	long long int liner = 0, total = 0;
	bool td = 1;
	int a1 = 0, a2 = 0;
	string a = "";
	long filesize = 0;
	fstream fin(tmpstr.c_str(), ios::in);
	if (!fin.is_open())
		return 0;
	fin.seekg(0,ios::end);
	int len = fin.tellg();
	fin.seekg(0);
	if (len == 0) return 0;
	fin.setf(std::ios::hex);
	fstream nout;
	fstream tout;
	if (quiet == 0)
		cout << "ID\t\tWallet\t\t\t\t\t\t\t\t\tLength" << endl;
	while (!fin.eof() && fin.peek() != EOF) {
	// get largess

		// Get wallet, prepare for transaction listing
		if ((char)fin.peek() == 'S') {
			fin.ignore();
			do {
				line.append((size_t)1,fin.get());
			} while ((char)fin.peek() != 'W');
			int x = stoi(line,nullptr,10);
			bool p = 1;
			if (!p)
				curr_wallet = "error_Al.log";
			nout.close();

			nout.open(curr_wallet.c_str(), ios::out | ios::app);
			nout.seekp(0);
			if (quiet == 0)
				cout << line << "\t\t" << curr_wallet << "\t";
			tout.open("tmp_90.tmp", ios::out);
			tout << nout.rdbuf();
			line = "";
		}

		// Get transaction size in bytes
		if ((char)fin.peek() == 'W') {
			filesize = 0;
			fin.ignore();
			do {
				width.append((size_t)1,fin.get());
			} while ((char)fin.peek() != 'C');
			fin.ignore();
			total = stoull(width,nullptr,10);
			if (quiet == 0)
				cout << width << endl;
			width = "";
		}
		string chars = "";
		do {
			chars.append((size_t)1,fin.get());
		} while (fin.peek() != ';');
		liner = stoh4(chars);
		fin.ignore();
		do {
			a.append((size_t)1,fin.get());
		} while (fin.peek() != 'j');
		a1 = stoi(a,nullptr,16);
		fin.ignore();
		a = "";
		do {
			a.append((size_t)1,fin.get());
		} while (fin.peek() != 'r');
		a2 = stoi(a,nullptr,16);
		fin.ignore();
		a = "";
		if ((char)fin.peek() == 'Z') {
			fin.ignore();
			MAX_Q = total - filesize;
		}
		airAlgo(liner);
		callMistakes(a1,a2);
		a2 = a1 = liner = 0;
		line = "";
		nout.seekp(ios::end);
		for (char res : results)
			nout.put(res);

		filesize += MAX_Q;
		results.clear();
		if (filesize == total) {
			tout.seekg(0);
			nout.seekp(ios::end);
			nout << tout.rdbuf();
			//nout << endl;
			tout.close();
			tout.open ("tmp_90.tmp", ios::out | ios::in | ios::trunc);
			tout << "";
			MAX_Q = MAX_ALL;
		}
	}
	fin.close();
	tout.close();
	if (tmpstr == transfile)
		remove(tmpstr.c_str());
	remove("temp_90.tmp");
	return 1;
}

void air::createTransaction(string json, fstream& fout) {

	string line = "";
	bool gz = 0;
	long smtc = json.length();
	auto system_start = chrono::system_clock::now();
	int v = 0, i = 0;
	fout << 'W' << smtc << 'C';

	do {
	// I need readsome here! //
		do {
			all_star.append((size_t)1,json[0]);
			if (json.length() > 1)
				json = json.substr(1,json.length());
			else
				json = "";
			i++;
		} while (json.length() > 0);
		makePrep(fout);
		airOut(fout);
		tachometer.clear();
		mistakes.clear();
		miscounts.clear();
		gz = 0;
		if (json.length() == 0) {
			gz = 1;
			std::chrono::duration<double,std::ratio<1>> diff = chrono::system_clock::now() - system_start;
			if (quiet == 0) {
				cout << diff.count() << " seconds passed\t" << (long)i << " Bytes added to chain" << endl << flush;
			}
		}
	} while (json.length() > 0);
	makePrep(fout);
	airOut(fout);
	if (!gz && quiet == 0) {
		std::chrono::duration<double,std::ratio<1>> diff = chrono::system_clock::now() - system_start;
		cout << diff.count() << " seconds passed\t" << (long)i << " Bytes added to chain" << endl << flush;
		cout << "Data has been successfully added to the blockchain. .. . .";
	}
	return;
}
/*
int main(int argc, char * argv[]) {
	int arg = 0;
	const char * wp = "--create-wallets";
	const char * dc = "--disperse";
	const char * cp = "-c";
	const char * wn = "-n";
	const char * fw = "-x";
	const char * bk = "--backup-transactions";
	const char * rp = "--restore-mode";
	const char * rf = "--restore-backups";
	const char * qm = "--quiet";
	const char * mv = "--move-backup";
	const char * bl = "--list-backups";
	for (int i = 1 ; i < argc ; i++) {
		if (strcmp(argv[i],qm) == 0 && i < 2) {
			arg = i;
			x.quiet = 1;
		}
		else if (strcmp(argv[i],qm) == 0)
			x.quiet = 1;
	}
	if (x.quiet == 0)
		cout << "Airlock Blockchain v1.0\nBy Inc2 Software (C) 2018\n" << flush;
	if (argc >= 4 && strcmp(argv[arg+1],cp) == 0) {
		string m = argv[arg+3];
		int xm = 0;
		for (auto r : m) {
			if (!isdigit(r))
				xm = -1;
		}
		if (xm != (-1))
			xm = stoi(m,nullptr,10);
		else {
			cout << "Arguments to -c must be \"file.ext\" int";
			exit(0);
		}
		fstream fin (argv[arg+2], ios::in | ios::binary);
		if (!fin) {
			printf("No input file");
			exit(-1);
		}
		fstream fout (x.transfile.c_str(), ios::out | ios::app | ios::binary);
		if (!fout) {
			printf("No output file");
			fin.close();
			exit(-1);
		}	
		x.createTransaction(fin,fout,m);
		fin.close();
		fout << "Z";
		fout.close();
	}
	else if (argc >= 3 && strcmp(argv[arg+1],wp) == 0) {
		int xm = 0;
		string m = argv[arg+2];
		for (auto r : m) {
			if (!isdigit(r))
				xm = -1;
		}
		if (xm != (-1))
			xm = stoi(m,nullptr,10);
		else {
			cout << "Argument to -w must be integer";
			exit(0);
		}
		x.newWallet(xm);
		cout << xm << " Attempted Wallets Creation\n";
		x.cleanUpAddrs(x.accts);
		x.countWallet(0);
		std::chrono::duration<double,ratio<1,1000000000>> diff = chrono::system_clock::now() - x.t0;
		auto mt = std::chrono::duration_cast<chrono::seconds>(diff);
		cout << "\nWallets created, waxed clean of duplicates, and counted again in " << mt.count() << " seconds.";
	}
	else if (argc >= 3 && (strcmp(argv[arg+1],bk) == 0 || strcmp(argv[arg+1],mv) == 0)) {
		int xm = 0;
		string m = argv[2];
		if (m.length() != 8) {
			cout << "Date argument must be YEARMMDD (8 digits)";
			return 0;
		}
		for (auto r : m) {
			if (!isdigit(r))
				xm = -1;
		}
		if (xm == (-1)) {
			cout << "Argument to -b must be all integers";
			return 0;
		}
		x.moveBackup(m);
		xm = 0;
		string tmpplace = "";
		fstream fsn (x.backups.c_str(), ios::in);
		while (!fsn.eof()) {
			tmpplace = "";
			do {
				tmpplace.append((size_t)1, fsn.get());
			} while (!fsn.eof() && fsn.peek() != ';');
			fsn.ignore();
		}
		fsn.close();
		fsn.open(x.backups.c_str(), ios::out | ios::app);
		fsn << m << ';';
		fsn.close();
	}
	else if (argc >= 2 && strcmp(argv[arg+1],dc) == 0) {
		bool p = x.dcmpChain(x.transfile);
		remove(x.transfile.c_str());
	}
	else if (argc >= 2 && strcmp(argv[arg+1],rf) == 0) {
		int cnt = x.countWallet(1);
		int go = 0, xm = 0;
		if (argc == 2)
			cout << "Deleting Wallet contents...\n";
		while (cnt > go) {
			try {
				x.findWallet(go, 1);
				remove(x.curr_wallet.c_str());
				go++;
			} catch (exception e) {
				printf("%e ",std::exception(e));
				go++;
			}
		}
		set<string, less<string>> hold {};
		string tmpplace = "";
		fstream fsn (x.backups.c_str(), ios::in);
		while (!fsn.eof()) {
			do {
				tmpplace.append((size_t)1, fsn.get());
			} while (!fsn.eof() && fsn.peek() != ';');
			fsn.ignore();
			hold.insert(tmpplace);
			tmpplace = "";
		}
		fsn.close();
		for (set<string>::reverse_iterator i = hold.rbegin() ; i != hold.rend() ; i++) {
			x.Restore(*i);
		}
		cout << "Restore Completed";
	}
	else if (argc >= 2 && strcmp(argv[arg+1],wn) == 0)
		x.countWallet();
	else if (argc >= 2 && strcmp(argv[arg+1],rp) == 0)
		x.restoreShell();
	else if (argc >= 3 && strcmp(argv[arg+1],fw) == 0) {
		int xm = 0;
		string m = argv[arg+2];
		for (auto r : m) {
			if (!isdigit(r))
				xm = -1;
		}
		if (xm != (-1))
			xm = stoi(m,nullptr,10);
		else {
			cout << "Argument to -x must be integer";
			exit(0);
		}
		x.findWallet(xm);
		cout << '\n' << x.curr_wallet << "\n";
	}
	else if (argc >= 2 && strcmp(argv[arg+1],bl) == 0) {
		cout << "Backup List:\n";
		int i = 0;
		fstream fin (x.backups.c_str(), ios::in);
		while (!fin.eof() && fin.peek() != EOF) {
			string nra = "";
			while (fin.peek() != ';' && fin.peek() != EOF && !fin.eof())
				nra.append((size_t)1,fin.get());
			fin.ignore();
			unsigned long tyvm = stoul(nra,nullptr,10);
			printf("\t%u", tyvm);
			if (i%7 == 6)
				cout << "\n\n";
			i++;
		}
		cout << "\n\n\tTotal Backup Files: " << i;
		fin.close();
		exit(0);
	}
	else {
		printf("Syntax Error:\nusage:\t%10s -c input data_id\n", argv[0]);
		printf("\t%10s -n (returns wallet count)\n", argv[0]);
		printf("\t%10s -x integer (retrieves wallet name by data id)\n", argv[0]);
		printf("\t%10s --backup-transactions YEARMMDD (8 digit string required as shown)\n", argv[0]);
		printf("\t%10s --restore-mode (starts restore shell)\n", argv[0]);
		printf("\t%10s --restore-backups (starts restore without shell from %s)\n", argv[0], x.backups.c_str());
		printf("\t%10s --move-backup YEARMMDD (moves CURRFILE contents to YEARMMDD (8 digit string))\n", argv[0]);
		printf("\t%10s --create-wallets N (creates N additional wallets)\n", argv[0]);
		printf("\t%10s --disperse (issues transactions to wallets)\n", argv[0]);
		printf("\t%10s --list-backups (lists backup files in %s)\n", argv[0], x.backups.c_str());
		printf("\t%10s --quiet mode is always available. Add to line as first or last argument.", argv[0]);
	}
	return 1;
}
*/