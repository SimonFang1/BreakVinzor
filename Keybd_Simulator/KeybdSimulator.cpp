#include "KeybdSimulator.h"
#include <string.h>	//strerror
#include <sstream>
#include <vector>
#include <time.h>


using std::endl;
using std::stringstream;
using std::string;
using std::vector;

KeybdSimulator::KeybdSimulator(std::istream &is, std::ostream &os) :is(is), os(os) {
	driver = nullptr;
	quit_status = false;
	is_htf_file = false;
	is_busy = false;
	pagesize = 1024;
	bin_file_path = "origin";
	htf_file_path = "HexTransitFormat.txt";
	tmp_file = nullptr;
}

KeybdSimulator::~KeybdSimulator() {
	delete driver;
	//os << "bye" << endl;
}

void KeybdSimulator::welcome() {
	os << "id name Keyboard Simulator" << endl
		<< "id version 3.1" << endl
		<< "id copyright 2016-2017 https://github.com/SimonFang1/BreakVinzor" << endl
		<< "id author Simon Fang" << endl
		<< "id user Everyone" << endl
		//<< "option pagesize default 1024" << endl
		//<< "option type var bin var htf default bin" << endl
		<< "option bin_path ./origin" << endl
		<< "option htf_path ./HexTransitFormat.txt" << endl
		<< endl;

	driver = new Driver();
	if (!driver->good()) {
		os << "fatal error: " << strerror(driver->getErrorCode()) << endl << endl;
		quit_status = true;
		return;
	}

	os << "ready" << endl;
	os << "enter \"h[elp]\" to get help information" << endl;

}

void KeybdSimulator::exec() {
	stringstream ss;
	string buffer;
	string cmd;
	string args;
	welcome();
	while (!quit_status) {
		getline(is, buffer);
		ss << buffer;
		ss >> cmd;
		if (ss.peek() == ' ') {
			ss.ignore();
		}
		getline(ss, args);
		int cmd_res = parse_cmd(cmd.c_str(), allcmds, N_CMDS);
		if (cmd_res >= 0) {
			(this->*KeybdSimulator::execfunc[cmd_res])(args.c_str());
		}
		if (is_busy && cmd_res < 0) {
			CloseHandle(child_thread);
			os << "typing task abort" << endl;
			is_busy = false;
			fclose(tmp_file);
			tmp_file = nullptr;
		}
		ss.clear();
		ss.str("");
		cmd.clear();
		args.clear();
	}
}

int KeybdSimulator::parse_cmd(const char *cmd, const char* target_cmd[], unsigned n) {
	vector<unsigned> valid;
	valid.reserve(n);
	for (int j = 0; j < n; j++) {
		valid.push_back(j);
	}
	for (int i = 0; cmd[i] != '\0'; i++) {
		for (vector<unsigned>::iterator it = valid.begin(); it != valid.end(); ) {
			if (cmd[i] != target_cmd[*it][i]) {
				valid.erase(it);
			} else {
				it++;
			}
		}
		if (valid.size() == 0) break;
	}
	if (valid.size() < 1) {
		os << "invalid command: " << cmd << endl;
		return -1;
	}
	if (valid.size() > 1) {
		for (int ii = 0; ii < valid.size(); ii++) {
			if (target_cmd[valid[ii]] == cmd) {
				return valid[ii];
			}
		}
		if (strcmp(cmd, "") == 0) return -2;
		os << "ambiguous command: " << cmd << endl;
		for (int j = 0; j < valid.size(); j++) {
			os << target_cmd[valid[j]] << '\t';
		}
		os << endl;
		return -2;
	}
	return *valid.begin();
}

void KeybdSimulator::help_func(const char *args) {
	char *line = "------------------------------";
	char *dline = "===============================";
	os << endl << dline << endl;
	//os << "help page: " << endl;
	os << "this program has been published on github." << endl;
	os << "https://github.com/SimonFang1/BreakVinzor" << endl;
	os << "total commands;\n";
	for (int i = 0; i < N_CMDS; i++) {
		os << allcmds[i] << '\t';
	}
	os << endl;
	
	os << endl << "encode:" << endl;
	os << line << endl;
	
	os << "this commands needs 0-2 arguments, you are required to enter as following format" << endl;
	os << "\te[ncode] ([source_binary_file] [hex_character_text_file])" << endl;
	os << "if only \"e\" is entered, it is equivalent to \"encode origin HexTransitFormat.txt\"" << endl;
	os << line << endl;

	os << endl << "decode:" << endl;
	os << line << endl;
	os << "this commands needs 0-2 arguments, you are required to enter as following format" << endl;
	os << "\td[ecode] ([source_binary_file] [hex_character_text_file])" << endl;
	os << "if only \"d\" is entered, it is equivalent to \"decode origin HexTransitFormat.txt\"" << endl;

	os << endl << "go" << endl;
	os << line << endl;
	os << "this commands needs none or 1 argument, you are required to enter as following format" << endl;
	os << "\tg[o] ([hex_character_text_file])" << endl;
	os << "if only \"g\" is entered, it is equivalent to \"go HexTransitFormat.txt\"" << endl;
	os << "if any commands or strings are recieved while typing, the typing task will be aborted" << endl;

	os << endl << "setoption" << endl;
	os << line << endl;
	os << "you are required to enter as following format" << endl;
	os << "\tse[toption] <-b bin_path> <-h htf_path>" << endl;

	os << endl << "showoption" << endl;
	os << line << endl;
	os << "this commands needs none arguments, enter to show options" << endl;
	os << "\tsh[owoption]" << endl;


	os << endl << "quit" << endl;
	os << line << endl;
	os << "this commands needs none arguments, enter to quit" << endl;
	os << "\tq[uit]" << endl;

	os << dline << endl << endl;
}

void KeybdSimulator::setoption_func(const char *args) {
	const char* set_cmds[4] = { "-binpath", "-htfpath", "-pagesize", "-mode" };
	stringstream ss;
	string key;
	string val;
	ss << args;
	while (ss >> key) {
		int cmd_res = parse_cmd(key.c_str(), set_cmds, 4);
		if (cmd_res == 0) {
			ss >> bin_file_path;
			os << "set binary filepath to " << bin_file_path << endl;
		} else if (cmd_res == 1) {
			ss >> htf_file_path;
			os << "set hex transformation filepath to " << htf_file_path << endl;
		} else if (cmd_res == 2) {
			ss >> pagesize;
			os << "set pagesize to " << pagesize << endl;
		} else if (cmd_res == 3) { // discard
			ss >> val;
			is_htf_file = (val == "h");
			os << "set mode to " << (is_htf_file ? "htf" : "bin") << endl;
		} else {
			os << "unknown parameter: " << key << endl;
		}
	}
}

void KeybdSimulator::showoption_func(const char * args) {
	os 
		//<< "option pagesize: " << pagesize << endl
		//<< "option type: " << (is_htf_file ? "htf" : "bin") << endl
		<< "option bin_path: " << bin_file_path << endl
		<< "option htf_path: " << htf_file_path << endl;
}

void KeybdSimulator::go_func(const char *args) {
	if (tmp_file) {
		fclose(tmp_file);
		tmp_file = nullptr;
	}
	string last_path = htf_file_path;
	if (args[0])
		htf_file_path = args;
	tmp_file = fopen(htf_file_path.c_str(), "r");
	if (!tmp_file) {
		os << "cannot open the file: " << htf_file_path << endl;
		return;
	}
	os << "typing the file: " << htf_file_path << " in 3 seconds" << endl;
	Sleep(3000);
	os << "\a";
	child_thread = CreateThread(
		nullptr,
		0,
		typing,
		this,
		0,
		nullptr
	);
	htf_file_path = last_path;
}

void KeybdSimulator::encode_func(const char *args) {
	stringstream ss;
	ss << args;
	string arg1, arg2;
	ss >> arg1 >> arg2;

	if (arg1.size() == 0) {
		arg1 = bin_file_path;
	}
	if (arg2.size() == 0) {
		arg2 = htf_file_path;
	}

	int res;
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	char cmdline[256];
	snprintf(cmdline, 256, "encoder %s %s", arg1.c_str(), arg2.c_str());
	res = CreateProcessA(nullptr,
		cmdline,
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&si,
		&pi);
	if (!res) {
		fprintf(stderr, "Create Process Failed\n");
		return;
	}
	int errorcode = WaitForSingleObject(pi.hProcess, INFINITE);
	if (errorcode) {
		os << strerror(errorcode);
	} else {
		os << "encoding complete" << endl;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void KeybdSimulator::decode_func(const char *args) {
	stringstream ss;
	ss << args;
	string arg1, arg2;
	ss >> arg1 >> arg2;

	if (arg1.size() == 0) {
		arg1 = bin_file_path;
	}
	if (arg2.size() == 0) {
		arg2 = htf_file_path;
	}

	int res;
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	char cmdline[256];
	snprintf(cmdline, 256, "decoder %s %s", arg1.c_str(), arg2.c_str());
	res = CreateProcessA(nullptr,
		cmdline,
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&si,
		&pi);
	if (!res) {
		fprintf(stderr, "Create Process Failed\n");
		return;
	}
	int errorcode = WaitForSingleObject(pi.hProcess, INFINITE);
	if (errorcode) {
		os << strerror(errorcode);
	} else {
		os << "decoding complete" << endl;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void KeybdSimulator::quit_func(const char *args) {
	quit_status = true;
	os << "bye" << endl;
}

DWORD WINAPI KeybdSimulator::typing(LPVOID lpParameter) {
	KeybdSimulator *thisKeybdSimulator = (KeybdSimulator *)lpParameter;
	thisKeybdSimulator->is_busy = true;
	char hexch;
	clock_t s_time = clock();
	while ((hexch = fgetc(thisKeybdSimulator->tmp_file)) != EOF/*!feof(thisKeybdSimulator->tmp_file)*/) {
		thisKeybdSimulator->driver->SendKey(hexch);
	}
	clock_t e_time = clock();
	fclose(thisKeybdSimulator->tmp_file);
	thisKeybdSimulator->tmp_file = nullptr;
	thisKeybdSimulator->is_busy = false;
	thisKeybdSimulator->os << "\atyping task completed" << endl
		<< "time cost: " << ((e_time - s_time) * 1.0 / CLOCKS_PER_SEC) << endl;
	return 0;
}
