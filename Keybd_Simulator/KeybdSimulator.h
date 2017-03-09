#ifndef __KEYBOARD_SIMULATOR_H__
#define __KEYBOARD_SIMULATOR_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include "Driver.h"

#define N_CMDS		7

//struct Parameter {
//	const char *fpath;
//	KeybdSimulator * const thisKeybdSimulator;
//};

class KeybdSimulator {
public:
	KeybdSimulator(std::istream &is = std::cin, std::ostream &os = std::cout);
	~KeybdSimulator();
	void exec();
private:
	Driver *driver;
	unsigned pagesize; // discard
	bool quit_status;
	bool is_busy;
	std::istream &is;
	std::ostream &os;
	std::string htf_file_path;
	std::string bin_file_path;
	bool is_htf_file; // discard
	HANDLE child_thread;
	FILE *tmp_file;
	const char* allcmds[N_CMDS] = {
		"help",
		"setoption",
		"showoption",
		"go",
		"encode",
		"decode",
		"quit"
	};
	void (KeybdSimulator::*execfunc[N_CMDS])(const char *) = {
		&KeybdSimulator::help_func,
		&KeybdSimulator::setoption_func,
		&KeybdSimulator::showoption_func,
		&KeybdSimulator::go_func,
		&KeybdSimulator::encode_func,
		&KeybdSimulator::decode_func,
		&KeybdSimulator::quit_func
	};
	void welcome();
	int parse_cmd(const char *cmd, const char* target_cmd[], unsigned n);
	void help_func(const char *args);
	void setoption_func(const char *args);
	void showoption_func(const char *args);
	void go_func(const char *args);
	void encode_func(const char *args);
	void decode_func(const char *args);
	void quit_func(const char *args);
	static DWORD WINAPI typing(LPVOID lpParameter);
};

#endif
