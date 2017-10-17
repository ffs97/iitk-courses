# NachOS Implementation

System calls to be implemented -

-	GetReg
-	GetPA
-	GetPIP
-	GetPPID
-	NumInstr
-	Time
-	Yield
-	Sleep
-	Exec
-	Exit
-	Join
-	Fork

All custom editings have been wrapped around by comments with the text "CUSTOM"


## General Workflow
--------------------------------------------------------------------------------

The system calls are defined within system call wrapper functions which are
called from within a user program. These system calls are responsible for
incrementing the program counter and hence handle the program flow. The system
calls are hard coded in the ExceptionHandler() function in exception.cc file in
an if...else block

The type of system call / exception is passed as an argument *which* in this
function. The values passed to and from these calls are handled through machine
registers. *Register 2* is used to pass return value and *Registers 4 to 7* are
used to pass arguments.

Common steps in most of the functions is reading the arguments from machine
registers, writing the return value to the machine registers and incrementing
the program counter.

**Reading Arguments**

>>	machine->readRegister(4)

This returns the value read in the 4th register. Here machine is an object
instance of the complete NachOS machine.

**Returning Value**

>>	machine->writeRegister(2, value)

This writes the *value* in the corresponding register, i.e.  register 2.

**Incrementing Program Counter**

>>	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
>>	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
>>	machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);

Since Program Counter is also a register, we can just increment the register
value by 4 to update the program counter. This step in crucial to continue
execution of the user program, otherwise the current system call will keep on
executing in an infinite loop.




## Implementation of Each System Call

Below is the individual implementation of each individual system call -



### GetReg
--------------------------------------------------------------------------------
*Syscall_GetReg*

For GetReg, first just  the register that is to be read is read from register 4
as the input is passed in that. Then the value of the register is read from
there using the ReadRegister function. Then finally the vaue read is written
into the register 2 as it is the return register.



### GetPA
--------------------------------------------------------------------------------
*Syscall_GetPA*

This syscall returns the Physical Address of the passed virtual page. This is
similar to what is done in the translate function of machine class except the
fact that we need not check if its read only, or set the dirty bit, use bit as
true. Hence the remaining part of the code is organized in the GetPA function
in the Exception.cc file itself. First we check if there is a TLB or a not. If
it is not there then we use the KernelPageTable. In this first we check in the
cache that is the TLB. If the virtual page is loaded in the TLB, then we can
directly find the physical page corresponding to it. Then its simply the
Physical Page Number * PageSize + offset. Otherwise we find the pointer to the
passed virtual Address^{th} entry in KernelPageTable and then use it to finf
the physical page number and then the same as before.



### GetPID
--------------------------------------------------------------------------------
*Syscall_GetPID*

In order to get the PID of a thread, we need to read the *pid* variable of the
thread instance. In order to access this variable, we created a public method
*int GetPID()* which returns the value of the thread pid.

In order to assign a unique PID to a thread, we kept a global variable *maxPID*
which stores the maximum PID observed yet, and is incremented on initiation of
each new thread. Although this helps in assigning unique PIDs, however it limits
the total number of threads created in the complete running of the OS as the max
PID is in (positive) integer range, and we never free a previously used PID even
after deletion of the thread. Another approach to this could be to have a queue
of the used PIDs and assign the smallest PID that is free to a new thread during
initiation. However this method adds overhead to the thread creation and
requires more space to maintain the global queue of PIDs.

Hence we just need to write the return value from *currentThread->GetPID()* and
write it to register 2 as the return value for this syscall.

Since we need to continue execution in the current program, we increment the
program counter before returning. 



### GetPPID
--------------------------------------------------------------------------------
*Syscall_GetPPID*

In order to get the PPID of a thread, we need to read the ppid variable of the
thread instance. In order to access this variable, we created a public method
*int GetPPID()* which returns the value of the thread ppid.

The Parent PID is obtained during thread initialization.  Since any thread other
than the main thread will only be initialized by some other thread, that thread
needs to be running. Hence the parent thread will be *currentThread* itself. In
case the *currentThread* is NULL, i.e. this thread is the main thread, we will
assign *ppid* = -1.

Hence we just need to write the return value from *currentThread->GetPPID()* and
write it to register 2 as the return value for this syscall.

Since we need to continue execution in the current program, we increment the
program counter before returning.



### NumInstr
--------------------------------------------------------------------------------
*Syscall_NumInstr*

Since the number of instructions need to be different per thread, we define a
public variable named *numInstructions* in the *Thread* Class. Since for each
user instruction, the method *Interrupt::OneTick()* is called, we just increment
the *numInstructions* variable of the calling thread, i.e.  *currentThread*
whenever *OneTick()* is called. However we increment only when the machine is
not in system mode, otherwise it is not to be considered as an interrupt
generated by a user instruction.

In order to access the value, we call *currentThread->numInstructions* and write
it to register 2 as the return value from this syscall.

Since we need to continue execution in the current program, we increment the
program counter before returning.



### Time
--------------------------------------------------------------------------------
*Syscall_Time*

The total time is proportional to the ticks of the machine.  Although not
precise, but we can use this value as the total simulation time of the NachOS
main thread. This value is stored in the variable *totalTicks* of the class
instance *stats* of the Class *Statistics*.

Hence we just need to access the value in this variable, i.e.
*stats->totalTicks* and write it to register 2 as the return value from this
syscall.

Since we need to continue execution in the current program, we increment the
program counter before returning.



### Yield
--------------------------------------------------------------------------------
*Syscall_Yield*

In Yield SysCall we just need to use the Yield function to yield to the CPU,
although one thing to keep in mind is that we need to move the program counters
first and then yield the CPU, otherwise we might get stuck in an infinite loop 
of CPU Yield, as the program will never move to the next instruction.



### Sleep
--------------------------------------------------------------------------------
*Syscall_Sleep*

In order to keep track of the all the waiting/sleeping threads, we need to keep
a queue, with all blocked threads along with their wake up time. For easier
operations, we keep this queue as sorted. This queue is declared as a global
List in *system.cc* and is defined as *waitingThreadList*.

Although putting a thread to sleep is done by a syscall, however we need to
have a method in order to restore or wake these threads up. We achieve this by
having a method which wakes up all threads whose wake up time has been passed
i.e.  wakeUpTime <= totalTicks, and this method is called on every interrupt.
This is implemented in *system.cc* with the method being defined as
*restoreSleepingThreads()* called on every interrupt through the
*TimerInterruptHandler()* function in the same file.

In case the *sleepTime* for the calling thread is 0, we need not put it in the
waiting queue, however we still need to yield the cpu. Otherwise, we put the
thread in the waiting queue.  (Implemented in Threads Class) Also, we put the
thread to sleep by removing it from the readyQueue and putting it's status as
BLOCKED.

Since we need to continue execution of the user program, we need to increment
the program counter.



### Exit
--------------------------------------------------------------------------------
*Syscall_Exit*

There are multiple alterations to the threads class in case of Exit and Join.

First of all, in order to communicate between parent and child. For this, we
maintain two private List objects for each thread instance
-	childQueue: Maintains all the created Childs of that thread with the pid of
	the child as the key
-	childExitCodesQueue: Maintains the exit codes of all child threads (which
	have exited) with the pid of the child as the key

We also need a parent object for each Thread instance, which stores the pointer
to the parent thread. Other than all of this, we need to keep an integer which
stores the pid of the thread this thread is waiting on, if it is at sleep
("waitingThreadPID"). In case the value of this variable is -1, then either the
parent is not sleeping, or it is sleeping, but will be woken up by an interrup,
after a certain amount of time.

On Exit, we need to do three things
-	Change the child's parent to NULL, i.e. orphan it
-	Add the currentThread's (which is being exited) exit code to the parent's
	childExitCodesQueue (if parent != NULL)
-	If no threads are left (running or waiting), then we halt the machine

In order to achieve the above functionality, we use a set of functions, all
implemented in the Threads class (thread.cc and thread.h)
-	UpdateParent: To change the parent of the thread instance (used only to
	orphan the child)
-	AddChild: To add a child in the thread instance's childQueue
-	AddChildExitCode: To add the exit code of a child in the thread instance's
	childExitCodesQueue (called when the child is exited)
-	AddExitCode: A connecting function, which simply calls the AddChildExitCode
	function on exit (required as the parent is private)
-	UpdateWaitingThreadPID: Update the parent thread's waiting child's PID
-	WakeUpParent: If the parent is sleeping and if the parent's waitingThreadPID
	is set to the PID of this child, we wake up the parent, and set the value of
	waitingThreadPID as "-1"

In the Exit exception handler, we call the FinishThread method, which we have
augmented, which essentially iterates over the childQueue and updates the
childs' parent to NULL. Here, we also check if there are no threads remaining,
in which case we Halt the machine. This is tracked using a global variable
"numThreadsCurrent", which is incremented 



### Join
--------------------------------------------------------------------------------
*Syscall_Join*

Most of the work is done in the exit part. In case of join, we search for the
exit code of the child in the parent, where there are three cases possible
-	The child on which join is called has exited
-	The child on which join is called has not exited yet
-	The PID passed is not the PID of any of this thread's children

In order to achieve this, we first search if the PID is actually of any of the
child (by iterating over the childQueue). If not found, we return -1.
Otherwise, we search for the child in the childExitCodesQueue using the
function "GetChildExitCode". For this function, we require a temporary list, as
there is no way to traverse the list without deleting the elements. We return
"-1" in case we do not find the child's pid in the childQueue, which signifies
it is not the child of that parent, otherwise we return the exitCode of the
child. If not found, then we put the thread to sleep, update it's
waitingThreadPID as the pid of this child, and then on the exit call of the
child, we wake up the parent.



### Exec
--------------------------------------------------------------------------------
*Syscall_Exec*

First , obtain the string of the file address that is to be executed. It is
read in memval and fed to the filename string of fixed size with '\0' at its
end. Then an address space is created as explained below. The file is then
loaded into the created address space and executed. Then we clear the previous
address space and assign this newly created address space to the process.
Finally the Usermode cpu registers are loaded and context switch is done.

Now the main part of allocating the memory for exec. This is done in the
addrspace.cc file.  Here the concept is that we allocate the memory for this
call after the space that has been allocated yet. That is after the
numTotalPages * PageSize. This space has presently been unallocated to any
program so we first assert to check if we are not exceding the number of pages
we can have. Then we zero the bytes in there and load the file passed as
parameter to this function. Another thing that needs to be changed is in the
KernelPageTable for translating between physical and virtual pages. The
physical page will be now numTotalPages  + 'i' for the i^{th}  virtual page
number. 

Here we don't need to move the PC counters.



### Fork
--------------------------------------------------------------------------------
*Syscall_Fork*

Similar in nature to Exec call, but here we first need to move the Program
Counters as the parent process registors are then saved, if we don't do this
then we end up in an infinite loop of creating childs. Then we create the child
thread and allocate memory to it abiet with a different constructor via
overloading the function. Then 0 is written to the register 2 as 0 is returned
to the child and we had loaded the context of the child in the main memory for
this instant. Then we save this state of registers to prepare for loading of
the parent via the ThreadFork function which is passed in the fork_init_func
function that restores the parent registers and starts the machine. Now it
proceeds from where it left and register 2 is written by the PID of the child.

Now coming to the part of allocating memory, here we have one key difference
from before. We need to copy the address space of the parent to the child as
well, thus we need to pass in the first physical addess of the loaded
kernelpagetable. Then this is coppied into the newely allocated memory of the
forked child in a way similar to exec, although here we give it the max
possible space possible that is the whole page table size. The parent field of
the child is set up as the currentThread as at that time, the curent thread is
the thread that has called the fork function.
