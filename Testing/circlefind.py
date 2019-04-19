
import random
import math

def printarr(arr):
	for y in range(0, len(arr)):
		for x in range(0, len(arr[y])):
			print(('-','+','@')[arr[y][x]],end="")
		print()
def printcircarr(arr):
	for y in range(0, len(arr)):
		for x in range(0, len(arr[y])):
			print(hex(arr[y][x][0]%16)[2:] if arr[y][x][1]==1 else ('-','+')[arr[y][x][0]>0],end="")
		print()

Width=300
Height=50
Arr=[[0 for i in range(0,Width)] for i in range(0,Height)]

for i in range(0, 20):
	posx=int(1000*random.random()%Width)
	posy=int(1000*random.random()%Height)
	s=int(1000*random.random()%(min(Width,Height)/2)+3)
	#print(str(posx)+' '+str(posy)+' '+str(s))
	for y in range(max(posy-int(s/2),0), min(posy+int(s/2)+1,Height)):
		for x in range(max(posx-int(s/2),0), min(posx+int(s/2)+1,Width)):
			Arr[y][x] = Arr[y][x] | (((posx-x)**2 + (posy-y)**2) <= (s/2)**2)

printarr(Arr)
print()

'''CircArr=[[[0,0] for i in range(0,Width)] for i in range(0,Height)]
for y in range(0, len(Arr)):
	s=0
	for x in range(0, len(Arr[y])):
		if Arr[y][x] and x!=len(Arr[y])-1:
			s+=1
		else:
			a=CircArr[y][x-math.ceil(s/2)]
			b=CircArr[y-1][x-math.ceil(s/2)]
			a[1]=1
			if (a[0]>=b[0]+1): b[1]=0
			if (a[0]<b[0]+1): a[1]=0
			a[0]=min(a[0],b[0]+1)
			s=0

		CircArr[y][x]=[s,0]
'''
#Solve as center finding algorithm, separate over rows and columns
CircArr=[[[0,0] for i in range(0,Width)] for i in range(0,Height)]
for y in range(0, Height):
	s=0; a=0
	
	for x in range(0, Width):
		r=0
		if Arr[y][x]: a=1
		elif a: r=1; a=0
		
		if r or x==Width-1:
			CircArr[y][x-math.ceil(s/2)][1]=1
		
		if a: s+=1
		else: s=0
			
		CircArr[y][x]=[s,0]
		

printcircarr(CircArr)
print()

CircArr2=[[[0,0] for i in range(0,Width)] for i in range(0,Height)]
for x in range(0, Width):
	s=0; a=0
	
	for y in range(0, Height):
		r=0
		if Arr[y][x]: a=1
		elif a: r=1; a=0
		
		if r or y==Height-1:
			CircArr2[y-math.ceil(s/2)][x][1] = 1 #& CircArr[y-math.ceil(s/2)][x][1]
			
		if a: s+=1
		else: s=0
			
		CircArr2[y][x]=[s,0]

printcircarr(CircArr2)
print()

CircArr3=[[[0,0] for i in range(0,Width)] for i in range(0,Height)]
for y in range(0, Height):	
	for x in range(0, Width):
		a=CircArr[y][x]
		b=CircArr2[y][x]
		c=CircArr3[y][x]=[ min(a[0],b[0]), a[1]&b[1] ]
		
		#if a[1]&b[1]:
		#for y in range(max(x-int(s/2),0), min(y+int(s/2)+1,Height)):
		#	for x in range(max(y-int(s/2),0), min(x+int(s/2)+1,Width)):
		#		None

printcircarr(CircArr3)
print()
