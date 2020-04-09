#!/bin/python3



'''

== Examples ==
#This is a comment. From the first "#" to the end of the line is a comment

Timeout=200 #Assign timeout to 200ms

L1=1  #Set button L1 to be addressed as id 1
L3=   #2  (Auto increment if left blank)
R1=   #3
R2=   #4

L1 + R2: EXPR1             #Button L1 and L2 at the same time; Set expression to EXPR1
L1 > L1: EXPR2             #Button L1 then L2, one after another; Set expression to EXPR2
L1 + R1 > L3: EXPR3        #Button L1 and R1, then L3; Set expression to EXPR3
L1 + L3: "\x12\xA3"        #Button L1 and L3; Send custom string (Useful for custom command sequences)
$ R1 + R2: EXPR4           #As soon as R1 and R2 are pressed, set Expr4. Then return to the previous expression
R2: "\xF3" EXPR5 "\xC5"    #transmit 0xF3, followed by EXPR5 code, followed by 0xC5


== Notes ==
Confirmed on each rising edge
Action after the last falling edge
Leave button assignment blank for auto increment
auto increment starts at 0
Use "$" to signify momentary expressions. Return to previous expression after trigger has been released 

== Code structure ==
test each combo against the pressed buttons
if the combo is pressed, then set active
if combo is active and buttons beyond the combo are pressed, then it is reset
if the entire combo is released, it progresses to the next stage. Timer starts

if a combo is inactive and is beyond the first state, then after the timeout has passed, it is cleared

the last state of a combo sets the expression
Then all active states are cleared


== Structs & Variables ==
struct Combo
{
	u8 Len
	u8 Buttons[]
}

struct Sequence
{
	u8 SequenceLen
	Combo Sequence[]
	u8 SequencePos
	u8 Active
}

u8 NumSequences
Sequence Sequences[]

== Pseudo Code ==
for each combo:
	if combo is pressed:
		set combo state to Pending


'''


import os

FButtons="Buttons.txt"
FFrameManifest="../ExpressionCtl_FeatherM0/FrameManifest.txt"
FButtonData="ButtonData.bin"
FNumSequences="NumSequences.val"


try: f = open(FButtons, 'r')
except: print("Error opening file \"{0}\"".format(FButtons)); exit(-1)
FButtons=f

try: f = open(FFrameManifest, 'r')
except: print("Error opening file \"{0}\"".format(FFrameManifest)); exit(-1)
FFrameManifest=f

try: f = open(FButtonData, 'wb')
except: print("Error opening file \"{0}\"".format(FButtonData)); exit(-1)
FButtonData=f

try: f = open(FNumSequences, 'w')
except: print("Error opening file \"{0}\"".format(FNumSequences)); exit(-1)
FNumSequences=f

def ValtoHx(val, length):
	return val.to_bytes(length, byteorder="little")

def StrtoHx(string):
	b = b''
	for s in string:
		b += ValtoHx(ord(s), 1)
	return b

def ChecksumInt_u8(val, numbytes):
	checksum = 0
	for i in range(numbytes):
		checksum = checksum ^ (val & 0xFF)
		val = val >> 8
	return checksum



'''
=== Parse FrameManifest.txt ===
'''

# Expression_template = {"Start":[], "Loop":[], "End":[]}
Expressions = {}
lastExpr = ""

def ParseManifestLine(line):
	global Expressions
	global lastExpr
	
	if (not line[0].isspace()):
		lastExpr = line
		Expressions[lastExpr] = {} #{"Start":[], "Loop":[], "End":[]}
	else:
		line = line.lstrip()
		section = line.split(":")
		pairs = [i for i in section[1].split(" ") if len(i)] #split by spaces, ignoring large sections of spaces
		
		dispStartFrames = []
		for keyval in pairs:
			keyval = keyval.split("=")
			dispStartFrames += [int(keyval[1])]
			# print(keyval, end=" ")
		# print()
		# print(dispStartFrames)
		Expressions[lastExpr][section[0]] = dispStartFrames
		

DoParse=True
while DoParse:
	line=FFrameManifest.readline()
	if (len(line)):
		line=line.split("#")[0].rstrip()
		if len(line):
			ParseManifestLine(line)
	else:
		DoParse=False

for expr in Expressions:
	print(expr)
	for section in Expressions[expr]:
		print("  ", section, ":", Expressions[expr][section])


# u8 Display, u16 Index, u8 checksum
def GenDisplayUpdateCmd(display, index):
	checksum = ChecksumInt_u8(display, 1) ^ ChecksumInt_u8(index, 2)
	return ValtoHx(display, 1) + ValtoHx(index, 2) + ValtoHx(checksum, 1)

#Convert Expression name to hex command data
def GetCmdFromExpr(exprname):
	cmd = b""

	if exprname in Expressions:
		d = 0
		for disp in Expressions[exprname]["Start"]:
			cmd += GenDisplayUpdateCmd(disp, d)
			d+=1
	else:
		return StrtoHx("["+exprname+"]")

	return cmd



'''
=== Parse Buttons.txt ===
'''

Variables = { "Timeout":-1 }
ButtonID = 0
Buttons = { }
Sequences = [ ]

def First(seq1, seq2, string):
	if seq1 in string and seq2 in string:
		return seq1 if string.find(seq1) < string.find(seq2) else seq2
	elif seq1 in string:
		return seq1
	elif seq2 in string:
		return seq2
	else:
		return False

def ParseLine(line):
	global Variables
	global ButtonID
	global Buttons
	global Sequences
	
	# print("\n\"" + line + "\"")

	#Key-Value Pair
	if "=" in line:
		keyval = line.replace(" ", "").split("=")
		
		if keyval[0] in Variables:
			Variables[keyval[0]] = keyval[1]
		else:
			if len(keyval[1]):
				ButtonID = int(keyval[1])
			Buttons[keyval[0]] = ButtonID
			ButtonID+=1

	#Combo Sequence
	if ":" in line:
		sequstr = line.split(":")[0].replace(" ", "")
		cmdstr = line.split(":")[1].rstrip()
		cmd = b""
		sequence = [ ]
		momentary = False


		if "$" in sequstr:
			momentary = True
			sequstr = sequstr.replace("$", "")
		#Button Combo
		for combo in sequstr.split(">"):
			if len(combo):
				sequence += [[]]
				for button in combo.split("+"):
					sequence[-1] += [Buttons[button]]

		#Command
		quotes=False
		while (len(cmdstr)):
			if not quotes:
				cmdstr = cmdstr.lstrip()
			# print(cmdstr)
			if not quotes and (First(" ", "\"", cmdstr) == " " or First(" ", "\"", cmdstr) == False):
				# Space is first
				part = cmdstr.partition(" ")
				# print("Space ", part)
				cmdstr = part[2]
				cmd += GetCmdFromExpr(part[0])
			else:
				# Quotes is first
				part = cmdstr.partition("\"")
				# print("Quotes ", part)
				if quotes: 
					cmd += StrtoHx(bytes(part[0], "utf-8").decode("unicode_escape")) #Process escape codes
				cmdstr = part[2]
				quotes = not quotes

		Sequences += [{}]
		Sequences[-1]["Sequence"] = sequence
		Sequences[-1]["Command"] = cmd
		Sequences[-1]["Momentary"] = momentary

		# print(sequence)
		# print(cmd)


DoParse=True
while DoParse:
	line=FButtons.readline()
	if (len(line)):
		line=line.split("#")[0].rstrip()
		if len(line):
			ParseLine(line)
	else:
		DoParse=False

print()
print(Variables)
print(Buttons)
print()
for seq in Sequences:
	print(seq)


'''
=== Write Output ===

Binary Format

u8 Timeout
# u8 NumSequences
struct Sequences
{
	u8 Momentary
	u8 NumCombos
	struct Combos
	{
		u8 NumButtons
		struct Buttons
		{
			u8 ButtonNum
		}
	}[NumCombos]
	u8 CommandLen
	u8 Command[CommandLen]
}[NumSequences]

'''


ButtonDataBuffer = b""

ButtonDataBuffer += ValtoHx(int(Variables["Timeout"]),1)
# ButtonDataBuffer += ValtoHx(len(Sequences),1)
for sequence in Sequences: #Struct Sequences
	ButtonDataBuffer += ValtoHx(1 if sequence["Momentary"] else 0,1) #Momentart
	ButtonDataBuffer += ValtoHx(len(sequence["Sequence"]),1) #NumCombos
	for combo in sequence["Sequence"]: #Struct Combos
		ButtonDataBuffer += ValtoHx(len(combo),1) #NumButtons
		for button in combo: #Struct Buttons
			ButtonDataBuffer += ValtoHx(button,1) #ButtonNum

	ButtonDataBuffer += ValtoHx(len(sequence["Command"]),1) #CommandLen
	ButtonDataBuffer += sequence["Command"] #Command

FButtonData.write(ButtonDataBuffer)

FNumSequences.write(str(len(Sequences)))