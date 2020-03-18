#ifndef _TASK_LIB_H
#define _TASK_LIB_H
/*___________________CONSTANT___________________*/

#define MAX_TASKS (3)

/*___________________FUNCITON___________________*/

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned int uint16_t;
#endif
#ifndef uint32_t
typedef unsigned long uint32_t;
#endif

#ifndef int8_t
typedef char int8_t;
#endif
#ifndef int16_t
typedef int int16_t;
#endif
#ifndef int16_t
typedef long int32_t;
#endif


/* Task control block */
typedef struct tcb
{
    void (*p_tack_cb)(void);

    uint16_t delay;
    uint16_t period;

    uint8_t run_me;

    uint8_t co_op;
} tcb_t;

/*___________________EXTERNDA___________________*/
void sch_update(void);
void dispathch_tasks(void);
int8_t add_task(void (*p_func)(void),
                uint16_t del,
                uint16_t per,
                uint8_t co_op);
                
#ifdef EN_DELETE_TASK
int8_t delete_task(uint8_t index);
#endif
#endif
