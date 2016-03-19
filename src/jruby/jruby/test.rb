print "Bonjour !\n"

3.times { print "hip " }
print "hourra\n"

1.upto(9) {|x| print x }
print "\n"

suc = lambda {|x| x+1}
print suc.call(5),"\n"

def applyto(x)
 yield x
end
applyto(123) {|x| print x,"\n"}

def mymap(l,f)
 l.map {|x| f.call(x)}
end
a=[10,20,30,40,50,60]
a.each {|x| print x," "}
print "\n"
mymap(a,suc).each do |x| print x," " end
print "\n"

a.select {|x| x%20==0 }.each {|x| print x," "}
print "\n"

def myselect(l,&b)
 l.each do |x| 
  print x,":",b.call(x)," "
  if b.call(x)
   print "yield ", x, " "
   yield x
  end
 end
end

myselect(a) {|x| x%20==0 }.each {|x| print x," "}
print "\n"

r=myselect(a) {|x| x%20==0}
r.each {|x| print x," "}
print "\n"

def myselect2(l,b)
 l.each do |x| 
  # print x,":",b.call(x)," "
  if b.call(x)
   # print "yield ", x, " "
   yield x
  end
 end
end

myselect2(a, lambda {|x| x%20==0 }) {|x| print x," "}
print "\n"

def myselect3(l,&c)
 r=[]
 l.each do |x|
  if c.call(x)
   r=r+[x]
  end
 end
 r
end

print "myselect3: "
myselect3(a) {|x| x%20==0 }.each {|x| print x," "}
print "\n"

print a.inject {|x,y| x+y}, "\n"

h={:un=>1, :deux=>2, :trois=>3}
print h[:trois],"\n"

def inter(a,b)
 i=a
 while i<b
  yield i
  i=i+1
 end
end
inter(10,20) {|x| print x," "}
print "\n"

print callcc {|k| k.call(123)},"\n"

$x = 123





 






