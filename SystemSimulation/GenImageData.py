#!/bin/python


import sys



if len(sys.argv) > 2:
	print("Usage: ./GenImageData.py [Optional Src File]\n")
	exit(-1)



file=None
if len(sys.argv) == 2:
	try:
		file = open(sys.argv[1], 'r')
	except:
		print("Error opening file")
		exit(-1)
else:
	file=sys.stdin


print("\"",end='')
lineflop=False; begin=False
for line in file:
	line.replace(" ", "")
	if line=="\n":
		if not lineflop and begin: 
			lineflop=True
	else:
		if lineflop: print("\"\n\"",end=''); lineflop=False
		begin=True
		byte=0x00
		bit=7
		for chr in line:

			c= (1 if chr=='#' else ( 0 if chr=='.' else -1))
			if c>=0:
				
				byte|=c<<bit

				bit-=1
				if bit==-1: 
					bit=7
					print("\\{0}".format(hex(byte)[1:]),end='')
					byte=0

print("\"")
		

file.close()