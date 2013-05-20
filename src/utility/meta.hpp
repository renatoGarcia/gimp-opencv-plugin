/* Gimp OpenCV Plugin
 *
 * Copyright (c) 2013 see AUTHORS file.
 *
 * Gimp OpenCV Plugin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Gimp OpenCV Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gimp OpenCV Plugin. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _UTILITY_META_HPP_
#define _UTILITY_META_HPP_

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#define GET_ITEM(z, n, data) boost::get<n>(BOOST_PP_LIST_AT(data, 0))BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(n, BOOST_PP_LIST_AT(data, 1)))

#define UNPACK_TUPLE(tuple, first, last) \
    BOOST_PP_REPEAT_FROM_TO(first, BOOST_PP_INC(last), GET_ITEM, (tuple, (last, BOOST_PP_NIL)))

#endif /* _UTILITY_META_HPP_ */
