#ifndef MACHINE_H
#define MACHINE_H

#include "copyright.h"
#include "utility.h"
#include "translate.h"
#include "disk.h"

#define TLBSize 4

enum ExceptionType
{
  NoException,
  SyscallException,
  PageFaultException,
  ReadOnlyException,

  BusErrorException,

  AddressErrorException,

  OverflowException,
  IllegalInstrException,

  NumExceptionTypes
};

#define StackReg 29
#define RetAddrReg 31
#define NumGPRegs 32
#define HiReg 32
#define LoReg 33
#define PCReg 34
#define NextPCReg 35
#define PrevPCReg 36
#define LoadReg 37
#define LoadValueReg 38
#define BadVAddrReg 39

#define NumTotalRegs 40

class Instruction
{
public:
  void Decode();

  unsigned int value;

  char opCode;

  char rs, rt, rd;
  int extra;
};

class Machine
{
public:
  Machine(bool debug);

  ~Machine();

  void Run();

  int ReadRegister(int num);

  void WriteRegister(int num, int value);

  void OneInstruction(Instruction *instr);

  void DelayedLoad(int nextReg, int nextVal);

  bool ReadMem(int addr, int size, int *value);

  bool WriteMem(int addr, int size, int value);

  ExceptionType Translate(int virtAddr, int *physAddr, int size, bool writing);

  int GetPA(unsigned vaddr);

  void RaiseException(ExceptionType which, int badVAddr);

  void Debugger();
  void DumpState();

  char *mainMemory;

  int registers[NumTotalRegs];

  TranslationEntry *tlb;

  TranslationEntry *KernelPageTable;
  unsigned int KernelPageTableSize;

private:
  bool singleStep;

  int runUntilTime;
};

extern void ExceptionHandler(ExceptionType which);

unsigned int WordToHost(unsigned int word);

unsigned short ShortToHost(unsigned short shortword);

unsigned int WordToMachine(unsigned int word);

unsigned short ShortToMachine(unsigned short shortword);

#endif
