#!/bin/python



F=None
Fframes="Frames.dat"
Fexpr="Expressions.dat"
FExpressionDataH=None #"ExpressionData.h"
FExpressionDataCpp=None #"ExpressionData.cpp"
FFrameDataH=None #"FrameData.h"
FFrameDataCpp=None #"FrameData.cpp"
try: F = open(Fframes, 'r')
except: print("Error opening file \"{0}\"".format(Fframes)); exit(-1)
Fframes=F
try: F = open(Fexpr, 'r')
except: print("Error opening file \"{0}\"".format(Fexpr)); exit(-1)
Fexpr=F

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

ImgData=[]
ImgDataTmp=""

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
			ImgDataTmp+="\\x{0:02X}".format(byte)
			byte=0

def EndImageData():
	global ImgDataTmp, ImgData
	if len(ImgDataTmp):
		ImgData+=[ImgDataTmp]
		ImgDataTmp=""


def Parse(f, fields):
	global ImgData
	cont=True
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
				for field in fields:
					print("{0}={1}".format(field,fields[field]))
					fields[field]=""
				if len(ImgData): print("ImgData:")
				for img in ImgData:
					print(img)
				ImgData=[]
				print()
		else:
			cont=False

Parse(Fframes, { "Name":"", "Type":"", "Delay":"", "Next":"" })
Parse(Fexpr, { "Name":"", "EL":"", "ER":"", "ML":"", "MR":"", "MC":"", "N":"", "B":"" })


Fframes.close()
Fexpr.close()
# fExpressionDataH.close()
# fExpressionDataCpp.close()
# fFrameDataH.close()
# fFrameDataCpp.close()