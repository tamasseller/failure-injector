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

#ifndef OOPS_H_
#define OOPS_H_

#include <signal.h>
#include <iostream>

template<void (*oops)(), void (*always)()>
class OopsUtils {
	static bool done;
	static int lastSignal;
	static const char* signalNames[];

	static void bye() {
		if(!done)
			oops();

		always();
	}

	static void sign(int x) {
		if(x == lastSignal) {
			std::cout << "Double fault !!!" << std::endl;
			exit(-1);
		} else {
			lastSignal = x;
			std::cout << "Caught signal " << x;
			if((unsigned int)x < sizeof(signalNames)/sizeof(signalNames[0])+1)
				std::cout << " ("<< signalNames[x-1] << ")";

			std::cout<< std::endl;
			bye();
			done = true;
		}
	}

public:
	static void registerOopsHandlers() {
	    atexit(bye);
	    signal(SIGABRT, sign);
	    signal(SIGSEGV, sign);
	}

	static void allWentOk() {
	    done = true;
	}
};

template<void (*oops)(), void (*always)()>
bool OopsUtils<oops, always>::done = false;

template<void (*oops)(), void (*always)()>
int OopsUtils<oops, always>::lastSignal = -1;

template<void (*oops)(), void (*always)()>
const char* OopsUtils<oops, always>::signalNames[] = {
		"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL",
		"SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE",
		"SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT",
		"SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
		"SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
		"SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
		"SIGIO", "SIGPWR", "SIGSYS"
};

#endif /* OOPS_H_ */
