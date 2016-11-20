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

#ifndef SOURCERECORD_H_
#define SOURCERECORD_H_

struct SourceRecord {
	FailureSourceConnector* source;
	unsigned int max;

	inline SourceRecord(): source(0), max(0) {}
	inline SourceRecord(FailureSourceConnector* source, unsigned int max): source(source), max(max) {}
};

#endif /* SOURCERECORD_H_ */
