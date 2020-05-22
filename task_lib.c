#include "task_lib.h"

tcb_t tcb[MAX_TASKS];

/**
  * @brief  Scheduler update
  * @param  void
  * @retval void
  */
void sch_update(void)
{
    uint8_t index;

    for (index = 0; index < MAX_TASKS; index++)
    {
        /* Check if there is a task at this location */
        if (tcb[index].p_tack_cb)
        {
            if (tcb[index].delay == 0)
            {
                /* The task is duo to run */
                if (tcb[index].co_op)
                {
                    /* If it is a co-operative task, increment the RunMe flag */
                    tcb[index].run_me += 1;
                }
                else
                {
                    /* If it is a pre-emptive task, run it IMMEDIATELY */
                    tcb[index].p_tack_cb();

                    if (tcb[index].period == 0)
                    {
                        tcb[index].p_tack_cb = 0;
                    }
                }

                if (tcb[index].period)
                {
                    /* Schedule this periodic task to run again */
                    tcb[index].delay = tcb[index].period;
                }
            }
            else
            {
                tcb[index].delay -= 1;
            }
        }
    }
}

/**
  * @brief  Dispathch tasks
  * @param  void
  * @retval void
  */
void dispathch_tasks(void)
{
    uint8_t index;

    /* Dispatches (runs) the next task (if one is ready) */
    for (index = 0; index < MAX_TASKS; index++)
    {
        /* Only dispatching co_operative tasks */
        if (tcb[index].co_op && tcb[index].run_me > 0)
        {
            tcb[index].p_tack_cb();

            tcb[index].run_me -= 1;

            /* Periodic tasks will automatically run again,if this is a one shot' task, remove it from the array*/
            if (tcb[index].period == 0)
            {
                tcb[index].p_tack_cb = 0;
            }
        }
    }
}

/**
  * @brief  Add task
  * @param  del : Delay time
  * @param  per : Period
  * @param  co_op : co_operative
  * @retval -1 : failed  index : success
  */
int8_t add_task(void (*p_func)(void),
                uint16_t del,
                uint16_t per,
                uint8_t co_op)
{
    uint8_t index = 0;

    /* First find a gap in the array (if there is one) */
    while ((tcb[index].p_tack_cb != 0) && (index < MAX_TASKS))
    {
        index++;
    }

    if (index == MAX_TASKS)
    {
        /* We reached the end of the list */
        return -1;
    }

    tcb[index].p_tack_cb = p_func;
    tcb[index].delay = del;
    tcb[index].period = per;
    tcb[index].co_op = co_op;
    tcb[index].run_me = 0;
    return index;
}

#if (EN_DELETE_TASK != 0)
/**
  * @brief  Delete task
  * @param  index : Task index
  * @retval -1 : failed  index : success
  */
int8_t delete_task(uint8_t index)
{
    if (tcb[index].p_tack_cb == 0)
    {
        /* No task at this location... */
        return -1;
    }

    /* Delete the success  */
    tcb[index].p_tack_cb = 0;
    tcb[index].delay = 0;
    tcb[index].period = 0;
    tcb[index].run_me = 0;

    return 0;
}
#endif