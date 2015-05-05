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

#include <task.h>
#include <scheduler.h>
#include <stdio.h>
#include <mm.h>
#include <utils.h>
#include <queue.h>
#include <arch/svc.h>
#include <armv7m/system.h>

static struct task *current_task;
static int task_count = 0;

static void increment_task_priority(void)
{
	struct entry *e = (struct entry *)&runnable_tasks.head;
	struct task *task;

	while (e->next) {
		task = (struct task *)container_of(e, struct task, list_entry);
		task->priority++;
		e = e->next;
	}
}

static void insert_task(struct task *t)
{
	struct entry *e = (struct entry *)&runnable_tasks.head;
	struct task *task;

	while (e->next) {
		task = (struct task *)container_of(e, struct task, list_entry);

		if (t->priority > task->priority) {
			list_insert_before(&task->list_entry, &t->list_entry);
			break;
		}

		e = e->next;
	}
}

void task_init(void)
{
	list_init(&runnable_tasks);
}

void add_task(void (*func)(void), unsigned int priority)
{
	struct task *task = (struct task *)kmalloc(sizeof(struct task));
	task->state = TASK_RUNNABLE;
	task->pid = task_count;
	task->priority = priority;
	task->start_stack = TASK_STACK_START + (task_count * TASK_STACK_OFFSET);
	task->func = func;
	task->regs = (struct registers *)kmalloc(sizeof(struct registers));
	task->regs->sp = task->start_stack;
	task->regs->lr = (unsigned int)end_task;
	task->regs->pc = (unsigned int)func;

	/* Creating task context */
	create_context(task->regs, task);

	insert_task(task);

	task_count++;
}

void first_switch_task(void)
{
	SVC_ARG(SVC_TASK_SWITCH, NULL);
}

void switch_task(struct task *task)
{
	task->state = TASK_RUNNING;

	current_task->regs->sp = PSP();
	SET_PSP((void *)task->regs->sp);

	current_task->state = TASK_RUNNABLE;
	current_task = task;

	increment_task_priority();

	insert_task(current_task);
}

struct task *get_current_task(void)
{
	return current_task;
}

struct task *find_next_task(void)
{
	struct entry *e;
	struct task *task;

	e = list_get_head(&runnable_tasks);
	task = (struct task *)container_of(e, struct task, list_entry);

	return task;
}

