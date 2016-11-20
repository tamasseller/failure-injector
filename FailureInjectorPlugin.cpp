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

#include "FailureInjectorPlugin.h"
#include "FailureInjectorShell.h"
#include "SourceRecord.h"

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestMemoryAllocator.h"

#undef new

FailureInjectorPlugin::FailureInjectorPlugin():
		TestPlugin("FailureInjector"),
		leaveAloneCurrent(false),
		letNextAssertPass(false),
		activeShell(0),
		savedFailureCount(0),
		traceValid(false),
		countingEnabled(true),
		sharedMode(false) {}

void FailureInjectorPlugin::setBacktraceFactory(BacktraceFactory* factory) {
	instance()->traceFactory = factory;
}

FailureInjectorPlugin* FailureInjectorPlugin::instance()
{
	static FailureInjectorPlugin _instance;
	return &_instance;
}

void FailureInjectorPlugin::preTestAction(UtestShell& test, TestResult& result)
{
	letNextAssertPass = false;
	savedFailureCount = result.getFailureCount();
	FailureSourceManager::reset();
}

void FailureInjectorPlugin::postTestAction(UtestShell& test, TestResult& result)
{
	if(leaveAloneCurrent) {
		leaveAloneCurrent = false;
	} else if(savedFailureCount == result.getFailureCount()) {
		if(!test.getName().contains(FailureInjectorShell::prefix)) {
			int nIter = 1, nSources = 0;

			for(auto it = FailureSourceManager::nonZeroMaxIterator(); it.current(); it.step()) {
				if(accessInhibited(it.current()) == false) {
					nIter *= accessCounter(it.current()) + 1;
					nSources++;
				}
			}

			nIter = nIter - 1;

			if(nSources) {
				static char temp[256];

				const int effIter = sharedMode ? accessSharedCounter() : nIter;
				const long eta = effIter * result.getCurrentTestTotalExecutionTime();
				if(eta > 1000) {
					static char temp[128];
					sprintf(temp, "\nWARNING: Generating many (%d) synthetic tests for %s in group %s. \n\n\tETA: %dms.\n\n", effIter, test.getName().asCharString(), test.getGroup().asCharString(), eta);
					result.print(temp);
				}

				const unsigned int recSize = sizeof(SourceRecord);
				void *records = defaultNewAllocator()->alloc_memory(recSize * nSources, __FILE__, __LINE__);
				SourceRecord* sources = new(records) SourceRecord[nSources];

				unsigned int idx = 0;
				for(auto it = FailureSourceManager::nonZeroMaxIterator(); it.current(); it.step()) {
					if(accessInhibited(it.current()) == false) {
						unsigned int maxCount = sharedMode ? accessSharedCounter() : accessCounter(it.current());
						sources[idx++] = SourceRecord(it.current(), maxCount);
					}
				}

				const unsigned int objSize = sizeof(FailureInjectorShell);
				void *obj = defaultNewAllocator()->alloc_memory(objSize, __FILE__, __LINE__);
				FailureInjectorShell* syntheticCase = new (obj) FailureInjectorShell(test, nSources, sources, sharedMode);
			}

			sharedMode = false;
		}
	} else {
		printLastTrace();
	}
	traceValid = false;
}

void FailureInjectorPlugin::setActiveShell(FailureInjectorShell* newShell) {
	activeShell = newShell;
}

FailureInjectorShell* FailureInjectorPlugin::getActiveShell() {
	return activeShell;
}

void FailureInjectorPlugin::saveTrace() {
	if(traceFactory) {
		traceValid = true;
		savedTrace = traceFactory->build();
	}
}

void FailureInjectorPlugin::printLastTrace() {
	if(savedTrace && traceValid) {
		activeShell->result->print("Last failure injected at:\n\n");
		savedTrace->print(activeShell->result);
	}
}

void FailureInjectorPlugin::inhibit() {
	leaveAloneCurrent = true;
}

void FailureInjectorPlugin::armAssert() {
	letNextAssertPass = true;
}

void FailureInjectorPlugin::disableCounting() {
	countingEnabled = false;
}

void FailureInjectorPlugin::enableCounting() {
	countingEnabled = true;
}

void FailureInjectorPlugin::setShared(bool shared) {
	sharedMode = shared;
}
