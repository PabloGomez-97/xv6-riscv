// Mutual exclusion lock.
#ifndef SPINLOCK_H
#define SPINLOCK_H
struct spinlock {
    uint locked;        // Is the lock held?
    char *name;         // Name of lock.
    struct cpu *cpu;    // The CPU holding the lock.
    uint pcs[10];       // Call stack (an array of program counters)
};
#endif // SPINLOCK_H