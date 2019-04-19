
#from cmath import *
#from math import *
import math
import cmath
import random

def graph(arr):
	n=len(arr)
	print("-"*(n+2))
	for h in range(10,-10-1,-1):
		print("|", end="")
		for i in range(0,n):
			v=round(arr[i])
			if (h==10 and v>10): print("!", end="")
			elif (h==-10 and v<-10): print("!", end="")
			elif (v==h): print("#", end="")
			else: print("-" if h==0 else " ", end="")
		print("|")
	print("-"*(n+2))

def fourier(arr):
	bands=[]
	n=len(arr)
	m=math.ceil((n+1)/2)
	for f in range(0,m):
		bands.append(0)
		for i in range(0,n):
			bands[f]+=arr[i]*cmath.exp((f*i/n)*math.tau*1j)
		#magnatude * normalization * last_element_adjustment
		bands[f]=abs(bands[f]) * (2/n) * (1,0.5)[f==m-1]
	return bands

arr=[]
n=300
for i in range(0,n):
	v=4*math.sin(10*i*math.tau/n) + 3*math.sin(3*i*math.tau/n) + 2*math.sin(5*i*math.tau/n)
	#v=3*math.sin(13*i*math.tau/n) + 3*math.sin(23*i*math.tau/n)
	#v=2*math.cos(n/2*i*math.tau/n)
	arr.append(v)

graph(arr)
bands=fourier(arr)
print()
graph(bands)
print()

m=math.ceil((n+1)/2)
arr2=[0]*n
for f in range(0,m):
	for i in range(0,n):
		arr2[i]+=bands[f]*(2*(f*i%n)/n-1)#bands[f]*math.sin(f*i*math.tau/n)
graph(arr2)
