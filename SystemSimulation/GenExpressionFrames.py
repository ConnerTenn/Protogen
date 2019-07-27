#!/bin/python



F=None
Fframes="Frames.dat"
Fexpr="Expressions.dat"
FFrameData="FrameData.bin"
# FExpressionDataH=None #"ExpressionData.h"
# FExpressionDataCpp=None #"ExpressionData.cpp"
# FFrameDataH=None #"FrameData.h"
# FFrameDataCpp=None #"FrameData.cpp"
try: F = open(Fframes, 'r')
except: print("Error opening file \"{0}\"".format(Fframes)); exit(-1)
Fframes=F
try: F = open(Fexpr, 'r')
except: print("Error opening file \"{0}\"".format(Fexpr)); exit(-1)
Fexpr=F

try: f = open(FFrameData, 'wb')
except: print("Error opening file \"{0}\"".format(FFrameData)); exit(-1)
FFrameData=f
# try: f = open(fExpressionDataH, 'w')
# except: print("Error opening file \"{0}\"".format(fExpressionDataH)); exit(-1)
# fExpressionDataH=f
# try: f = open(fExpressionDataCpp, 'w')
# except: print("Error opening file \"{0}\"".format(fExpressionDataCpp)); exit(-1)
# fFrameDataH=f
# try: f = open(fFrameDataH, 'w')
# except: print("Error opening file \"{0}\"".format(fFrameDataH)); exit(-1)
# fFrameDataCpp=f
# try: f = open(fFrameDataCpp, 'w')
# except: print("Error opening file \"{0}\"".format(fFrameDataCpp)); exit(-1)
# fFrameDataCpp=f

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

# def Hxstr(string):
# 	out=b''
# 	for ch in string:
# 		out+="\\x{0:02X}".format(ord(ch) & 0xFF)
# 	return out

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
		if bit==-1: 
			bit=7
			ImgDataTmp+=Hx(byte,1) #"\\x{0:02X}".format(byte)
			byte=0

def EndImageData():
	global ImgDataTmp, ImgData
	if len(ImgDataTmp):
		ImgData+=[ImgDataTmp]
		ImgDataTmp=b""


def Parse(f, fields):
	global ImgData
	cont=True
	outData=[]
	while cont:
		line=f.readline()

		if len(line):
			line=line.split("\\")[0].rstrip()
			
			keyval=line.split("=")
			
			for field in fields:
				if keyval[0]==field: fields[field] = (keyval[1] if len(keyval)==2 else "")
			
			if line=="":
				EndImageData()
			elif line[0]=="." or line[0]=="#":
				ParseImageData(line)

			if line==";":
				EndImageData()
				dat={}
				for field in fields:
					#print("{0}={1}".format(field,fields[field]))
					dat[field]=fields[field]
					fields[field]=""
				#if len(ImgData): print("ImgData:")
				dat["ImgData"]=[]
				for img in ImgData:
					dat["ImgData"]+=[img]
				#	print(img)
				ImgData=[]
				outData+=[dat]
				#print(dat)
				#print()
		else:
			cont=False
	#print(outData)
	return outData



#TypeMap={""}
DataStr=b""
HeaderStr=b""
FrameStr=b""
FrameData=Parse(Fframes, { "Name":"", "Type":"", "Delay":"", "Next":"" })
Header=[]
FrameOffset=len(FrameData)*8
for frame in FrameData:
	#Data+=hx(0,1) #TypeMap[frame["Type"]]
	#Data+=hx(int(frame["Delay"]), 1)
	#Data+=hx(int(frame["Next"]), 2)
	HeaderStr+=Hx(FrameOffset, 8)
	for imgDat in frame["ImgData"]:
		FrameStr+=imgDat
		FrameOffset+=len(imgDat)
#print(HeaderStr)
#print(FrameStr)
DataStr=HeaderStr+FrameStr

print(DataStr)
FFrameData.write(DataStr)


Parse(Fexpr, { "Name":"", "EL":"", "ER":"", "ML":"", "MR":"", "MC":"", "N":"", "B":"" })


Fframes.close()
Fexpr.close()
# fExpressionDataH.close()
# fExpressionDataCpp.close()
# fFrameDataH.close()
# fFrameDataCpp.close()