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

#ifndef MACROS_H_
#define MACROS_H_

#define INHIBIT_FAILURE_INJECTION_FOR_TESTCASE() FailureInjectorPlugin::instance()->inhibit()
#define INHIBIT_FAILURE_INJECTION_FOR_STATIC_SOURCE(x) StaticFailureSource<x>::inhibit();
#define DISABLE_FAILURE_INJECTION_TEMPORARILY() FailureInjectorPlugin::instance()->disableCounting();
#define ENABLE_FAILURE_INJECTION_TEMPORARILY()  FailureInjectorPlugin::instance()->enableCounting();
#define SET_FAILURE_INJECTION_MODE_INDEPENDENT()  FailureInjectorPlugin::instance()->setShared(false);
#define SET_FAILURE_INJECTION_MODE_SHARED()  FailureInjectorPlugin::instance()->setShared(true);

#define FI_ARM		FailureInjectorPlugin::instance()->armAssert()
#define FI_ARM_COND(x) if(FailureInjectorPlugin::instance()->getActiveShell()){FailureInjectorPlugin::instance()->armAssert(); x;}

#define CHECK_ALWAYS(condition) \
	{FI_ARM; CHECK(condition);}

#define CHECK_TEXT_ALWAYS(condition, text) \
	{FI_ARM; CHECK_TEXT(condition, #condition, text);}

#define CHECK_TRUE_ALWAYS(condition) \
	{FI_ARM; CHECK_TRUE(condition, #condition);}

#define CHECK_FALSE_ALWAYS(condition) \
	{FI_ARM; CHECK_FALSE(condition, #condition);}

#define CHECK_EQUAL_ALWAYS(expected,actual) \
	{FI_ARM; CHECK_EQUAL(expected,actual);}

#define STRCMP_EQUAL_ALWAYS(expected,actual) \
	{FI_ARM; STRCMP_EQUAL(expected,actual);}

#define STRCMP_NOCASE_EQUAL_ALWAYS(expected,actual) \
	{FI_ARM; STRCMP_NOCASE_EQUAL(expected,actual);}

#define STRCMP_NOCASE_EQUAL_LOCATION_ALWAYS(expected,actual, file, line) \
	{FI_ARM; STRCMP_NOCASE_EQUAL_LOCATION(expected,actual, file, line);}

#define STRCMP_CONTAINS_ALWAYS(expected,actual) \
	{FI_ARM; STRCMP_CONTAINS(expected,actual);}

#define STRCMP_NOCASE_CONTAINS_ALWAYS(expected,actual) \
	{FI_ARM; STRCMP_NOCASE_CONTAINS(expected,actual);}

#define LONGS_EQUAL_ALWAYS(expected,actual) \
	{FI_ARM; LONGS_EQUAL(expected,actual);}

#define LONGS_EQUAL_LOCATION_ALWAYS(expected,actual,file,line) \
	{FI_ARM; LONGS_EQUAL_LOCATION(expected,actual,file,line);}

#define BYTES_EQUAL_ALWAYS(expected, actual) \
	{FI_ARM; BYTES_EQUAL(expected, actual);}

#define POINTERS_EQUAL_ALWAYS(expected, actual) \
	{FI_ARM; POINTERS_EQUAL(expected, actual);}

#define DOUBLES_EQUAL_ALWAYS(expected,actual,threshold) \
	{FI_ARM; DOUBLES_EQUAL(expected,actual,threshold);}

#define FAIL_ALWAYS(text) \
	{FI_ARM; FAIL(text);}

#define FAIL_TEST_ALWAYS(text) \
	{FI_ARM; FAIL_TEST(text);}

#define CHECK_THROWS_ALWAYS(expected, expression) \
	{FI_ARM; CHECK_THROWS(expected, expression);}

#define CHECK_ON_FAILURE(condition) \
	{FI_ARM_COND( CHECK(condition));}

#define CHECK_TEXT_ON_FAILURE(condition, text) \
	{FI_ARM_COND( CHECK_TEXT(condition, text));}

#define CHECK_TRUE_ON_FAILURE(condition) \
	{FI_ARM_COND( CHECK_TRUE(condition));}

#define CHECK_FALSE_ON_FAILURE(condition) \
	{FI_ARM_COND( CHECK_FALSE(condition));}

#define CHECK_EQUAL_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( CHECK_EQUAL(expected,actual));}

#define STRCMP_EQUAL_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( STRCMP_EQUAL(expected,actual));}

#define STRCMP_NOCASE_EQUAL_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( STRCMP_NOCASE_EQUAL(expected,actual));}

#define STRCMP_NOCASE_EQUAL_LOCATION_ON_FAILURE(expected,actual, file, line) \
	{FI_ARM_COND( STRCMP_NOCASE_EQUAL_LOCATION(expected,actual, file, line));}

#define STRCMP_CONTAINS_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( STRCMP_CONTAINS(expected,actual));}

#define STRCMP_NOCASE_CONTAINS_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( STRCMP_NOCASE_CONTAINS(expected,actual));}

#define LONGS_EQUAL_ON_FAILURE(expected,actual) \
	{FI_ARM_COND( LONGS_EQUAL(expected,actual));}

#define LONGS_EQUAL_LOCATION_ON_FAILURE(expected,actual,file,line) \
	{FI_ARM_COND( LONGS_EQUAL_LOCATION(expected,actual,file,line));}

#define BYTES_EQUAL_ON_FAILURE(expected, actual) \
	{FI_ARM_COND( BYTES_EQUAL(expected, actual));}

#define POINTERS_EQUAL_ON_FAILURE(expected, actual) \
	{FI_ARM_COND( POINTERS_EQUAL(expected, actual));}

#define DOUBLES_EQUAL_ON_FAILURE(expected,actual,threshold) \
	{FI_ARM_COND( DOUBLES_EQUAL(expected,actual,threshold));}

#define FAIL_ON_FAILURE(text) \
	{FI_ARM_COND( FAIL(text));}

#define FAIL_TEST_ON_FAILURE(text) \
	{FI_ARM_COND( FAIL_TEST(text));}

#define CHECK_THROWS_ON_FAILURE(expected, expression) \
	{FI_ARM_COND( CHECK_THROWS(expected, expression));}

#endif /* MACROS_H_ */
