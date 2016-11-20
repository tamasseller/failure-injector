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

#ifndef SIMPLESTRINGHELPER_H_
#define SIMPLESTRINGHELPER_H_

#include "CppUTest/SimpleString.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/Utest.h"

class SimpleStringHelper {
public:
    static const char *copyString(const SimpleString& sstr);
};

#endif /* SIMPLESTRINGHELPER_H_ */
