/*
 * Copyright (C) 2013 Alexander Saprykin <xelfium@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 */

#include "ptimeprofiler.h"
#include "pmem.h"

#include <time.h>

struct _PTimeProfiler {
	puint64		counter;
};

static puint64 __p_time_profiler_current_ticks (const PTimeProfiler *profiler);

static puint64
__p_time_profiler_current_ticks (const PTimeProfiler *profiler)
{
	pint64 val;

	if (profiler == NULL)
		return 0;

	if ((val = (pint64) time (NULL)) == -1) {
		P_ERROR ("PTimeProfiler: Failed to get time using time()");
		return 0;
	}

	return (puint64) (val * 1000000);
}

P_LIB_API PTimeProfiler *
p_time_profiler_new ()
{
	PTimeProfiler *ret;

	if ((ret = p_malloc0 (sizeof (PTimeProfiler))) == NULL)
		return NULL;

	ret->counter = __p_time_profiler_current_ticks (ret);

	return ret;
}

P_LIB_API void
p_time_profiler_reset (PTimeProfiler *profiler)
{
	if (profiler == NULL)
		return;

	profiler->counter = __p_time_profiler_current_ticks (profiler);
}

P_LIB_API puint64
p_time_profiler_elapsed_usecs (const PTimeProfiler *profiler)
{
	if (profiler == NULL)
		return 0;

	return __p_time_profiler_current_ticks (profiler) - profiler->counter;
}

P_LIB_API void
p_time_profiler_free (PTimeProfiler *profiler)
{
	if (profiler == NULL)
		return;

	p_free (profiler);
}