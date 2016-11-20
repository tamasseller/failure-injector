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

TEST_GROUP(FailureInjector) {
};

TEST(FailureInjector, Single1) {
	CHECK(TestFailureSource1::operation());
};

TEST(FailureInjector, Single2) {
	CHECK(TestFailureSource2::operation());
	CHECK_ALWAYS(true);
};

TEST(FailureInjector, Double1) {
	bool ret1 = TestFailureSource1::operation();
	bool ret2 = TestFailureSource2::operation();
	CHECK(ret1 && ret2);
	CHECK_ON_FAILURE(!ret1 || !ret2);
};

TEST(FailureInjector, Double2) {
	bool ret1 = TestFailureSource2::operation();
	bool ret2 = TestFailureSource1::operation();
	CHECK(ret1 && ret2);
	CHECK_ON_FAILURE(!ret1 || !ret2);
};

TEST_GROUP(InhibitFailureInjection) {
	TEST_SETUP() {
		CHECK_ON_FAILURE(false);
	}
	TEST_TEARDOWN() {
		CHECK_ON_FAILURE(false);
	}
};

TEST(InhibitFailureInjection, InhibitPlanted) {
	INHIBIT_FAILURE_INJECTION_FOR_TESTCASE();
	CHECK_ON_FAILURE(false);
};

TEST_GROUP(DisableFailureInjectionTemporarily) {
	TEST_SETUP() {
		DISABLE_FAILURE_INJECTION_TEMPORARILY();
		CHECK_ALWAYS(TestFailureSource1::operation());
		ENABLE_FAILURE_INJECTION_TEMPORARILY();

		CHECK(TestFailureSource1::operation());
	}
	TEST_TEARDOWN() {
		DISABLE_FAILURE_INJECTION_TEMPORARILY();
		CHECK_ALWAYS(TestFailureSource1::operation());
		ENABLE_FAILURE_INJECTION_TEMPORARILY();

		CHECK(TestFailureSource1::operation());
	}
};

TEST(DisableFailureInjectionTemporarily, DisabledFailureInjectionTest) {
	DISABLE_FAILURE_INJECTION_TEMPORARILY();
	CHECK_ALWAYS(TestFailureSource1::operation());
	ENABLE_FAILURE_INJECTION_TEMPORARILY();

	CHECK(TestFailureSource1::operation());
};

TEST(DisableFailureInjectionTemporarily, DisabledFailureInjectionSource) {
	INHIBIT_FAILURE_INJECTION_FOR_STATIC_SOURCE(TestFailureSource2);

	CHECK_ALWAYS(TestFailureSource2::operation());
	CHECK(TestFailureSource1::operation());
};

TEST_GROUP(FailureInjectionMode) {
};


TEST(FailureInjectionMode, Shared) {
	SET_FAILURE_INJECTION_MODE_SHARED();
	CHECK(TestFailureSource1::operation());
	CHECK(TestFailureSource1::operation());
	CHECK(TestFailureSource2::operation());
};

TEST(FailureInjectionMode, Independent) {
	SET_FAILURE_INJECTION_MODE_INDEPENDENT();
	CHECK(TestFailureSource1::operation());
	CHECK(TestFailureSource1::operation());
	CHECK(TestFailureSource2::operation());
};

