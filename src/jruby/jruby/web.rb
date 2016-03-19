require "net/http" 
def readweb(h, p, f) 
 h = Net::HTTP.new(h) 
 hd, b = h.get(p) 
 f = File.open(f, "w") 
 f.write(b) 
 f.close 
end 
