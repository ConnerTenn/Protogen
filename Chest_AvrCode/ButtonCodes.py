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
FButtonData="ButtonData.bin"


try: f = open(FButtons, 'r')
except: print("Error opening file \"{0}\"".format(FButtons)); exit(-1)
FButtons=f

try: f = open(FButtonData, 'wb')
except: print("Error opening file \"{0}\"".format(FButtonData)); exit(-1)
FButtonData=f

def ValtoHx(val, length):
	return val.to_bytes(length, byteorder="little")

def StrtoHx(string):
	b = b''
	for s in string:
		b += ValtoHx(ord(s), 1)
	return b


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
	
	print("\n\"" + line + "\"")

	#Key-Value Pair
	if "=" in line:
		keyval = line.split("=")
		
		specialvar=False
		for v in Variables: specialvar = True if keyval[0] == v else False
		if specialvar:
			Variables[keyval[0]] = keyval[1]
		else:
			if len(keyval[1]):
				ButtonID = int(keyval[1])
			Buttons[keyval[0]] = ButtonID
			ButtonID+=1

	#Combo Sequence
	if ":" in line:
		sequstr = line.split(":")[0]
		cmdstr = line.split(":")[1].rstrip()
		cmd = b""
		sequence = { }

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
				cmd += StrtoHx("["+part[0]+"]") ##TODO FIX WITH LABEL REPLACEMENT
			else:
				# Quotes is first
				part = cmdstr.partition("\"")
				# print("Quotes ", part)
				if quotes: 
					cmd += StrtoHx(bytes(part[0], "utf-8").decode("unicode_escape")) #Process escape codes
				cmdstr = part[2]
				quotes = not quotes


		print(cmd)

	

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

