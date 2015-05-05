/*
 * Copyright (C) 2014  Raphaël Poggi <poggi.raph@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MUTEX_H
#define MUTEX_H

#include <task.h>
#include <queue.h>

#define NR_MUTEX_HOLD	10

struct mutex {
	unsigned char lock;
	struct task *owner;
	unsigned int waiting;
	struct list waiting_tasks;
};

void mutex_lock(struct mutex *mutex);
void mutex_unlock(struct mutex *mutex);

static inline void init_mutex(struct mutex *mutex) {
    mutex->lock = 0;
    mutex->owner = NULL;
    mutex->waiting = NULL;
}

#endif /* MUTEX_H */
