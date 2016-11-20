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

#ifndef REGISTRY_H_
#define REGISTRY_H_

template<class Interface>
class Registry {
public:
	class ElementBase: public Interface {
		friend Registry;
		ElementBase *next;
	protected:
		ElementBase(): next(0) {}
	};

private:
	static ElementBase* first;
public:
	template <class Child>
	class StaticElement: public ElementBase {
	protected:
		StaticElement() {
			Registry::add(this);
		}

		static Child instance;
	};

	static void add(ElementBase* item) {
		item->next = first;
		first = item;
	}

	class Iterator {
		ElementBase* currItem;

		friend Registry;
	protected:
		inline Iterator(ElementBase* firstItem): currItem(firstItem) {}
	public:
		Interface* current() {
			return currItem;
		}

		void step() {
			if(currItem)
				currItem = currItem->next;
		}
	};

	static Iterator iterator() {
		return Iterator(first);
	}
};

template<class Interface>
typename Registry<Interface>::ElementBase* Registry<Interface>::first = 0;

template <class Interface>
template <class Child>
Child Registry<Interface>::StaticElement<Child>::instance;

#endif /* REGISTRY_H_ */
