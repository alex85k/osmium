#ifndef OSMIUM_STORAGE_BYID_SIMPLE_MAP_HPP
#define OSMIUM_STORAGE_BYID_SIMPLE_MAP_HPP

/*

Copyright 2012 Jochen Topf <jochen@topf.org> and others (see README).

This file is part of Osmium (https://github.com/joto/osmium).

Osmium is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License or (at your option) the GNU
General Public License as published by the Free Software Foundation, either
version 3 of the Licenses, or (at your option) any later version.

Osmium is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License and the GNU
General Public License for more details.

You should have received a copy of the Licenses along with Osmium. If not, see
<http://www.gnu.org/licenses/>.

*/


#include <map>

#include <stdio.h>
#include <osmium/storage/byid.hpp>

namespace Osmium {

    namespace Storage {

        namespace ById {

            /**
            * This is the most simple storage based on standrd map class
	    * It is suitable when not many IDs are used and they are distributed sparsely across big interval
            */
            template <typename TValue>
            class SimpleMap : public Osmium::Storage::ById::Base<TValue> {

            public:

                /**
                * Constructor.
                *
                * @param grow_size is ignored for this type of storage
                */
                SimpleMap(const uint64_t grow_size=10000) :
                    Base<TValue>(),
                    m_grow_size(grow_size),
                    m_items() {
                }

                void set(const uint64_t id, const TValue value) {
                    m_items[id] = value;
                }

                const TValue operator[](const uint64_t id) const {
	            typename std::map<uint64_t, TValue>::const_iterator iter = m_items.find(id);
	            if (iter != m_items.end() )
                         return iter->second; 
                    else
                        return TValue(); 
//                       throw std::runtime_error("object with given ID was not found in SimpleMap storage");
                }

                uint64_t size() const {
                    return m_items.size();
                }

                uint64_t used_memory() const {
                    // unused items use 1 bit, used items sizeof(TValue) bytes
                    return (m_items.size()) * sizeof(TValue);
                }

                void clear() {
                    m_items.clear();
                }

            private:

                uint64_t m_grow_size;

		std::map <uint64_t, TValue> m_items;

            }; // class SimpleMap

        } // namespace ById

    } // namespace Storage

} // namespace Osmium

#endif // OSMIUM_STORAGE_BYID_SIMPLE_MAP_HPP
