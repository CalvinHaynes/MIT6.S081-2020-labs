// lab 6
#include "types.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
// cow reference count
struct{
    uint8 ref_cnt;
    struct spinlock lock;
}cows[(PHYSTOP - KERNBASE) >> 12];

// increase the reference count
void increfcnt(uint64 pa){
    if(pa < KERNBASE){
        return;
    }
    pa = (pa - KERNBASE) >> 12;
    acquire(&cows[pa].lock);
    ++cows[pa].ref_cnt;
    release(&cows[pa].lock);
}

// decrease the reference count

uint8 decrefcnt(uint64 pa){
    uint8 ret;
    if(pa < KERNBASE){
        return 0;
    }
    pa = (pa - KERNBASE) >> 12;
    acquire(&cows[pa].lock);
    ret = --cows[pa].ref_cnt;
    release(&cows[pa].lock);
    return ret;
}