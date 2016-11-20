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

#ifndef FAILURESOURCE_H_
#define FAILURESOURCE_H_

#include "Registry.h"

inline bool isFailureCountingEnabled();

class FailureSourceConnector {
	friend class FailureSourceIntrumentationHelper;
	unsigned int counter;
	unsigned int errorAt;
	unsigned int static sharedCounter;
protected:
	bool inhibited;
	inline FailureSourceConnector(): counter(0), errorAt(0), inhibited(false) {}

	struct Inhibitor {
		static inline void doInhibit(FailureSourceConnector& source) {
			source.inhibited = true;
		}
	};

	void saveTrace();
public:
	inline bool shouldSimulateError() {
		if(isFailureCountingEnabled()) {
			counter++;
			sharedCounter++;
			if(counter == errorAt || sharedCounter == errorAt) {
				saveTrace();
				return true;
			}
		}

		return false;
	}

	virtual const char* getFailureSourceName() {
		return 0;
	}
};

struct FailureSourceIntrumentationHelper {
	static unsigned int &accessCounter(FailureSourceConnector *fsc) {return fsc->counter;}
	static unsigned int &accessErrorAt(FailureSourceConnector *fsc) {return fsc->errorAt;}
	static unsigned int &accessSharedCounter() {return FailureSourceConnector::sharedCounter;}
	static bool &accessInhibited(FailureSourceConnector *fsc) {return fsc->inhibited;}
};

template <class Child>
class StaticFailureSource: public Registry<FailureSourceConnector>::StaticElement<Child>, private FailureSourceConnector::Inhibitor {
public:
	static inline void inhibit() {
		doInhibit(Registry<FailureSourceConnector>::StaticElement<Child>::instance);
	}
};

struct FailureSourceManager: public Registry<FailureSourceConnector>, FailureSourceIntrumentationHelper {
	static inline void reset() {
		for(auto it = FailureSourceManager::iterator(); it.current(); it.step()) {
			accessSharedCounter() = 0;
			accessCounter(it.current()) = 0;
			accessInhibited(it.current()) = false;
		}
	}

	class NonZeroMaxIterator: Iterator {
			friend FailureSourceManager;
			inline NonZeroMaxIterator(const Iterator& other): Iterator(other) {}

		public:
			FailureSourceConnector* current() {
				if(!Iterator::current())
					return 0;

				if(!accessCounter(Iterator::current()))
					step();

				return Iterator::current();
			}

			void step() {
				if(Iterator::current()) {
					do {
						Iterator::step();
					} while(Iterator::current() && accessCounter(Iterator::current()) == 0);
				}
			}
	};

	static NonZeroMaxIterator nonZeroMaxIterator() {
		return iterator();
	}
};

#include "FailureInjectorPlugin.h"

#endif /* FAILURESOURCE_H_ */
