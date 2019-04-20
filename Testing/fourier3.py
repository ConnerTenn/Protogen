
import math
import cmath
import random

def ceildiv(a,b):
	return int(a/b) + (1 if a%b else 0)
def minmax(val,low,high):
	return min(max(val,low),high)

def graph(arr,height=10,scalex=1,scaley=1):
	n=len(arr)
	print("-"*(int(n/scalex)+2))
	for h in range(height,-height-1,-scaley):
		print("|", end="")
		for i in range(0,n,scalex):
			v=round(arr[i])
			if (h==10 and v>10): print("!", end="")
			elif (h==-10 and v<-10): print("!", end="")
			elif (v==h): print("#", end="")
			else: print("-" if h==0 else " ", end="")
		print("|")
	print("-"*(int(n/scalex)+2))

def graphcpx(arr,height=10,scalex=1,scaley=1):
	n=len(arr)
	print("-"*(int(n/scalex)+2))
	for h in range(height,-height-1,-scaley):
		print("|", end="")
		for i in range(0,n,scalex):
			v=[round(abs(arr[i])), round(cmath.phase(arr[i])/math.tau*height)]
			if v[0] < 10**-10: v[1]=0
			c=0
			if (v[0]==h): c+=1
			if (v[1]==h): c+=2
			c=(0, '#', '\'', '@')[c]
			if (h==10 and v[0]>10): print("!", end="")
			elif (h==-10 and v[0]<-10): print("!", end="")
			elif (c): print(c, end="")
			else: print("-" if h==0 else " ", end="")
		print("|")
	print("-"*(int(n/scalex)+2))

def fourier(arr):
	bands=[]
	n=len(arr)
	m=math.ceil((n+1)/2)
	for f in range(0,m):
		bands.append(0)
		for i in range(0,n):
			bands[f]+=arr[i]*cmath.exp((0.25-f*i/n)*math.tau*1j)
		#magnatude * normalization * last_element_adjustment
		bands[f]=bands[f] * (2/n) * ((1,0.5)[f==0],0.5)[f==m-1]
	return bands

arr=[]
n=100
for i in range(0,n):
	#v=1*math.sin(0.5*i*math.tau/n+0.2*math.tau)
	v=2.5*math.sin(10*i*math.tau/n+0.5*math.tau) #+ 3*math.sin(3*i*math.tau/n+0.6*math.tau) + 2*math.sin(5*i*math.tau/n+0.9*math.tau)
	#v=4*((int((5*i/n*2)%2))-0.5)*2
	#v*=2*(n-i)/n
	#v=3*math.sin(13*i*math.tau/n) + 3*math.sin(23*i*math.tau/n)
	#v=2*math.cos(n/2*i*math.tau/n)
	arr.append(v)

graph(arr,scalex=1)
print()

bands=fourier(arr)
graphcpx(bands)
print()

m=len(bands)
arr2=[0]*n
for f in range(0,m):
	for i in range(0,n):
		arr2[i]+=abs(bands[f])*math.sin(f*((i/n)*math.tau) + cmath.phase(bands[f])) #
		#arr2[i]=5*math.cos(f*((i/(n/d))*math.tau) + s*f*math.tau + f/(m/2)*math.tau)

graph(arr2,scalex=1)
print()


d=50
bands=ceildiv(n,2)+1
spectrum=[[0]*bands for i in range(0,d)]
for i in range(0,n):
	#l=minmax(i-d,0,d)
	m=ceildiv(n+1,2)
	
	for f in range(0,m):
		imag = lambda i: arr[i] * cmath.exp((0.25-f*i/n)*math.tau*1j)
		val = imag(i)
		val=val+spectrum[1][f]#-( imag(i-d) if i-d>=0 else 0)

		spectrum[0][f]=val# * (2/n) * (1,0.5)[f==m-1] * (1,0.5)[f==0]
	
		#spectrum[0][f]=spectrum[0][f] * (2/n) * ((1,0.5)[f==0],0.5)[f==n-1]

	'''if i<4:
		for f in range(0,m):
			v=spectrum[0][f] * (2/n) * ((1,0.5)[f==0],0.5)[f==m-1]
			print("i",i," m",m," f",f,":", abs(v), cmath.phase(v))
		print()
	'''
	'''for f in range(0,bands):
		v=abs(spectrum[0][f])
		print("\033[38;5;" + str(int(23*v/10+232)) + "m",end=hex(int(v))[2])
	print("\033[0m")
	'''

	spectrum=[[0]*bands]+spectrum[:-1]


for f in range(0,len(spectrum[1])): spectrum[1][f] = spectrum[1][f] * (2/n) * ((1,0.5)[f==0],0.5)[f==m-1]
graphcpx(spectrum[1])


'''
print()

f=fourier([0]*(n-4)+arr[0:4])
i=0
for b in f: print(i,"::",abs(b),cmath.phase(b)); i+=1
'''

