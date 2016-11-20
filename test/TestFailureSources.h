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

#ifndef TESTFAILURESOURCES_H_
#define TESTFAILURESOURCES_H_

#include "FailureSource.h"

class TestFailureSource1: public StaticFailureSource<TestFailureSource1> {
public:
	virtual const char* getFailureSourceName() {
		return "Source1";
	}

	static bool operation() {
		if(instance.shouldSimulateError())
			return false;
		return true;
	}
};

class TestFailureSource2: public StaticFailureSource<TestFailureSource2> {
public:
	virtual const char* getFailureSourceName() {
		return "Source2";
	}

	static bool operation() {
		if(instance.shouldSimulateError())
			return false;
		return true;
	}
};

#endif /* TESTFAILURESOURCES_H_ */
