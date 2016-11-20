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

#include "Registry.h"

struct Required {
	virtual char getId() = 0;
	int logStuff(int x) {
		return x;
	}
};

struct RegistreeA: public Registry<Required>::StaticElement<RegistreeA> {
	virtual char getId() {
		return 'a';
	}

	static int someAstuff(int x) {
		instance.logStuff(3);
		return 3*x;
	}
};

struct RegistreeB: public Registry<Required>::StaticElement<RegistreeB> {
	virtual char getId() {
		return 'b';
	}

	static int someBstuff(int x) {
		instance.logStuff(2);
		return 2*x;
	}
};

TEST_GROUP(Registry) {
	TEST_SETUP() {
	}
};

TEST(Registry, StaticRegistration) {
	auto it = Registry<Required>::iterator();
	CHECK(it.current() != NULL);
	char first = it.current()->getId();
	it.step();
	char second= it.current()->getId();
	it.step();
	CHECK(it.current() == NULL);
	it.step();
	CHECK(it.current() == NULL);

	CHECK(first == 'a' || second == 'a');
	CHECK(first == 'b' || second == 'b');
}
