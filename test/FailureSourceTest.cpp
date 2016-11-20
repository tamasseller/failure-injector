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

#include "CppUTest/TestHarness.h"

#include "FailureInjectorPlugin.h"
#include "TestFailureSources.h"

TEST_GROUP(FailureSource) {
};

TEST(FailureSource, Discovery) {
	auto it = FailureSourceManager::iterator();
	CHECK(it.current() != NULL);
	it.step();
	CHECK(it.current() != NULL);
	it.step();
	CHECK(it.current() == NULL);
}

TEST(FailureSource, Counting) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	struct AccessWrapper: public FailureSourceIntrumentationHelper {
		void run() {
			FailureSourceManager::reset();

			volatile bool v1 = TestFailureSource1::operation();
			volatile bool v2 = TestFailureSource1::operation();
			volatile bool v3 = TestFailureSource2::operation();

			int firstCount, secondCount;
			auto it = FailureSourceManager::iterator();
			CHECK(it.current());
			firstCount = accessCounter(it.current());
			it.step();
			CHECK(it.current());
			secondCount = accessCounter(it.current());
			it.step();
			CHECK(!it.current());

			CHECK((firstCount == 1 && secondCount == 2) || (firstCount == 2 && secondCount == 1));
		}
	};

	AccessWrapper().run();
}

TEST_GROUP(FailureInjection) {
	FailureSourceConnector *src1, *src2;
	TEST_SETUP() {
		struct AccessWrapper: public FailureSourceIntrumentationHelper {
			void run(FailureSourceConnector *&src1, FailureSourceConnector *&src2) {
				FailureSourceManager::reset();
				volatile bool v1 = TestFailureSource1::operation();
				volatile bool v2 = TestFailureSource1::operation();
				volatile bool v3 = TestFailureSource2::operation();

				auto it = FailureSourceManager::iterator();
				if(accessCounter(it.current()) == 2) {
					src1 = it.current();
					it.step();
					src2 = it.current();
				} else {
					src2 = it.current();
					it.step();
					src1 = it.current();
				}

				CHECK(src1 != NULL);
				CHECK(src2 != NULL);
			}
		};

		AccessWrapper().run(src1, src2);
	}
};

TEST(FailureInjection, SingleFault1) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	struct AccessWrapper: public FailureSourceIntrumentationHelper {
		void run(FailureSourceConnector *&src1, FailureSourceConnector *&src2) {
			accessCounter(src1) = 0;
			accessCounter(src2) = 0;

			accessErrorAt(src1) = 1;
			accessErrorAt(src2) = 0;

			CHECK(!TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
		}
	};

	AccessWrapper().run(src1, src2);
}

TEST(FailureInjection, SingleFault2) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	struct AccessWrapper: public FailureSourceIntrumentationHelper {
		void run(FailureSourceConnector *&src1, FailureSourceConnector *&src2) {
			accessCounter(src1) = 0;
			accessCounter(src2) = 0;

			accessErrorAt(src1) = 0;
			accessErrorAt(src2) = 1;

			CHECK(TestFailureSource1::operation());
			CHECK(!TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
		}
	};

	AccessWrapper().run(src1, src2);
}

TEST(FailureInjection, DoubleFault1) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	struct AccessWrapper: public FailureSourceIntrumentationHelper {
		void run(FailureSourceConnector *&src1, FailureSourceConnector *&src2) {
			accessCounter(src1) = 0;
			accessCounter(src2) = 0;

			accessErrorAt(src1) = 1;
			accessErrorAt(src2) = 2;

			CHECK(!TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(!TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
		}
	};

	AccessWrapper().run(src1, src2);
}

TEST(FailureInjection, DoubleFault2) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	struct AccessWrapper: public FailureSourceIntrumentationHelper {
		void run(FailureSourceConnector *&src1, FailureSourceConnector *&src2) {
			accessCounter(src1) = 0;
			accessCounter(src2) = 0;

			accessErrorAt(src1) = 3;
			accessErrorAt(src2) = 2;

			CHECK(TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(!TestFailureSource2::operation());
			CHECK(!TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
			CHECK(TestFailureSource1::operation());
			CHECK(TestFailureSource2::operation());
		}
	};

	AccessWrapper().run(src1, src2);
}
