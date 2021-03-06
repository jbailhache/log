﻿
import math

pi = 3.1415926

wd=800
hg=400

def cid(v):
  return format_cours_intraday (lire_isin(v))

def graphe_tf_1(intraday,t1,duree,n,tau0,tau1,test):
  graphe_tfg(intraday,1,1,t1,t1,duree,n,tau0,tau1,test)

def graphe_tfg_1(intraday,a,b,tr,t1,duree,n,tau0,tau1,test):
  master=Tk()
  w=Canvas(master,width=wd,height=hg)
  w.pack(side=LEFT)
  w.create_rectangle(1,1,wd-1,hg-1,fill=bgcolor)

  # intraday = format_cours_intraday (lire_isin(v))

  l0 = math.log(tau0)
  l1 = math.log(tau1)

  imin = 8*60
  imax = 19*60

  cimin = 1000000
  cimax = 0

  for i in intraday:
    if i[1]<cimin: cimin=i[1]
    if i[1]>cimax: cimax=i[1]

  cimin1 = 1000000
  cimax1 = 0

  for i in intraday:
    if i[0] < t1 :
      if i[1]<cimin1: cimin1=i[1]
      if i[1]>cimax1: cimax1=i[1]
      lt = i[0]
      lc = i[1]

  cimin2 = 1000000
  cimax2 = 0

  for i in intraday:
    if i[0] < t1 and t1-i[0] < duree:
      if i[1]<cimin2: cimin2=i[1]
      if i[1]>cimax2: cimax2=i[1]

  cimoy = (cimin+cimax)/2.0
  cimoy1 = (cimin1+cimax1)/2.0

  for i in range(0, 18):
    t = 9*60 + 30 * i
    x = wd * (t - imin) / (imax - imin)
    w.create_line (x, 1, x, hg-1, fill='grey')

  x = wd * (t1 - imin) / (imax - imin)
  w.create_line (x, 1, x, hg-1, fill='red')

  premier = 1
  x = 0
  y = 0

  for i in intraday:
    xp = x
    yp = y
    if test==0:
      c = i[1]
    else:
      c = 3260.0 + 10.0 * math.sin (i[0] / 10.0)
      # print 'i[0]=', i[0], 'c=', c, ' i[1]=', i[1]
    x = wd * (i[0]-imin) / (imax-imin)
    y = hg - hg * (c-cimin) / (cimax-cimin)
    if premier:
      premier = 0
    else:
      w.create_line (xp, yp, x, y, fill=inki)

  ft = intraday[0][0]
  fc = intraday[0][1]
  
  phi = []
  psi = []
  for o in range (n):
    # tau = tau0 + (tau1 - tau0) * o / (n - 1)
    l = l0 + (l1 - l0) * o / (n - 1)
    tau = math.exp(l)
    # print 'tau=',tau
    phi1 = 0
    psi1 = 0
    s = 0

    for j in range(1,len(intraday)-1):
      i = intraday[j]
      t = i[0] 

    # for t in range(9*60+1,18*60-1):
    #   i = intraday[0]

      tp = intraday[j-1][0] 
      ts = intraday[j+1][0] 

      if t <= t1:
        # lt = i[0]
        # lc = i[1]
        # c = i[1] - cimoy1
        if test==0:
          c = i[1] - lc
        else:
          c = 10.0 * math.sin (i[0] / 10.0) 
        e = math.exp(float(t-t1)/duree)
        c1 = e * c
        s = s + e
        # phi1 = phi1 + c1 * math.cos (t / tau) 
        # psi1 = psi1 + c1 * math.sin (t / tau) 

        phi1 = phi1 + e * c * math.cos ((t-tr) / tau) * (ts-tp)/2
        psi1 = psi1 + e * c * math.sin ((t-tr) / tau) * (ts-tp)/2 

        # phi1 = phi1 + e * c * math.cos ((t-tr) / tau) 
        # psi1 = psi1 + e * c * math.sin ((t-tr) / tau)  

        # print ' tau=', tau, ' t=', t, ' tr=', tr, ' phase=', (t-tr)/tau, ' cos=', cos((t-tr)/tau), ' sin=', sin((t-tr)/tau), ' c=', c

    # print 'tau=', tau, 'phi1=', phi1, 'psi1=', psi1

    phi = phi + [phi1]
    psi = psi + [psi1]

    # print 'tau=', tau, ' phi1=', phi1, ' psi1=', psi1

  # print 'phi=', phi
  # print 'psi=', psi

  phimin = 1000000
  phimax = -1000000
  psimin = 1000000
  psimax = -1000000

  for o in range(n):
    if phi[o] < phimin:
      phimin = phi[o]
    if phi[o] > phimax:
      phimax = phi[o]
    if psi[o] < psimin:
      psimin = psi[o]
    if psi[o] > psimax:
      psimax = psi[o]

  lm = math.log(10.0)
  om = (n-1) * (lm-l0) / (l1-l0) 
  xm = om * wd / (n-1)
  # w.create_line (xm, 1, xm, hg-1, fill='yellow')

  premier = 1
  x = 0
  y1 = 0
  y2 = 0
  for o in range(n):
    xp = x
    yp1 = y1
    yp2 = y2
    x = o * wd / (n-1)
    y1 = hg - hg * (phi[o] - phimin) / (phimax - phimin)
    y2 = hg - hg * (psi[o] - psimin) / (psimax - psimin)
    if premier:
      premier = 0
    else:
      w.create_line (xp, yp1, x, y1, fill='brown')
      w.create_line (xp, yp2, x, y2, fill='blue')

  premier = 1
  x = 0
  y = 0
  dmin = 1000000.0
  dmax = -1000000.0
  dmin1 = 1000000.0
  dmax1 = -1000000.0

  for t in range(imin,imax):
    d = 0
    for o in range (n):
      # tau = tau0 + (tau1 - tau) * o / (n - 1)
      l = l0 + (l1 - l0) * o / (n - 1)
      tau = math.exp(l)
      # d = d + phi[o] * math.cos (t / tau) + psi[o] * math.sin (t / tau) 
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      d = d + 1.0/tau * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/tau * (phi[o] * math.cos(t/tau) + psi[o] * math.sin(t/tau))
      # print 'tau=', tau, ' d=', d
    # d = d / n 
    # d = 1/(2*3.1415926) * (tau1-tau0)/(n-1) * d
    # print 'd=', d
    if d < dmin:
      dmin = d
    if d > dmax:
      dmax = d
    if t < t1:
      if d < dmin1:
        dmin1 = d
      if d > dmax1:
        dmax1 = d

  print 'cimin1=', cimin1, ' cimax1=', cimax1, ' dmin1=', dmin1, ' dmax1=', dmax1

  # t = ft
  # fd = 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))

  t = lt
  ld = 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))

  # dmin = fd + (cimin - fc) * (ld - fd) / float(lc - fc)
  # dmax = fd + (cimax - fc) * (ld - fd) / float(lc - fc) 
  
  # print 'ft=', ft, ' lt=', lt, ' fc=', fc, ' lc=', lc, ' fd=', fd, ' ld=', ld, ' cimin=', cimin, ' cimax=', cimax, ' dmin=', dmin, ' dmax=', dmax

  dmin = dmin1 + (cimin - cimin2) * (dmax1 - dmin1) / float(cimax2 - cimin2)
  dmax = dmin1 + (cimax - cimin2) * (dmax1 - dmin1) / float(cimax2 - cimin2)

  for t in range(imin,imax):
    xp = x
    yp = y
    d = 0
    for o in range (n):
      # tau = tau0 + (tau1 - tau) * o / (n - 1)
      l = l0 + (l1 - l0) * o / (n - 1)
      tau = math.exp(l)
      # d = d + phi[o] * math.cos (t / tau) + psi[o] * math.sin (t / tau) 
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      d = d + 1.0/tau * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/tau * (phi[o] * math.cos(t/tau) + psi[o] * math.sin(t/tau))
    # d = d / n 
    # d = 1/(2*3.1415926) * (tau1-tau0)/(n-1) * d
    # print 'd=', d
    d1 = cimin2 + (d - dmin1) * (cimax2 - cimin2) / float(dmax1 - dmin1)
    x = wd * (t - imin) / (imax - imin)
    # y = hg - hg * (d - dmin) / float(dmax - dmin)
    y = hg - hg * (d1 - cimin) / (cimax - cimin)
    if premier:
      premier = 0
    else:
      # print 't=', t, ' d=', d, ' xp=', xp, ' yp=', yp, ' x=', x, ' y=', y
      w.create_line (xp, yp, x, y, fill='green')

  print 'mainloop'

  mainloop()

def cours_t(intraday,t):
  t1 = 0
  c = 0
  for i in intraday:
    if i[0] > t1 and i[0] <= t:
      t1 = i[0]
      c = i[1]
  return c


def tf(intraday,t1,duree,n,tau0,tau1):

  n = int(n)

  a = 1
  b = 1
  tr = t1
  test = 0

  l0 = math.log (tau0)
  l1 = math.log (tau1)

  imin = 8*60
  imax = 19*60

  cimin = 1000000
  cimax = 0

  for i in intraday:
    if i[1]<cimin: cimin=i[1]
    if i[1]>cimax: cimax=i[1]

  cimin1 = 1000000

  cimax1 = 0

  for i in intraday:
    if i[0] < t1 :
      if i[1]<cimin1: cimin1=i[1]
      if i[1]>cimax1: cimax1=i[1]
      lt = i[0]
      lc = i[1]

  cimin2 = 1000000
  cimax2 = 0

  for i in intraday:
    if i[0] < t1 and t1-i[0] < duree:
      if i[1]<cimin2: cimin2=i[1]
      if i[1]>cimax2: cimax2=i[1]

  cimoy = (cimin+cimax)/2.0
  cimoy1 = (cimin1+cimax1)/2.0

  ft = intraday[0][0]
  fc = intraday[0][1]
  
  phi = []
  psi = []
  for o in range (n):
    # tau = tau0 + (tau1 - tau0) * o / (n - 1)
    l = l0 + (l1 - l0) * o / (n - 1)
    tau = math.exp(l)
    # print 'tau=',tau
    phi1 = 0
    psi1 = 0
    s = 0

    for j in range(1,len(intraday)-1):
      i = intraday[j]
      t = i[0] 

    # for t in range(9*60+1,18*60-1):
    #   i = intraday[0]

      tp = intraday[j-1][0] 
      ts = intraday[j+1][0] 

      if t <= t1:
        # lt = i[0]
        # lc = i[1]
        # c = i[1] - cimoy1
        if test==0:
          c = i[1] - lc
        else:
          c = 10.0 * math.sin (i[0] / 10.0) 
        e = math.exp(float(t-t1)/duree)

        c1 = e * c
        s = s + e
        # phi1 = phi1 + c1 * math.cos (t / tau) 
        # psi1 = psi1 + c1 * math.sin (t / tau) 

        phi1 = phi1 + e * c * math.cos ((t-tr) / tau) * (ts-tp)/2
        psi1 = psi1 + e * c * math.sin ((t-tr) / tau) * (ts-tp)/2 

        # phi1 = phi1 + e * c * math.cos ((t-tr) / tau) 
        # psi1 = psi1 + e * c * math.sin ((t-tr) / tau)  

        # print ' tau=', tau, ' t=', t, ' tr=', tr, ' phase=', (t-tr)/tau, ' cos=', cos((t-tr)/tau), ' sin=', sin((t-tr)/tau), ' c=', c

    # print 'tau=', tau, 'phi1=', phi1, 'psi1=', psi1

    phi = phi + [phi1]
    psi = psi + [psi1]

  phimin = 1000000
  phimax = -1000000
  psimin = 1000000
  psimax = -1000000

  for o in range(n):
    if phi[o] < phimin:
      phimin = phi[o]
    if phi[o] > phimax:
      phimax = phi[o]
    if psi[o] < psimin:
      psimin = psi[o]
    if psi[o] > psimax:
      psimax = psi[o]

  premier = 1
  x = 0
  y = 0
  dmin = 1000000.0
  dmax = -1000000.0
  dmin1 = 1000000.0
  dmax1 = -1000000.0

  for t in range(imin,imax):
    d = 0
    for o in range (n):
      # tau = tau0 + (tau1 - tau) * o / (n - 1)
      l = l0 + (l1 - l0) * o / (n - 1)
      tau = math.exp(l)
      # d = d + phi[o] * math.cos (t / tau) + psi[o] * math.sin (t / tau) 
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      d = d + 1.0/tau * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/tau * (phi[o] * math.cos(t/tau) + psi[o] * math.sin(t/tau))
      # print 'tau=', tau, ' d=', d
    # d = d / n 
    # d = 1/(2*3.1415926) * (tau1-tau0)/(n-1) * d
    # print 'd=', d
    if d < dmin:
      dmin = d
    if d > dmax:
      dmax = d
    if t < t1:
      if d < dmin1:
        dmin1 = d
      if d > dmax1:
        dmax1 = d

  # print 'cimin1=', cimin1, ' cimax1=', cimax1, ' dmin1=', dmin1, ' dmax1=', dmax1

  # t = ft
  # fd = 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))

  t = lt
  ld = 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))

  # dmin = fd + (cimin - fc) * (ld - fd) / float(lc - fc)
  # dmax = fd + (cimax - fc) * (ld - fd) / float(lc - fc) 
  
  # print 'ft=', ft, ' lt=', lt, ' fc=', fc, ' lc=', lc, ' fd=', fd, ' ld=', ld, ' cimin=', cimin, ' cimax=', cimax, ' dmin=', dmin, ' dmax=', dmax

  dmin = dmin1 + (cimin - cimin2) * (dmax1 - dmin1) / float(cimax2 - cimin2)
  dmax = dmin1 + (cimax - cimin2) * (dmax1 - dmin1) / float(cimax2 - cimin2)

  ctr = cours_t (intraday, tr)

  return [n, tau0, tau1, tr, ctr, cimin2, cimax2, dmin1, dmax1, phi, psi] 

def tfi1 (lp, t):
  n = lp[0]
  tau0 = lp[1]
  tau1 = lp[2]
  tr = lp[3]
  ctr = lp[4]
  cimin2 = lp[5]
  cimax2 = lp[6]
  dmin1 = lp[7]
  dmax1 = lp[8]
  phi = lp[9]
  psi = lp[10]

  a = 1
  b = 1
  l0 = math.log(tau0)
  l1 = math.log(tau1)

  d = 0
  for o in range (n):
      # tau = tau0 + (tau1 - tau) * o / (n - 1)
      l = l0 + (l1 - l0) * o / (n - 1)
      tau = math.exp(l)
      # d = d + phi[o] * math.cos (t / tau) + psi[o] * math.sin (t / tau) 
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/(tau*tau) * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      d = d + 1.0/tau * (a * phi[o] * math.cos((t-tr)/tau) + b * psi[o] * math.sin((t-tr)/tau))
      # d = d + 1.0/tau * (phi[o] * math.cos(t/tau) + psi[o] * math.sin(t/tau))
    # d = d / n 
    # d = 1/(2*3.1415926) * (tau1-tau0)/(n-1) * d
      # print 'd=', d
  d1 = cimin2 + (d - dmin1) * (cimax2 - cimin2) / float(dmax1 - dmin1)

  return d1

def tfi (lp, t):
  tr = lp[3]
  ctr = lp[4]
  return tfi1 (lp, t) + ctr - tfi1 (lp, tr)

  
def graphe_tf(intraday,t1,duree,n,tau0,tau1,test):

  master=Tk()
  w=Canvas(master,width=wd,height=hg)
  w.pack(side=LEFT)
  w.create_rectangle(1,1,wd-1,hg-1,fill=bgcolor)

  # intraday = format_cours_intraday (lire_isin(v))

  l0 = math.log(tau0)
  l1 = math.log(tau1)

  imin = 8*60
  imax = 19*60

  cimin = 1000000
  cimax = 0

  for i in intraday:
    if i[1]<cimin: cimin=i[1]
    if i[1]>cimax: cimax=i[1]

  cimin1 = 1000000
  cimax1 = 0

  for i in intraday:
    if i[0] < t1 :
      if i[1]<cimin1: cimin1=i[1]
      if i[1]>cimax1: cimax1=i[1]
      lt = i[0]
      lc = i[1]

  cimin2 = 1000000
  cimax2 = 0

  for i in intraday:
    if i[0] < t1 and t1-i[0] < duree:
      if i[1]<cimin2: cimin2=i[1]
      if i[1]>cimax2: cimax2=i[1]

  cimoy = (cimin+cimax)/2.0
  cimoy1 = (cimin1+cimax1)/2.0

  for i in range(0, 18):
    t = 9*60 + 30 * i
    x = wd * (t - imin) / (imax - imin)
    w.create_line (x, 1, x, hg-1, fill='grey')

  x = wd * (t1 - imin) / (imax - imin)
  w.create_line (x, 1, x, hg-1, fill='red')

  premier = 1
  x = 0
  y = 0

  for i in intraday:
    xp = x
    yp = y
    if test==0:
      c = i[1]
    else:
      c = 3260.0 + 10.0 * math.sin (i[0] / 10.0)
      # print 'i[0]=', i[0], 'c=', c, ' i[1]=', i[1]
    x = wd * (i[0]-imin) / (imax-imin)
    y = hg - hg * (c-cimin) / (cimax-cimin)
    if premier:
      premier = 0
    else:
      w.create_line (xp, yp, x, y, fill=inki)

  ft = intraday[0][0]
  fc = intraday[0][1]

  lp = tf (intraday, t1, duree, n, tau0, tau1)

  cimin2 = lp[5]
  cimax2 = lp[6]
  dmin1 = lp[7]
  dmax1 = lp[8]
  phi = lp[9]
  psi = lp[10]

  # print 'phi=', phi
  # print 'psi=', psi

  premier = 1
  x = 0
  y = 0

  for t in range(imin,imax):
    xp = x
    yp = y

    d1 = tfi (lp, t)

    x = wd * (t - imin) / (imax - imin)
    # y = hg - hg * (d - dmin) / float(dmax - dmin)
    y = hg - hg * (d1 - cimin) / (cimax - cimin)
    if premier:
      premier = 0
    else:
      # print 't=', t, ' d=', d, ' xp=', xp, ' yp=', yp, ' x=', x, ' y=', y
      w.create_line (xp, yp, x, y, fill='green')

  print 'mainloop'

  mainloop()


# exemple : simul(cid('FR0003500008'),30,12,3,6,15,0.0006)

def simul (intraday, duree, n, tau0, tau1, temps, seuil):
  ng = 0
  np = 0
  for i in range (16):
    t2 = 10*60 + 30*i
    t1 = t2 - temps
    lp = tf (intraday, t1, duree, n, tau0, tau1)    
    c1 = cours_t (intraday, t1)
    c2 = cours_t (intraday, t2)
    d1 = tfi (lp, t1)
    d2 = tfi (lp, t2)
    # print 't1=', t1, ' t2=', t2, ' c1=', c1, ' c2=', c2, ' d1=', d1, ' d2=', d2, ' var=', (d2-d1)/d1
    if (d2-d1) / d1 > seuil:
      if c2-c1 > 0:
        ng = ng+1
      else:
        np = np+1
    if (d1-d2) / d1 > seuil:
      if c1-c2 > 0:
        ng = ng+1
      else:
        np = np+1
  return [ng, np]

def simul_lp (intraday, lp):
  duree = lp[0]
  n = lp[1]
  tau0 = lp[2]
  tau1 = lp[3]
  temps = lp[4]
  seuil = lp[5]
  r = simul (intraday, duree, n, tau0, tau1, temps, seuil) 
  return r 
  
def meilleure_lp (intraday, n, lpmin, lpmax):
  mtaux = 0.0
  mgp = []
  mlp = []
  for i0 in range(n):
    p0 = lpmin[0] * ((lpmax[0] / float(lpmin[0])) ** (i0/float(n-1)))
    for i1 in range(n):
      p1 = lpmin[1] * ((lpmax[1] / float(lpmin[1])) ** (i1/float(n-1)))
      for i2 in range(n):
        p2 = lpmin[2] * ((lpmax[2] / float(lpmin[2])) ** (i2/float(n-1)))
        for i3 in range(n):
          p3 = lpmin[3] * ((lpmax[3] / float(lpmin[3])) ** (i3/float(n-1)))
          for i4 in range(n):
            p4 = lpmin[4] * ((lpmax[4] / float(lpmin[4])) ** (i4/float(n-1)))
            for i5 in range(n):
              p5 = lpmin[5] * ((lpmax[5] / float(lpmin[5])) ** (i5/float(n-1)))
              lp = [p0, p1, p2, p3, p4, p5]
              gp = simul_lp (intraday, lp)
              taux = gp[0] / float (gp[0] + gp[1])
              print 'lp=', lp, ' gp=', gp, ' taux=', taux
              if taux > mtaux:
                mtaux = taux
                mgp = gp
                mlp = lp
  return mlp



    
    

