print("Running Test 1: Decoding Logic I")
test1A = 0x00e0
test1B = 0x00ee
test1C = 0x0bf9
assert test1A & 0xf000 >> 12 == 0x0
assert test1A & 0x0fff == 0x00e0
assert test1B & 0x0fff == 0x00ee 
assert test1C & 0x0fff == 0xbf9

print("Running Test 1: Decoding Logic II")
test2 = 0x1aba
pc = 0
pc = test2 & 0x0fff
pc = pc - 2
assert (test2 & 0xf000) >> 12 == 0x1
pc = pc + 2
assert(pc == 0xaba)
print("Passed Test 1")
print("\n")


def callSubroutine(stackFrame, stackPointer, pc, opcode):
    if len(stackFrame) <= stackPointer[0] + 1:
        return 0
    stackPointer[0] += 1
    stackFrame[stackPointer[0]] = pc[0]
    pc[0] = opcode & 0x0fff
    pc[0] = pc[0] - 2

def returnSubroutine(stackFrame, stackPointer, pc):
    pc[0] = stackFrame[stackPointer[0]]
    if stackPointer[0] - 1 < 0:
        return 1
    stackPointer[0] -= 1
    pc[0] = pc[0] - 2

print("Running Test 2: Stack Frame I")
#Basic test on stack frames calling two different subroutines
stack = [0, 0, 0, 0]
sp = [0]
startAddress = 0xaaa
call1Address = 0x2bbb
call2Address = 0x2ccc
pc = [0xaaa]
callSubroutine(stack, sp, pc, call1Address)
pc[0] += 2
assert pc[0] == 0xbbb
callSubroutine(stack, sp, pc, call2Address)
pc[0] += 2
assert pc[0] == 0xccc
returnSubroutine(stack, sp, pc)
pc[0] += 2
assert pc[0] == 0xbbb
returnSubroutine(stack, sp, pc)
pc[0] += 2
assert pc[0] == 0xaaa
print("Running Test 2: Stack Frame II")
#Tests calling the same function, allocating more space than available, deallocating base stack frame
stack = [0, 0, 0, 0]
sp = [0]
startAddress = 0xaaa
call1Address = 0x2bbb
call2Address = 0x2ccc
pc = [0xaaa]

#Putting the same function on the stack twice
callSubroutine(stack, sp, pc, call1Address)
pc[0] += 2
assert pc[0] == 0xbbb
callSubroutine(stack, sp, pc, call2Address)
pc[0] += 2
assert pc[0] == 0xccc
callSubroutine(stack, sp, pc, call1Address)
pc[0] += 2
assert pc[0] == 0xbbb
returnSubroutine(stack, sp, pc)
pc[0] += 2
assert pc[0] == 0xccc
returnSubroutine(stack, sp, pc)
pc[0] += 2
assert pc[0] == 0xbbb
returnSubroutine(stack, sp, pc)
pc[0] += 2
assert pc[0] == 0xaaa

#Deallocating the base stack 
assert returnSubroutine(stack, sp, pc) == 1
for i in range(0, 3):
    callSubroutine(stack, sp, pc, call1Address)
    pc[0] += 2
#Allocating 1 frame over stack frame limit
print(sp[0])
assert callSubroutine(stack, sp, pc, call1Address) == 0
pc[0] += 2
print("Passed Test 2")
print("\n")
