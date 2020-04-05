#!/bin/python

import os
from PIL import Image

F=None
FFrames="Frames"
FFrameData="FrameData.bin"
FFrameManifest="FrameManifest.txt"
Displays=[
	#Name, Img Src Rectangle (x1, y1, x2, y2), Mirror Horizontally
	{"Name":"RightEye", "Rect":{"x1":0, "y1":0, "x2":15, "y2":7}, "Mirror":"YX"},
	{"Name":"RightMouth", "Rect":{"x1":16, "y1":0, "x2":47, "y2":7}, "Mirror":""},
	{"Name":"RightNose", "Rect":{"x1":48, "y1":0, "x2":55, "y2":7}, "Mirror":"XY"},
	{"Name":"LeftNose", "Rect":{"x1":56, "y1":0, "x2":63, "y2":7}, "Mirror":""},
	{"Name":"LeftMouth", "Rect":{"x1":64, "y1":0, "x2":95, "y2":7}, "Mirror":"X"},
	{"Name":"LeftEye", "Rect":{"x1":96, "y1":0, "x2":111, "y2":7}, "Mirror":""},
]

try: f = open(FFrameData, 'wb')
except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)
FFrameData=f

try: f = open(FFrameManifest, 'w')
except: print("Error opening file \"{0}\"".format(FFrameManifest)); exit(-1)
FFrameManifest=f


def Hx(val, length):
	return val.to_bytes(length, byteorder="little")

def StrtoHx(string):
	return bytes(string, "utf8")

# ImgData=[]
# ImgDataTmp=b""


def ParseImageData(img, imgrange, mirrorx, mirrory):
	data = [ ]
	for y in range(imgrange["y1"],imgrange["y2"]+1):
		if mirrory:
			y=((imgrange["y2"])-imgrange["y1"])-(y-imgrange["y1"])+imgrange["y1"]
		bit=7
		byte=0x00
		data += [ b"" ]
		for x in range(imgrange["x1"],imgrange["x2"]+1):
			#read the red channel at each pixel and set on/off
			c = 1 if img.getpixel((x,y))[0]>127 else 0
			byte |= c<<bit if not mirrorx else c<<(7-bit)
			bit-=1
			#byte filled; save it and reset bits
			if bit==-1:
				if not mirrorx:
					data[-1] += Hx(byte,1)
				else:
					data[-1] = Hx(byte,1) + data[-1]
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
	#Calculate the width and height
	d["w"] = d["Rect"]["x2"] - d["Rect"]["x1"] + 1 #Width in bits
	d["W"] = int(d["w"]/8)                         #Width in bytes
	d["h"] = d["Rect"]["y2"] - d["Rect"]["y1"] + 1 #Height in bits
	d["H"] = int(d["h"]/8)                         #Height in bytes

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

Index=len(Displays) #Index for every FrameEntry
Last=[None]*len(Displays) #Reference to the last entry made

def ParseImage(path, expr, stage):
	global Frames
	global Index
	global FrameData

	frameData = []

	#Open image
	try: img = Image.open(path)
	except: print("Error opening file \"{0}\"".format(path)); exit(-1)

	if (img.size[1] % 8 != 0 or img.size[0] % 8 != 0):
		print("Error: Image improperly sized")
		exit(0)

	#parse the frame number out
	imgnum = int(path.split("frame")[1].split(".bmp")[0])

	frameData = []
	l=0
	for d in Displays:
		#Add a new frame for the current display
		newframe = ParseImageData(img, d["Rect"], d["Mirror"].find("X")!=-1, d["Mirror"].find("Y")!=-1)
		
		name = expr + "_" + stage + "_" + str(imgnum) + "_" + d["Name"]
		
		#Check to see if the frame already exists
		frameidx = ExistsInFrames(newframe)
		if frameidx == -1:
			#The frame doesn't so add it to the list
			frameidx = len(Frames)
			Frames += [ newframe ]

		#Check if the last frame (of this display) is still the same
		if Last[l] == None or Last[l]["FrameIdx"] != frameidx:
			#Current frame is different
			if Last[l] != None:
				#Set the previous frame to point to this one
				Last[l]["NextIdx"] = Index
			#Add new frame
			frameData += [{"Name":name, "FrameIdx":frameidx, "Delay":40, "NextIdx":-1, "Index":Index}]
			Last[l] = frameData[-1] #Update this one as the new previous frame
			Index+=1
		else:
			Last[l]["Delay"] += 40
			#Increment the delay of the previous frame
		
		l+=1

	# for f in newframes:
	# 	print(f)
	# for f in frameData:
	# 	print(f)
	# exit()	
	return frameData

ManifestStr=""

def Parse():
	global FrameData
	global Last
	global ManifestStr

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

		#Parse the frames for each section

		Last=[None]*len(Displays) #Reset Last frames; start of each section must be recorded
		for f in start:
			path = FFrames+"/"+expr+"/start/"+f
			startFrameData += ParseImage(path, expr, "start")
		lastofstart = Last
		
		Last=[None]*len(Displays) #Reset Last frames; start of each section must be recorded
		for f in loop:
			path = FFrames+"/"+expr+"/loop/"+f
			loopFrameData += ParseImage(path, expr, "loop")
		lastofloop = Last

		Last=[None]*len(Displays) #Reset Last frames so that the last of loop sequence can loop back 
		for f in end:
			path = FFrames+"/"+expr+"/end/"+f
			endFrameData += ParseImage(path, expr, "end")
		lastofend = Last

		
		#Set the Next Indexes
		for fi in range(0, len(Displays)):
			lastofstart[fi]["NextIdx"] = loopFrameData[fi]["Index"]
			lastofloop[fi]["NextIdx"] = loopFrameData[fi]["Index"]
			# print(loopFrameData[fi]["Index"])
			lastofend[fi]["NextIdx"] = fi
		

		FrameData += startFrameData
		FrameData += loopFrameData
		FrameData += endFrameData
		
		ManifestStr += expr
		ManifestStr += "\n  Start: "
		for i in range(len(Displays)):
			ManifestStr += Displays[i]["Name"] + "={:<5d}".format(startFrameData[i]["Index"]) + " "
		ManifestStr += "\n  Loop:  "
		for i in range(len(Displays)):
			ManifestStr += Displays[i]["Name"] + "={:<5d}".format(loopFrameData[i]["Index"]) + " "
		ManifestStr += "\n  End:   "
		for i in range(len(Displays)):
			ManifestStr += Displays[i]["Name"] + "={:<5d}".format(endFrameData[i]["Index"]) + " "
		ManifestStr += "\n\n"

		#Print data
		i=0
		for f in FrameData:
			print(str(i) + "(" + str(f["Index"]) + ") " + f["Name"] + " [" + str(f["FrameIdx"]) + "] " + str(f["Delay"]) + "ms -> [" + str(f["NextIdx"]) + "] ", end="")
			print(FrameData[f["NextIdx"]]["Name"] )
			i+=1


Parse()

print()
print("Number of Frames: " +str(len(Frames)))
print("Number of Entries: " +str(len(FrameData)))
print()


HeaderStr=b""
FrameStr=b""

for frame in FrameData:
	#Data+=hx(0,1) #TypeMap[frame["Type"]]
	frameNext = frame["NextIdx"]
	#= Size of header * bytes in each header + \
	#   the size of frames before current one
	FrameOffset=len(FrameData)*(2+2+2) + sum(list(map(lambda f: len(f)*len(f[0]), Frames[:frame["FrameIdx"]])))

	frameDelay = frame["Delay"] if frame["Delay"] else 0xFFFF #frame["FrameIdx"]
	if frameDelay > 0xFFFF:
		print("Error: Delay time exceeds maximum allowed value")
		print("Please Implement fix")
		exit(-1)
	HeaderStr+=Hx(FrameOffset, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameNext, 2) #2 bytes  16 bits
	HeaderStr+=Hx(frameDelay, 2) #2 byte  16 bits
	#HeaderStr+=Hx(0, 1) #padding to 4-byte boundary  =4-(2+1) =1
	
#Add in all the frames
for imgdat in Frames:
	for imgline in imgdat:
		FrameStr+=imgline
	

print("Header: " + str(len(HeaderStr)) + "B")
print("Frames: " + str(len(FrameStr)) + "B")

print("Writing Output Files")
FFrameData.write(HeaderStr+FrameStr)
FFrameData.close()

FFrameManifest.write(ManifestStr)
FFrameManifest.close()


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
