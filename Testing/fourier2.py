
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

def graphcpx(arr):
	n=len(arr)
	print("-"*(n+2))
	for h in range(10,-10-1,-1):
		print("|", end="")
		for i in range(0,n):
			v=[round(abs(arr[i])), round(cmath.phase(arr[i])/math.tau*10)]
			c=0
			if (v[0]==h): c+=1
			if (v[1]==h): c+=2
			c=(0, '#', '*', '@')[c]
			if (h==10 and v[0]>10): print("!", end="")
			elif (h==-10 and v[0]<-10): print("!", end="")
			elif (c): print(c, end="")
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
			bands[f]+=arr[i]*cmath.exp((0.25-f*i/n)*math.tau*1j)
		#magnatude * normalization * last_element_adjustment
		bands[f]=bands[f] * (2/n) * (1,0.5)[f==m-1] * (1,0.5)[f==0]
	return bands

arr=[]
n=300
for i in range(0,n):
	#v=1*math.sin(0.5*i*math.tau/n+0.2*math.tau)
	#v=2.5*math.sin(10*i*math.tau/n+0.25*math.tau) + 3*math.sin(3*i*math.tau/n+0.6*math.tau) + 2*math.sin(5*i*math.tau/n+0.9*math.tau) + 6#+6*math.sin(0.5*i*math.tau/n+0.25*math.tau)
	v=4*((int((5*i/n*2)%2))-0.5)*2
	#v*=2*(n-i)/n
	#v=3*math.sin(13*i*math.tau/n) + 3*math.sin(23*i*math.tau/n)
	#v=2*math.cos(n/2*i*math.tau/n)
	arr.append(v)

graph(arr)
print()

d=4
arr3=[]
for s in range(0,d):
	#graph(arr[s*int(n/d):(s+1)*int((n+1)/d)])
	bands=fourier(arr[s*int(n/d):(s+1)*int((n+1)/d)])
	graphcpx(bands)
	print()
	
	m=len(bands)
	arr2=[0]*int(n/d)
	for f in range(0,m):
		for i in range(0,int(n/d)):
			arr2[i]+=abs(bands[f])*math.sin(f*((i/(n/d))*math.tau)) #+ cmath.phase(bands[f])) #
			#arr2[i]=5*math.cos(f*((i/(n/d))*math.tau) + s*f*math.tau + f/(m/2)*math.tau)
	
	arr3=arr3+arr2

graph(arr3)
print()

bands=fourier(arr3)
graphcpx(bands)
print()

'''
m=math.ceil((n+1)/2)
arr2=[0]*int(n/2)
for f in range(0,int(m/2)):
	for i in range(0,int(n/2)):
		arr2[i]+=abs(bands1[f])*math.cos(f*((i/(n/2))*math.tau) )#-cmath.phase(bands1[f]))
		#arr2[i]+=bands[f]*(2*(f*i%n)/n-1)

m=math.ceil((n+1)/2)
arr3=[0]*int(n/2)
for f in range(0,int(m/2)):
	for i in range(0,int(n/2)):
		arr3[i]+=abs(bands2[f])*math.cos(f*((i/(n/2))*math.tau) + f*math.tau)#-cmath.phase(bands2[f]))
		#arr2[i]+=bands[f]*(2*(f*i%n)/n-1)

arr4=arr2+arr3

graph(arr4)
'''
