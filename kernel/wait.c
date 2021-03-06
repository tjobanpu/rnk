#include <kernel/wait.h>
#include <list.h>
#include <kernel/thread.h>
#include <errno.h>
#include <kernel/scheduler.h>
#include <kernel/spinlock.h>
#include <kernel/printk.h>

static void insert_waiting_thread(struct wait_queue *wait, struct thread *t)
{
#ifdef CONFIG_SCHEDULE_ROUND_ROBIN
	struct thread *thread;
#endif

	if (wait->count) {
#ifdef CONFIG_SCHEDULE_ROUND_ROBIN
		list_for_every_entry(&wait->list, thread, struct thread, event_node) {

			if (!list_next(&wait->list, &thread->event_node)) {
				list_add_after(&thread->event_node, &t->event_node);
				break;
			}
		}
#elif defined(CONFIG_SCHEDULE_PRIORITY)
		list_add_tail(&wait->list, &t->event_node);
#endif
	} else {
		list_add_head(&wait->list, &t->event_node);
	}


}

static void remove_waiting_thread(struct wait_queue *wait, struct thread *t)
{
	list_delete(&t->event_node);
}

int wait_queue_init(struct wait_queue *wait)
{
	if (!wait)
		return -EINVAL;

	list_initialize(&wait->list);
	wait->count = 0;

	return 0;
}

static int __wait_queue_block(struct wait_queue *wait, unsigned long *irqstate)
{
	struct thread *current_thread = get_current_thread();
	
	current_thread->state = THREAD_BLOCKED;
	remove_runnable_thread(current_thread);

	insert_waiting_thread(wait, current_thread);
	wait->count++;
			
	schedule_yield();

	arch_interrupt_restore(*irqstate, SPIN_LOCK_FLAG_IRQ);

	return 0;
}

static int __wait_queue_wake(struct wait_queue *wait, unsigned long *irqstate)
{
	struct thread *thread;

	if (!wait)
		return -EINVAL;

	if (wait->count) {
			wait->count--;

			thread = list_peek_head_type(&wait->list, struct thread, event_node);

			remove_waiting_thread(wait, thread);
			insert_runnable_thread(thread);

			schedule_yield();
	}

	arch_interrupt_restore(*irqstate, SPIN_LOCK_FLAG_IRQ);

	return 0;
}

int wait_queue_block_irqstate(struct wait_queue *wait, unsigned long *irqstate)
{
	int ret;

	if (!wait)
		return -EINVAL;

	ret = __wait_queue_block(wait, irqstate);

	arch_interrupt_save(irqstate, SPIN_LOCK_FLAG_IRQ);

	return ret;
}

int wait_queue_block(struct wait_queue *wait)
{
	int ret;
	unsigned long irqstate;

	if (!wait)
		return -EINVAL;

	arch_interrupt_save(&irqstate, SPIN_LOCK_FLAG_IRQ);

	ret = __wait_queue_block(wait, &irqstate);

	return ret;
}

int wait_queue_wake_irqstate(struct wait_queue *wait, unsigned long *irqstate)
{
	int ret;

	if (!wait)
		return -EINVAL;

	ret = __wait_queue_wake(wait, irqstate);

	arch_interrupt_save(irqstate, SPIN_LOCK_FLAG_IRQ);

	return ret;
}

int wait_queue_wake(struct wait_queue *wait)
{
	int ret;
	unsigned long irqstate;

	if (!wait)
		return -EINVAL;

	arch_interrupt_save(&irqstate, SPIN_LOCK_FLAG_IRQ);

	ret = __wait_queue_wake(wait, &irqstate);

	return ret;	
}
