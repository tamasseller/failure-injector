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

#ifndef FAILUREINJECTOR_H_
#define FAILUREINJECTOR_H_

#include "CppUTest/SimpleString.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/Utest.h"

#include "FailureSource.h"
#include "BacktraceFactory.h"

#include "Macros.h"

class FailureInjectorShell;

class FailureInjectorPlugin: public TestPlugin, private FailureSourceIntrumentationHelper
{
		friend FailureInjectorShell;

		bool leaveAloneCurrent;
		bool letNextAssertPass;

		friend bool isFailureCountingEnabled();
		bool countingEnabled;

        FailureInjectorShell* activeShell;
		int savedFailureCount;

        FailureInjectorPlugin();

        void setActiveShell(FailureInjectorShell*);

        bool traceValid;
        BacktraceFactory *traceFactory;
        Backtrace *savedTrace = 0;
        bool sharedMode;
    public:
        FailureInjectorShell* getActiveShell();

        virtual void preTestAction(UtestShell&, TestResult&);
        virtual void postTestAction(UtestShell& test, TestResult& result);

        void saveTrace();
        void printLastTrace();

        static FailureInjectorPlugin* instance();

        static void setBacktraceFactory(BacktraceFactory* factory);

        template<class T>
        void inhibitSource();

        void inhibit();
        void armAssert();
        void disableCounting();
        void enableCounting();
        void setShared(bool shared);
};

template<class T>
void FailureInjectorPlugin::inhibitSource() {
	accessInhibit(StaticFailureSource<T>::instance) = true;
}

bool isFailureCountingEnabled() {
	return FailureInjectorPlugin::instance()->countingEnabled;
}

#endif /* FAILUREINJECTOR_H_ */
