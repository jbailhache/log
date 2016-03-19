
def sigmat(t):
  return exp(-t)

def combtvn(x,y,param,t):
  z=x
  b=uniform(0.0,param[8]+param[9]+param[10]+param[11])
  if b<param[8]: 
    i=randint(0,len(z)-1)
    z[i]=uniform(-1.0,1.0)
  elif b<param[8]+param[9]:
    i=randint(0,len(z)-1)
    z[i]=z[i]+gauss(0.0,sigmat(t))
    if z[i]<-1.0: z[i]=-1.0
    if z[i]>1.0: z[i]=1.0
  elif b<param[8]+param[9]+param[10]+param[11]:
    if (param[4] != 0) and (x==y):
      return [uniform(-1.0,1.0) for i in range(len(x))]
    z=[0 for i in x]
    for i in range(len(x)):
        b=randint(0,1)
        if b==0: z[i]=x[i]
        else: z[i]=y[i]
  else:
    if (param[4] != 0) and (x==y):
      return [uniform(-1.0,1.0) for i in range(len(x))]
    z=[0 for i in x]
    for i in range(len(x)):
        z[i]=(x[i]+y[i])/2.0
  return z

      