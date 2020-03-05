#!/bin/python

import os
from PIL import Image

F=None
FFrames="Frames"
FFrameData="FrameData.bin"



try: f = open(FFrameData, 'wb')
except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)
FFrameData=f


def Hx(val, length):
	return val.to_bytes(length, byteorder="little")

def StrtoHx(string):
	return bytes(string, "utf8")

# ImgData=[]
# ImgDataTmp=b""

'''
def ParseImageData(line):
	global ImgDataTmp, ImgData
	byte=0x00
	bit=7
	for ch in line:
		c = (1 if ch=='#' else 0)
		byte|=c<<bit
		bit-=1
		if bit==-1: #Reached end of bit
			ImgDataTmp+=Hx(byte,1) #Convert byte to byte string and append #"\\x{0:02X}".format(byte)
			bit=7
			byte=0

def EndImageData():
	global ImgDataTmp, ImgData
	if len(ImgDataTmp):
		ImgData+=[ImgDataTmp]
		ImgDataTmp=b""

def Parse(f, fields):
	global ImgData
	cont=True
	outData={}
	index=0
	while cont:
		line=f.readline()

		if len(line):
			line=line.split("\\")[0].rstrip() #Remove Comments and whitespace
			
			keyval=line.split("=") #Split keys and values
			
			for field in fields: #Assign values to field if it exists
				if keyval[0]==field: fields[field] = (keyval[1] if len(keyval)==2 else "")
			
			if line=="":
				EndImageData() #end image parsing
			elif line[0]=="." or line[0]=="#":
				ParseImageData(line)

			if line==";":
				EndImageData() #end image parsing
				dat={}
				for field in fields: 
					#print("{0}={1}".format(field,fields[field]))
					dat[field]=fields[field]
					fields[field]="" #reset fields back to default
				#if len(ImgData): print("ImgData:")
				dat["ImgData"]=[] #image data array
				for img in ImgData: #append each image into the array
					dat["ImgData"]+=[img]
				dat["Index"]=index
				#	print(img)
				outData[dat["Name"]]=dat #Label this data with the name value

				ImgData=[] #reset image data
				index+=1
				#print(dat)
				#print()
		else:
			cont=False
	#print(outData)
	return outData
'''

def ParseImageData(img, y, xrange):
	data = b""
	bit=7
	byte=0x00
	for x in range(xrange[0],xrange[1]):
		#read the red channel at each pixel and set on/off
		c = 1 if img.getpixel((x,y))[0]>127 else 0
		byte |= c<<bit
		bit-=1
		#byte filled; save it and reset bits
		if bit==-1:
			data += Hx(byte,1)
			bit=7
			byte = 0x00
	#return the data for the entire display
	return data

FrameData = {}
index = 0
def ParseImage(path, expr, stage):

	try: img = Image.open(path)
	except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)

	if (img.size[1] != 8 or img.size[0] != (8*4 + 8*2 + 8)*2):
		print("Error: Image improperly sized")
		exit(0)
	
	framenum = int(path.split("frame")[1].split(".bmp")[0])
	name = expr + "_" + stage + "_" + str(framenum)
	print(name)

	FrameData[name+"_EyeLeft"] = {"ImgData":[]}
	FrameData[name+"_MouthLeft"] = {"ImgData":[]}
	FrameData[name+"_NoseLeft"] = {"ImgData":[]}
	FrameData[name+"_NoseRight"] = {"ImgData":[]}
	FrameData[name+"_MouthRight"] = {"ImgData":[]}
	FrameData[name+"_EyeRight"] = {"ImgData":[]}

	for y in range(0,8):
		FrameData[name+"_EyeLeft"]["ImgData"] += [ b"" ]
		FrameData[name+"_EyeLeft"]["ImgData"][-1] += ParseImageData(img, y, (0,16))

		FrameData[name+"_MouthLeft"]["ImgData"] += [ b"" ]
		FrameData[name+"_MouthLeft"]["ImgData"][-1] += ParseImageData(img, y, (16,48))

		FrameData[name+"_NoseLeft"]["ImgData"] += [ b"" ]
		FrameData[name+"_NoseLeft"]["ImgData"][-1] += ParseImageData(img, y, (48,56))

		FrameData[name+"_NoseRight"]["ImgData"] += [ b"" ]
		FrameData[name+"_NoseRight"]["ImgData"][-1] += ParseImageData(img, y, (48,56))

		FrameData[name+"_MouthRight"]["ImgData"] += [ b"" ]
		FrameData[name+"_MouthRight"]["ImgData"][-1] += ParseImageData(img, y, (16,48))

		FrameData[name+"_EyeRight"]["ImgData"] += [ b"" ]
		FrameData[name+"_EyeRight"]["ImgData"][-1] += ParseImageData(img, y, (0,16))


	print(FrameData[name+"_MouthLeft"]["ImgData"])

def Parse():
	Expressions = os.listdir(FFrames)
	for expr in Expressions:
		start = os.listdir(FFrames + "/" + expr + "/start")
		loop = os.listdir(FFrames + "/" + expr + "/loop")
		end = os.listdir(FFrames + "/" + expr + "/end")
		# start = list(map(lambda s: FFrames+"/"+expr+"/start/"+s, start))
		# loop = list(map(lambda s: FFrames+"/"+expr+"/loop/"+s, loop))
		# end = list(map(lambda s: FFrames+"/"+expr+"/end/"+s, end))
		start.sort()
		loop.sort()
		end.sort()
		for f in start:
			path = FFrames+"/"+expr+"/start/"+f
			ParseImage(path, expr, "start")

		for f in loop:
			path = FFrames+"/"+expr+"/loop/"+f
			ParseImage(path, expr, "loop")

		for f in end:
			path = FFrames+"/"+expr+"/end/"+f
			ParseImage(path, expr, "end")
			
		

# FrameData=Parse(FFrames, { "Name":"", "Mask":"", "Pass":"", "Delay":"", "Next":"" })

Parse()

print("Number of frames: " +str(len(FrameData)) + "\n")

HeaderStr=b""
FrameStr=b""
FrameOffset=len(FrameData)*(2+2+2) #Size of header * bytes in each header
for name in FrameData:
	frame=FrameData[name]
	#Data+=hx(0,1) #TypeMap[frame["Type"]]
	frameNext = int(FrameData[frame["Next"]]["Index"]) if len(frame["Next"]) else frame["Index"]
	frameDelay = int(frame["Delay"]) if len(frame["Delay"]) else 0xFF #frame["Index"]
	HeaderStr+=Hx(FrameOffset, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameNext, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameDelay, 2) #2 byte  16 bits
	#HeaderStr+=Hx(0, 1) #padding to 4-byte boundary  =4-(2+1) =1
	for imgDat in frame["ImgData"]:
		FrameStr+=imgDat
		FrameOffset+=len(imgDat)

FFrameData.write(HeaderStr+FrameStr)


FFrameData.close()


"""

Label [Bytes]
Label 
{ 
	Block [Bytes]
} [Bytes]

FrameOffset stores the offset from the beginning of the datablock

Datablock Format:

Header
	Frame
	{
		FrameOffset [4]
		FrameNext [2]
		FrameDelay [1]
		Padding [1]
	} [8]
	...

ImageData
	BinaryData...


"""
