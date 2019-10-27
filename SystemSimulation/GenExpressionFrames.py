#!/bin/python



F=None
FFrames="Frames.dat"
FExpr="Expressions.dat"
FFrameData="FrameData.bin"
FExpressionData="ExpressionData.bin"

try: F = open(FFrames, 'r')
except: print("Error opening file \"{0}\"".format(FFrames)); exit(-1)
FFrames=F
try: F = open(FExpr, 'r')
except: print("Error opening file \"{0}\"".format(FExpr)); exit(-1)
FExpr=F

try: f = open(FFrameData, 'wb')
except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)
FFrameData=f
try: f = open(FExpressionData, 'wb')
except: print("Error opening file \"{0}\"".format(FExpressionData)); exit(-1)
FExpressionData=f


def Hx(val, length):
	return val.to_bytes(length, byteorder="little")

# def Hx(val,length):
# 	out=b""
# 	while length:
# 		#out+="\\x{0:02X}".format(val & 0xFF)
# 		out+=chr(val & 0xFF)
# 		val=val>>8
# 		length-=1
# 	return out

# def HxtoStr(hx):
# 	out=""
# 	for val in hx:
# 		out+="\\x{0:02X}".format(val & 0xFF)
# 	return out

def StrtoHx(string):
	return bytes(string, "utf8")

ImgData=[]
ImgDataTmp=b""

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



FrameData=Parse(FFrames, { "Name":"", "Type":"", "Delay":"", "Next":"" })

DataStr=b""
HeaderStr=b""
FrameStr=b""
FrameOffset=len(FrameData)*(2+1+1+(4)+8)
for name in FrameData:
	frame=FrameData[name]
	#Data+=hx(0,1) #TypeMap[frame["Type"]]
	frameNext = int(FrameData[frame["Next"]]["Index"]) if len(frame["Next"]) else frame["Index"]
	frameDelay = int(frame["Delay"]) if len(frame["Delay"]) else frame["Index"]
	HeaderStr+=Hx(frameNext, 2)
	HeaderStr+=Hx(frameDelay, 1)
	HeaderStr+=Hx(0, 5) #padding  =8-(2+1)
	HeaderStr+=Hx(FrameOffset, 8)
	for imgDat in frame["ImgData"]:
		FrameStr+=imgDat
		FrameOffset+=len(imgDat)

DataStr=HeaderStr+FrameStr
FFrameData.write(DataStr)


ExpressionData=Parse(FExpr, { "Name":"", "EL":"", "ER":"", "ML":"", "MR":"", "MC":"", "N":"", "B":"" })

DataStr=b""
DataStr+=Hx(len(FrameData),2)
DataStr+=Hx(len(ExpressionData),2)
for name in FrameData:
	frame=FrameData[name]
	DataStr+=StrtoHx(name+'\x00'*(15-len(name))+"\0")
	DataStr+=Hx(frame["Index"],2)

for name in ExpressionData:
	expr=ExpressionData[name]
	DataStr+=StrtoHx(name+'\x00'*(15-len(name))+"\0")
	DataStr+=Hx(FrameData[expr["EL"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["ER"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["ML"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["MR"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["MC"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["N"]]["Index"], 2)
	DataStr+=Hx(FrameData[expr["B"]]["Index"], 2)
	DataStr+=Hx(0, 2)
FExpressionData.write(DataStr)

FFrames.close()
FExpr.close()
FFrameData.close()
FExpressionData.close()

