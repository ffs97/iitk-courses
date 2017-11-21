#include "copyright.h"
#include "machine.h"
#include "addrspace.h"
#include "system.h"

unsigned int
WordToHost(unsigned int word)
{
#ifdef HOST_IS_BIG_ENDIAN
    register unsigned long result;
    result = (word >> 24) & 0x000000ff;
    result |= (word >> 8) & 0x0000ff00;
    result |= (word << 8) & 0x00ff0000;
    result |= (word << 24) & 0xff000000;
    return result;
#else
    return word;
#endif /* HOST_IS_BIG_ENDIAN */
}

unsigned short
ShortToHost(unsigned short shortword)
{
#ifdef HOST_IS_BIG_ENDIAN
    register unsigned short result;
    result = (shortword << 8) & 0xff00;
    result |= (shortword >> 8) & 0x00ff;
    return result;
#else
    return shortword;
#endif /* HOST_IS_BIG_ENDIAN */
}

unsigned int
WordToMachine(unsigned int word) { return WordToHost(word); }

unsigned short
ShortToMachine(unsigned short shortword) { return ShortToHost(shortword); }

bool Machine::ReadMem(int addr, int size, int *value)
{
    int data;
    ExceptionType exception;
    int physicalAddress;

    DEBUG('a', "Reading VA 0x%x, size %d\n", addr, size);

    exception = Translate(addr, &physicalAddress, size, FALSE);
    if (exception != NoException)
    {
        machine->RaiseException(exception, addr);
        return FALSE;
    }
    switch (size)
    {
    case 1:
        data = machine->mainMemory[physicalAddress];
        *value = data;
        break;

    case 2:
        data = *(unsigned short *)&machine->mainMemory[physicalAddress];
        *value = ShortToHost(data);
        break;

    case 4:
        data = *(unsigned int *)&machine->mainMemory[physicalAddress];
        *value = WordToHost(data);
        break;

    default:
        ASSERT(FALSE);
    }

    DEBUG('a', "\tvalue read = %8.8x\n", *value);
    return (TRUE);
}

bool Machine::WriteMem(int addr, int size, int value)
{
    ExceptionType exception;
    int physicalAddress;

    DEBUG('a', "Writing VA 0x%x, size %d, value 0x%x\n", addr, size, value);

    exception = Translate(addr, &physicalAddress, size, TRUE);
    if (exception != NoException)
    {
        machine->RaiseException(exception, addr);
        return FALSE;
    }
    switch (size)
    {
    case 1:
        machine->mainMemory[physicalAddress] = (unsigned char)(value & 0xff);
        break;

    case 2:
        *(unsigned short *)&machine->mainMemory[physicalAddress] = ShortToMachine((unsigned short)(value & 0xffff));
        break;

    case 4:
        *(unsigned int *)&machine->mainMemory[physicalAddress] = WordToMachine((unsigned int)value);
        break;

    default:
        ASSERT(FALSE);
    }

    return TRUE;
}

ExceptionType
Machine::Translate(int virtAddr, int *physAddr, int size, bool writing)
{
    int i;
    unsigned int vpn, offset;
    TranslationEntry *entry;
    unsigned int pageFrame;

    DEBUG('a', "\tTranslate 0x%x, %s: ", virtAddr, writing ? "write" : "read");

    if (((size == 4) && (virtAddr & 0x3)) || ((size == 2) && (virtAddr & 0x1)))
    {
        DEBUG('a', "alignment problem at %d, size %d!\n", virtAddr, size);
        return AddressErrorException;
    }

    ASSERT(tlb == NULL || KernelPageTable == NULL);
    ASSERT(tlb != NULL || KernelPageTable != NULL);

    vpn = (unsigned)virtAddr / PageSize;
    offset = (unsigned)virtAddr % PageSize;

    if (tlb == NULL)
    {
        if (vpn >= KernelPageTableSize)
        {
            DEBUG('a', "virtual page # %d too large for page table size %d!\n",
                  virtAddr, KernelPageTableSize);
            return AddressErrorException;
        }
        else if (!KernelPageTable[vpn].valid)
        {
            DEBUG('a', "virtual page # %d too large for page table size %d!\n",
                  virtAddr, KernelPageTableSize);
            return PageFaultException;
        }
        entry = &KernelPageTable[vpn];
    }
    else
    {
        for (entry = NULL, i = 0; i < TLBSize; i++)
            if (tlb[i].valid && (tlb[i].virtualPage == vpn))
            {
                entry = &tlb[i];
                break;
            }
        if (entry == NULL)
        {
            DEBUG('a', "*** no valid TLB entry found for this virtual page!\n");
            return PageFaultException;
        }
    }

    if (entry->readOnly && writing)
    {
        DEBUG('a', "%d mapped read-only at %d in TLB!\n", virtAddr, i);
        return ReadOnlyException;
    }
    pageFrame = entry->physicalPage;

    if (pageFrame >= NumPhysPages)
    {
        DEBUG('a', "*** frame %d > %d!\n", pageFrame, NumPhysPages);
        return BusErrorException;
    }
    entry->use = TRUE;
    if (writing)
        entry->dirty = TRUE;
    *physAddr = pageFrame * PageSize + offset;
    ASSERT((*physAddr >= 0) && ((*physAddr + size) <= MemorySize));
    DEBUG('a', "phys addr = 0x%x\n", *physAddr);

    /* ------------------------ CUSTOM ------------------------ */
    physicalPagesList[pageFrame].usageTime = stats->totalTicks;
    physicalPagesList[pageFrame].reference = TRUE;
    /* ------------------------ CUSTOM ------------------------ */

    return NoException;
}

int Machine::GetPA(unsigned vaddr)
{
    unsigned vpn = vaddr / PageSize;
    unsigned offset = vaddr % PageSize;
    TranslationEntry *entry;
    unsigned int pageFrame;

    if ((vpn < KernelPageTableSize) && KernelPageTable[vpn].valid)
    {
        entry = &KernelPageTable[vpn];
        pageFrame = entry->physicalPage;
        if (pageFrame >= NumPhysPages)
            return -1;
        return pageFrame * PageSize + offset;
    }
    else
        return -1;
}
