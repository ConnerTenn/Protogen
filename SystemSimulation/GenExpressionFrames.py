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
	

Name=""
Type="Default"
Delay=0
Next=""
Continue=True
while Continue:
	line=fframes.readline()
	#print("\"{0}\"".format(line[:-1]))
	if len(line):
		line=line.split("\\")[0].rstrip() #Remove comments
		
		keyval=line.split("=")
		#if len(tag)==2 and len(tag[0]) and len(tag[1]):
		if keyval[0]=="Name": Name = (keyval[1] if len(keyval)==2 else "")
		if keyval[0]=="Type": Type = (keyval[1] if len(keyval)==2 else "")
		if keyval[0]=="Delay": Delay = (keyval[1] if len(keyval)==2 else "")
		if keyval[0]=="Next": Next = (keyval[1] if len(keyval)==2 else "")

		if line=="":
			EndImageData()
		elif line[0]=="." or line[0]=="#":
			ParseImageData(line)

		if line==";":
			EndImageData()
			print("Name:{0}\nType:{1}\nDelay:{2}\nNext:{3}\nImgData:\n".format(Name,Type,Delay,Next),end='')
			for img in ImgData:
				print(img)
			print()
			Name=""
			Type="Default"
			Delay=0
			Next=""
			ImgData=[]
			pass
			# Finish
	else:
		Continue=False



fframes.close()
fexpr.close()
# fExpressionDataH.close()
# fExpressionDataCpp.close()
# fFrameDataH.close()
# fFrameDataCpp.close()