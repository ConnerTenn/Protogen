#!/bin/python



f=None
fframes="Frames.dat"
fexpr="Expressions.dat"
fExpressionDataH=None #"ExpressionData.h"
fExpressionDataCpp=None #"ExpressionData.cpp"
fFrameDataH=None #"FrameData.h"
fFrameDataCpp=None #"FrameData.cpp"
try: f = open(fframes, 'r')
except: print("Error opening file \"{0}\"".format(fframes)); exit(-1)
fframes=f
try: f = open(fexpr, 'r')
except: print("Error opening file \"{0}\"".format(fexpr)); exit(-1)
fexpr=f

try: f = open(fExpressionDataH, 'w')
except: print("Error opening file \"{0}\"".format(fExpressionDataH)); exit(-1)
fExpressionDataH=f
try: f = open(fExpressionDataCpp, 'w')
except: print("Error opening file \"{0}\"".format(fExpressionDataCpp)); exit(-1)
fFrameDataH=f
try: f = open(fFrameDataH, 'w')
except: print("Error opening file \"{0}\"".format(fFrameDataH)); exit(-1)
fFrameDataCpp=f
try: f = open(fFrameDataCpp, 'w')
except: print("Error opening file \"{0}\"".format(fFrameDataCpp)); exit(-1)
fFrameDataCpp=f

exit(0)

buff=""
for line in file:
	line.replace(" ", "")
	if line=="\n":
		if (len(buff)>0): print("\""+buff+"\"\n")
		else: print()
		buff=""
	elif line[0]==';':
		print(line[1:],end='')
	else:
		byte=0x00
		bit=7
		for chr in line:

			c= (1 if chr=='#' else ( 0 if chr=='.' else -1))
			if c>=0:
				
				byte|=c<<bit

				bit-=1
				if bit==-1: 
					bit=7
					buff+="\\x{0:02X}".format(byte)
					byte=0
					
if (len(buff)>0): print("\""+buff+"\"\n")	

file.close()