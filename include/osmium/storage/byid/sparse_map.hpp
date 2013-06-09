#ifndef OSMIUM_STORAGE_BYID_SPARSE_MAP_HPP
#define OSMIUM_STORAGE_BYID_SPARSE_MAP_HPP

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

#include <google/sparse_hash_map>

using google::sparse_hash_map;      // namespace where class lives by default

#include <map>

#include <stdio.h>
#include <osmium/storage/byid.hpp>

namespace Osmium {

    namespace Storage {

        namespace ById {

            /**
            * The SparseMap store stores items in a Google sparse map,
            * a data structure that implement memory-efficient hash map
            *
            * Use this node location store if the ID space is only sparsly
            * populated, such as when working with smaller OSM files (like
            * country extracts).
            */
            template <typename TValue>
            class SparseMap : public Osmium::Storage::ById::Base<TValue> {

            public:

                /**
                * Constructor.
                *
                * @param grow_size is ignored fo rthis type of storage.
                */
                SparseMap(const uint64_t grow_size=10000) :
                    Base<TValue>(),
                    m_grow_size(grow_size),
                    m_items() {
                }

                void set(const uint64_t id, const TValue value) {
                    m_items[id] = value;
                }

                const TValue operator[](const uint64_t id) const {
	            typename sparse_hash_map<uint64_t, TValue>::const_iterator iter = m_items.find(id);
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
                    return (m_items.size()) * sizeof(TValue);
                }

                void clear() {
                    m_items.clear();
                }

            private:

                uint64_t m_grow_size;

                sparse_hash_map<uint64_t, TValue> m_items;

            }; // class SparseMap

        } // namespace ById

    } // namespace Storage

} // namespace Osmium

#endif // OSMIUM_STORAGE_BYID_SPARSE_MAP_HPP
