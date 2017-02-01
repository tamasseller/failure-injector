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

#ifndef BACKTRACE_H_
#define BACKTRACE_H_

#include <execinfo.h>

#include "failure-injector/BacktraceFactory.h"
#include "CppUTest/TestHarness.h"

class Addr2line {
	int child, mosi, miso;
public:
	void start();
	const char* convert(void*);
	void close();
};

class Addr2lineBacktrace: public Backtrace {
	int addrlen;
	void* addrlist[64];
public:
	inline Addr2lineBacktrace () {
		addrlen = backtrace(addrlist, 64);
	}

	virtual void print(TestResult* result = 0);
};

class Addr2lineBacktraceFactory: public BacktraceFactory {
	virtual Backtrace* build();
};

#endif /* BACKTRACE_H_ */
