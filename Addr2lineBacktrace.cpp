/*******************************************************************************
 *
 * Copyright (c) 2016 Seller Tamás. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************/

#include "Addr2lineBacktrace.h"

extern const char *__progname;

#include <cstring>
#include <iostream>

#include <sys/wait.h>
#include <cxxabi.h>
#include <unistd.h>

void Addr2line::start() {
	int addrpipefd[2], linepipefd[2];
	if((pipe(addrpipefd) >= 0) && (pipe(linepipefd) >= 0)){
		if((child = fork()) == 0){
			::close(linepipefd[0]);
			::close(addrpipefd[1]);

			if(dup2(addrpipefd[0], 0) != -1){
				::close(addrpipefd[0]);
				if(dup2(linepipefd[1], 1) != -1){
					::close(linepipefd[1]);
					execlp("addr2line", "-s", "-e", __progname, (char *)0);
				}
			}
		} else {
			::close(addrpipefd[0]);
			mosi = addrpipefd[1];
			miso = linepipefd[0];
			::close(linepipefd[1]);
		}
	}
}

static char addr2LineBuffer[1024];
const char* Addr2line::convert(void* arg) {
	sprintf(addr2LineBuffer, "%X\n", (int)arg);
	write(mosi, addr2LineBuffer, strlen(addr2LineBuffer));
	addr2LineBuffer[read(miso, addr2LineBuffer, sizeof(addr2LineBuffer))-1] = '\0';
	return basename(addr2LineBuffer);
}

void Addr2line::close() {
	::close(mosi);
	::close(miso);

	int returnStatus;
	waitpid(child, &returnStatus, 0);
}

class Printer {
	TestResult* result;
public:
	Printer (TestResult* result): result(result){}

	void print(const char *c) {
		if(result)
			result->print(c);
		else
			std::cout << c;
	}
};


void Addr2lineBacktrace::print(TestResult* result)
{
	Printer printer(result);

	if (addrlen == 0) {
		printer.print("<empty, probably corrupt trace>\n");
		return;
	}

	char** symbollist = backtrace_symbols(addrlist, addrlen);
	static char funcname[1024];

	Addr2line addr2Line;
	addr2Line.start();

	for (int i = 1; i < addrlen; i++) {
		printer.print("\033[1;31m");
		printer.print(addr2Line.convert(addrlist[i]));
		printer.print("\033[0m: ");

		char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
		for (char *p = symbollist[i]; *p; ++p) {
			if (*p == '(')
				begin_name = p;
			else if (*p == '+')
				begin_offset = p;
			else if (*p == ')' && begin_offset) {
				end_offset = p;
				break;
			}
		}

		if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
			*begin_name++ = '\0';
			*begin_offset++ = '\0';
			*end_offset = '\0';

			int status;
			size_t funcnamesize = sizeof(funcname);
			char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
			printer.print("\033[1m");
			printer.print((status == 0) ? ret : begin_name);
			printer.print("\033[0m + ");
			printer.print(begin_offset);
			printer.print("\n");
		} else {
			printer.print(symbollist[i]);
			printer.print("\n");
		}
	}

	addr2Line.close();
	printer.print("\n");

	free(symbollist);
}

Backtrace* Addr2lineBacktraceFactory::build() {
	return new Addr2lineBacktrace();
}
