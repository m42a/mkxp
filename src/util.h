/*
** util.h
**
** This file is part of mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_H
#define UTIL_H

static inline int
wrapRange(int value, int min, int max)
{
	if (value >= min && value <= max)
		return value;

	while (value < min)
		value += (max - min);

	return value % (max - min);
}

template<typename T>
static inline T bound(T value, T min, T max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}

template<typename T>
static inline T min(T value1, T value2)
{
	return (value1 < value2) ? value1 : value2;
}

template<typename T>
static inline T max(T value1, T value2)
{
	return (value1 > value2) ? value1 : value2;
}

static inline int
findNextPow2(int start)
{
	int i = 1;
	while (i < start)
		i <<= 1;

	return i;
}

#define ARRAY_SIZE(obj) (sizeof(obj) / sizeof((obj)[0]))

#define elementsN(obj) const int obj##N = ARRAY_SIZE(obj)

#define DECL_ATTR_DETAILED(name, type, keyword1, keyword2) \
	keyword1 type get##name() keyword2; \
	keyword1 void set##name(type value);

#define DECL_ATTR(name, type) DECL_ATTR_DETAILED(name, type, , const)
#define DECL_ATTR_VIRT(name, type) DECL_ATTR_DETAILED(name, type, virtual, const)
#define DECL_ATTR_STATIC(name, type) DECL_ATTR_DETAILED(name, type, static, )
#define DECL_ATTR_INLINE(name, type, loc) \
	type get##name() const { return loc; } \
	void set##name(type value) { loc = value; }

/* Undef this if not needed */
#define CHK_DISP GUARD_DISPOSED

#define DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	type klass :: get##name() keyword1 \
	{ \
		CHK_DISP \
		return location; \
	}

#define DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	void klass :: set##name(type value) \
{ \
	CHK_DISP \
	location = value; \
}

#define DEF_ATTR_RD_SIMPLE(klass, name, type, location) \
	DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, const)
#define DEF_ATTR_SIMPLE(klass, name, type, location) \
	DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, const)

#define DEF_ATTR_SIMPLE_STATIC(klass, name, type, location) \
	DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, )

#endif // UTIL_H
