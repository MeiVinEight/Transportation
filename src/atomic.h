#ifndef TRANSPORTATION_ATOMIC_H
#define TRANSPORTATION_ATOMIC_H

extern "C"
{
long _InterlockedCompareExchange(long volatile*, long, long);
char _InterlockedCompareExchange8(char volatile*, char, char);
long _InterlockedIncrement(long volatile *);
long _InterlockedDecrement(long volatile *);
}

#endif //TRANSPORTATION_ATOMIC_H
