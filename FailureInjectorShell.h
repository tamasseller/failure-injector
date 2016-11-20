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

#ifndef FAILUREINJECTORSHELL_H_
#define FAILUREINJECTORSHELL_H_

#include "CppUTest/SimpleString.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/Utest.h"

#include "FailureSource.h"
#include "BacktraceFactory.h"

class SourceRecord;
class FailureInjectorPlugin;

class FailureInjectorShell: public UtestShell, private FailureSourceIntrumentationHelper {
private:
		friend FailureInjectorPlugin;
		unsigned int nRecords;
		SourceRecord *records;
		UtestShell *savedNext;
		UtestShell *originalShell;
		const char* newName;
		const char* baseName;
		bool sharedMode;
		TestResult* result = 0;
public:
		static const char* prefix;

		FailureInjectorShell(UtestShell &testShell, unsigned int, SourceRecord*, bool);

		virtual Utest* createTest();
		virtual void runOneTest(TestPlugin* plugin, TestResult& result);

		virtual void assertTrue(bool condition, const char *checkString, const char *conditionString, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertCstrEqual(const char *expected, const char *actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertCstrNoCaseEqual(const char *expected, const char *actual, const char* text, const char *fileName, int lineNumber);
		virtual void assertCstrContains(const char *expected, const char *actual, const char* text, const char *fileName, int lineNumber);
		virtual void assertCstrNoCaseContains(const char *expected, const char *actual, const char* text, const char *fileName, int lineNumber);
		virtual void assertLongsEqual(long expected, long actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertUnsignedLongsEqual(unsigned long expected, unsigned long actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertLongLongsEqual(cpputest_longlong expected, cpputest_longlong actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertUnsignedLongLongsEqual(cpputest_ulonglong expected, cpputest_ulonglong actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertSignedBytesEqual(signed char expected, signed char actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertPointersEqual(const void *expected, const void *actual, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertFunctionPointersEqual(void (*expected)(), void (*actual)(), const char* text, const char* fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertDoublesEqual(double expected, double actual, double threshold, const char* text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void assertEquals(bool failed, const char* expected, const char* actual, const char* text, const char* file, int line, const TestTerminator& testTerminator = NormalTestTerminator());
		virtual void fail(const char *text, const char *fileName, int lineNumber, const TestTerminator& testTerminator = NormalTestTerminator());
};

#endif /* FAILUREINJECTORSHELL_H_ */
