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
#Basic test on stack frames calling two different subroutines. Two levels deep in terms of recursion. Base calls call1, within call1 the function calls call2
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

#Putting the same function on the stack twice. Three levels deep of recursion. Base calls call1, call1 calls call2, and call2 calls call1
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

print("Running Test 3: Binary-Coded Decimal")
#Decimal value of 171
testVal = 0xab
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 1
assert middleDigit == 7
assert leftDigit == 1

#Decimal value of 109
testVal = 0x6d
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 9
assert middleDigit == 0
assert leftDigit == 1

#Decimal value of 230
testVal = 0xe6
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 0
assert middleDigit == 3
assert leftDigit == 2

#Decimal value of 11
testVal = 0xb
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 1
assert middleDigit == 1
assert leftDigit == 0

#Decimal value of 1
testVal = 0x1
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 1
assert middleDigit == 0
assert leftDigit == 0

#Decimal value of 239
testVal = 0xef
rightDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
middleDigit = testVal % 10
testVal = (testVal - (testVal % 10)) / 10
leftDigit = testVal % 10
assert rightDigit == 9
assert middleDigit == 3
assert leftDigit == 2
print("Passed Test 3s")
print("\n")
