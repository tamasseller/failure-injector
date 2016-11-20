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

#include "FailureInjectorShell.h"
#include "SimpleStringHelper.h"
#include "SourceRecord.h"

#include "CppUTest/TestResult.h"

const char* FailureInjectorShell::prefix = "*** SYNTHETIC *** ";

FailureInjectorShell::FailureInjectorShell(UtestShell &testShell, unsigned int nRecords, SourceRecord* records, bool sharedMode):
		nRecords(nRecords),
		records(records),
		sharedMode(sharedMode),
		newName(0)
{
	setFileName(SimpleStringHelper::copyString(testShell.getFile()));
	setLineNumber(testShell.getLineNumber());
	setGroupName(SimpleStringHelper::copyString(testShell.getGroup()));
	setTestName(baseName = SimpleStringHelper::copyString(SimpleString(prefix)+testShell.getName()));

	originalShell = &testShell;
	savedNext = testShell.getNext();
	testShell.addTest(this);
	addTest(this);
}

Utest* FailureInjectorShell::createTest() {
	return originalShell->createTest();
}

void FailureInjectorShell::runOneTest(TestPlugin* plugin, TestResult& result)
{
	int savedFailureCount = result.getFailureCount();
	FailureInjectorPlugin::instance()->setActiveShell(this);

	this->result = &result;

	for(int i = 0; i < nRecords; i++) {
		unsigned int &errorAt = accessErrorAt(records[i].source);
		errorAt++;
		if(!sharedMode) {
			if(errorAt > records[i].max) {
				errorAt = 0;
			} else {
				break;
			}
		}
	}

	SimpleString fancyName = SimpleString(baseName) + "(";

	if(!sharedMode) {
		bool first = true;
		for(int i = 0; i < nRecords; i++) {
			unsigned int &errorAt = accessErrorAt(records[i].source);
			if(errorAt) {
				if(!first) {
					fancyName += ", ";
				}else
					first = false;

				const char* sourceName = records[i].source->getFailureSourceName();
				if(!sourceName)
					sourceName = "<NONAME>";

				fancyName += SimpleString(sourceName) + "@" + StringFrom((int)errorAt);
			}
		}
	}else
		fancyName += SimpleString("<SHARED>@") + StringFrom((int)accessErrorAt(records[0].source));

	if(newName)
		SimpleString::deallocStringBuffer((char*)newName, __FILE__, __LINE__);

	newName = SimpleStringHelper::copyString(fancyName + ")");
	originalShell->setTestName(newName);

	UtestShell::runOneTest(plugin, result);

	bool isLast = true;
	if(savedFailureCount == result.getFailureCount()) {
		for(int i = 0; i < nRecords; i++) {
			if(accessErrorAt(records[i].source) != records[i].max) {
				isLast = false;
				break;
			}
		}
	}

	if(isLast) {
		for(int i = 0; i < nRecords; i++)
			accessErrorAt(records[i].source) = 0;

		addTest(savedNext);
	}

	FailureInjectorPlugin::instance()->setActiveShell(0);
}

void FailureInjectorShell::assertTrue(bool condition, const char *checkString, const char *conditionString, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertTrue(condition, checkString, conditionString, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertCstrEqual(const char *expected, const char *actual, const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertCstrEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertCstrNoCaseEqual(const char *expected, const char *actual, const char *text, const char *fileName, int lineNumber) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertCstrNoCaseEqual(expected, actual, text, fileName, lineNumber);
	}
}

void FailureInjectorShell::assertCstrContains(const char *expected, const char *actual, const char *text, const char *fileName, int lineNumber) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertCstrContains(expected, actual, text, fileName, lineNumber);
	}
}

void FailureInjectorShell::assertCstrNoCaseContains(const char *expected, const char *actual, const char *text, const char *fileName, int lineNumber) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertCstrNoCaseContains(expected, actual, text, fileName, lineNumber);
	}
}

void FailureInjectorShell::assertLongsEqual(long  expected, long  actual, const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertLongsEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertPointersEqual(const void *expected, const void *actual, const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertPointersEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertUnsignedLongsEqual(unsigned long expected, unsigned long actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertUnsignedLongsEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertLongLongsEqual(cpputest_longlong expected, cpputest_longlong actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertLongLongsEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertUnsignedLongLongsEqual(cpputest_ulonglong expected, cpputest_ulonglong actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertUnsignedLongLongsEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertSignedBytesEqual(signed char expected, signed char actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertSignedBytesEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertFunctionPointersEqual(void (*expected)(), void (*actual)(), const char* text, const char* fileName, int lineNumber, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertFunctionPointersEqual(expected, actual, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::assertEquals(bool failed, const char* expected, const char* actual, const char* text, const char* file, int line, const TestTerminator& testTerminator){
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertEquals(failed, expected, actual, text, file, line, testTerminator);
	}
}

void FailureInjectorShell::assertDoublesEqual(double expected, double actual, double threshold, const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->assertDoublesEqual(expected, actual, threshold, text, fileName, lineNumber, testTerminator);
	}
}

void FailureInjectorShell::fail(const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator) {
	if(FailureInjectorPlugin::instance()->letNextAssertPass) {
		FailureInjectorPlugin::instance()->letNextAssertPass = false;
		originalShell->fail(text, fileName, lineNumber, testTerminator);
	}
}



