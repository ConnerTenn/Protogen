#!/bin/python

import os
from PIL import Image

F=None
FFrames="Frames"
FFrameData="FrameData.bin"
Displays=[
	#Name, Src Rectangle (x1, y1, x2, y2)
	{"Name":"LeftEye", "Rect":{"x1":0, "y1":0, "x2":15, "y2":7}},
	{"Name":"LeftMouth", "Rect":{"x1":16, "y1":0, "x2":47, "y2":7}},
	{"Name":"LeftNose", "Rect":{"x1":48, "y1":0, "x2":55, "y2":7}},
	{"Name":"RightNose", "Rect":{"x1":56, "y1":0, "x2":63, "y2":7}},
	{"Name":"RightMouth", "Rect":{"x1":64, "y1":0, "x2":95, "y2":7}},
	{"Name":"RightEye", "Rect":{"x1":96, "y1":0, "x2":111, "y2":7}},
]

try: f = open(FFrameData, 'wb')
except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)
FFrameData=f


def Hx(val, length):
	return val.to_bytes(length, byteorder="little")

def StrtoHx(string):
	return bytes(string, "utf8")

# ImgData=[]
# ImgDataTmp=b""


def ParseImageData(img, imgrange):
	data = [ ]
	for y in range(imgrange["y1"],imgrange["y2"]+1):
		bit=7
		byte=0x00
		data += [ b"" ]
		for x in range(imgrange["x1"],imgrange["x2"]+1):
			#read the red channel at each pixel and set on/off
			c = 1 if img.getpixel((x,y))[0]>127 else 0
			byte |= c<<bit
			bit-=1
			#byte filled; save it and reset bits
			if bit==-1:
				data[-1] += Hx(byte,1)
				bit=7
				byte = 0x00
	#return the data for the entire display
	return data

#verify displays
for d in Displays:
	if d["Name"].find(" ") != -1 or d["Name"].find("_") != -1:
		print("Spaces and Underscores not allowed in name")
		exit(-1)
	if ((d["Rect"]["x2"] - d["Rect"]["x1"] + 1) % 8 != 0) or ((d["Rect"]["y2"] - d["Rect"]["y1"] + 1) % 8 != 0):
		print("Invalid Display Rect")
		exit(-1)
	d["w"] = d["Rect"]["x2"] - d["Rect"]["x1"] + 1
	d["W"] = int(d["w"]/8)
	d["h"] = d["Rect"]["y2"] - d["Rect"]["y1"] + 1
	d["H"] = int(d["h"]/8)

# for d in Displays:
# 	print(d)

Frames = []
FrameData = []
i = 0
for d in Displays:
	Frames += [ [ b'\x00' * d["W"] ] * d["h"] ]
	FrameData += [{"Name":"Null_"+d["Name"], "FrameIdx":i, "Delay":0, "NextIdx":i, "Index":i}]
	i+=1
del i

# for f in Frames:
# 	print(f)

def ExistsInFrames(frame):
	global Frames
	i = 0
	for f in Frames:
		if f==frame:
		    return i
		i+=1
	return -1

Index=len(Displays)
Last=[None]*len(Displays)
def ParseImage(path, expr, stage):
	global Frames
	global Index
	global FrameData

	frameData = []

	try: img = Image.open(path)
	except: print("Error opening file \"{0}\"".format(path)); exit(-1)

	if (img.size[1] % 8 != 0 or img.size[0] % 8 != 0):
		print("Error: Image improperly sized")
		exit(0)

	imgnum = int(path.split("frame")[1].split(".bmp")[0])

	newframes = []
	frameData = []
	l=0
	for d in Displays:
		newframes += [ ParseImageData(img, d["Rect"]) ]
		
		name = expr + "_" + stage + "_" + str(imgnum) + "_" + d["Name"]
		
		frameidx = ExistsInFrames(newframes[-1])
		if frameidx == -1:
			frameidx = len(Frames)
			Frames += [ newframes[-1] ]

		if Last[l] == None or Last[l]["FrameIdx"] != frameidx:
			if Last[l] != None:
				Last[l]["NextIdx"] = Index
			frameData += [{"Name":name, "FrameIdx":frameidx, "Delay":40, "NextIdx":-1, "Index":Index}]
			Last[l] = frameData[-1] 
			Index+=1
		else:
			Last[l]["Delay"] += 40
		
		l+=1

	return frameData
	# for f in newframes:
	# 	print(f)
	# for f in frameData:
	# 	print(f)
	# exit()

	
	
	
	return frameData

def Parse():
	global FrameData
	global Last

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
		
		startFrameData = []
		loopFrameData = []
		endFrameData = []


		for f in start:
			path = FFrames+"/"+expr+"/start/"+f
			startFrameData += ParseImage(path, expr, "start")
		lastofstart = Last
		
		for f in loop:
			path = FFrames+"/"+expr+"/loop/"+f
			loopFrameData += ParseImage(path, expr, "loop")
		lastofloop = Last

		for f in end:
			path = FFrames+"/"+expr+"/end/"+f
			endFrameData += ParseImage(path, expr, "end")
		lastofend = Last

		for fi in range(0, len(Displays)):
			lastofstart[fi]["NextIdx"] = loopFrameData[fi]["Index"]
			lastofloop[fi]["NextIdx"] = loopFrameData[fi]["Index"]
			lastofend[fi]["NextIdx"] = fi
		
		
		# startFrameData[-1]["NextIdx"] = loopFrameData[0]["Index"]
		# loopFrameData[-1]["NextIdx"] = loopFrameData[0]["Index"]
		# endFrameData[-1]["NextIdx"] = 0 #Null

		FrameData += startFrameData
		FrameData += loopFrameData
		FrameData += endFrameData
		
		# last = -1
		# tmpdata = []
		# backmod = 0
		# for f in FrameData:
		# 	if f["FrameIdx"] == last:
		# 		tmpdata[-1]["Delay"] += 40
		# 		#tmpdata[-1]["NextIdx"] -= 1
		# 		backmod += 1
		# 	else:
		# 		f["NextIdx"] =  f["NextIdx"] - backmod if f["NextIdx"] else 0
		# 		tmpdata += [ f ]
		# 		last = f["FrameIdx"]
		# FrameData = tmpdata

		i=0
		for f in FrameData:
			print(str(i) + " " + f["Name"] + " [" + str(f["FrameIdx"]) + "] " + str(f["Delay"]) + "ms -> [" + str(f["NextIdx"]) + "] ", end="")
			print(FrameData[f["NextIdx"]]["Name"] )
			i+=1
			#FrameData[f]["NextIdx"] = ""

			
		

# FrameData=Parse(FFrames, { "Name":"", "Mask":"", "Pass":"", "Delay":"", "NextIdx":"" })

Parse()

print()
print("Number of Frames: " +str(len(Frames)))
print("Number of Entries: " +str(len(FrameData)))
print()


HeaderStr=b""
FrameStr=b""
FrameOffset=len(FrameData)*(2+2+2) #Size of header * bytes in each header
for frame in FrameData:
	#Data+=hx(0,1) #TypeMap[frame["Type"]]
	frameNext = int(frame["FrameIdx"])
	frameDelay = frame["Delay"] if frame["Delay"] else 0xFFFF #frame["FrameIdx"]
	if frameDelay > 0xFFFF:
		print("Error: Delay time exceeds maximum allowed value")
		print("Please Implement fix")
		exit(-1)
	HeaderStr+=Hx(FrameOffset, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameNext, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameDelay, 2) #2 byte  16 bits
	#HeaderStr+=Hx(0, 1) #padding to 4-byte boundary  =4-(2+1) =1
for imgdat in Frames:
    for imgline in imgdat:
        FrameStr+=imgline
        FrameOffset+=len(imgline)

	

print("Header: " + str(len(HeaderStr)) + "B")
print("Frames: " + str(len(FrameStr)) + "B")

print("Writing Data File")
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
		FrameOffset [2]
		FrameNext [2]
		FrameDelay [2]
	} [6]
	...

ImageData
	BinaryData...


"""
