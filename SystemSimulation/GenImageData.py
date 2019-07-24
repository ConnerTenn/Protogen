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