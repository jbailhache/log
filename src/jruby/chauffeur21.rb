
require 'ruboto' 

ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText 

java_import "android.graphics.Color"
java_import "android.graphics.Paint"
java_import "android.graphics.RectF"

ruboto_import "org.ruboto.RubotoView"

$activity.start_ruboto_activity "$add" do 

 @ruboto_view = RubotoView.new(self)
  
 @tt = 0 

 setup_content do 

  @color1 = Color.argb(255,210,120,40)
  @color2 = Color::GREEN
  @color3 = Color.argb(255,120,40,60)

  @paint1 = Paint.new
  @paint1.setAntiAlias(true)
  @paint1.setStyle(Paint::Style::FILL)
  @paint1.setColor(@color1)

  @paint2 = Paint.new
  @paint2.setAntiAlias(true)
  @paint2.setStyle(Paint::Style::FILL)
  @paint2.setColor(@color2)

  @paint3 = Paint.new
  @paint3.setAntiAlias(true)
  @paint3.setStyle(Paint::Style::FILL)
  @paint3.setColor(@color3)
 
  @paint4 = Paint.new
  @paint4.setAntiAlias(true)
  @paint4.setStyle(Paint::Style::FILL)
  @paint4.setColor(Color::WHITE)
 
  layout = LinearLayout.new(self)
  layout.orientation = LinearLayout::VERTICAL

  layout.add_view(@ruboto_view)
  
  @p1 = [0.0, 0.0, 0.0]
  @p2 = [0.0, 0.0, 0.0]
  # @p2 = [1.01, 1.01, 1.0] 
  # @p2 = [0.01, 0.01, 0.01]
  # @p1 = [-0.23, -0.06, -0.03] 
  # @p2 = [1.52, 1.13, 1.0] 
  # @p1 = [-3.375, -0.375, -3.75] 
  # @p2 = [-2.615, -0.365, 1.0] 

  @po1 = @p1.clone
  @po2 = @p2.clone
  @i = 0
  @s = 1
  @j = 1
  @slo1 = []
  @slo2 = []
  @dp0 = 1.0
  @dp = @dp0
  @amelior = 0
  @jp = 0
  @k = 0

  @h1 = 0
  @h2 = 0

  layout
 end 

 @ruboto_view.handle_draw do |canvas|
  canvas.drawColor(Color::BLACK)
  # canvas.drawCircle(@x+ @t, @y, 40, @paint)

  dt = 0.1
  v1 = 6.0
  v2 = 2.0
  ll = 40.0
  m = 60.0
  xmin = -m
  xmax = m
  ymin = m
  ymax = -m
  xxmin = 0
  xxmax = 400
  yymin = 0
  yymax = 400 
  # u1 = 0
  u1 = "undefined"

 display=0
 # while display==0 do
 if 0==0 

 # 0.upto(200) do |k|
 @k = @k+1
 # if @k <= 200
 if 0==0

  if @dp < 0.002
   ttt = 40.0
  else
   ttt = 40.0
  end


  px1 = @p1[0]
  py1 = @p1[1]
  pa1 = @p1[2]
  px2 = @p2[0]
  py2 = @p2[1]
  pa2 = @p2[2]

  # print "\n*** px1=",px1

  sl1 = 0.0
  sl2 = 0.0
  x1 = 10.0
  y1 = 0.0
  x2 = -3.0
  y2 = 1.0
  x = x2 - x1
  y = y2 - y1
  a = 0.0 

=begin
   px1 = -2*x
   py1 = -2*y
   pa1 = v1*(-px1*Math.sin(a)+py1*Math.cos(a))

   px2 = 2*x
   py2 = 2*y
   pa2 = v1*(-px2*Math.sin(a)+py2*Math.cos(a))
=end

  if @k<10000 # && @dp > 0.001
   @tt = ttt 
  end
  @dtt=5.0

  nt = @tt/dt 
  0.upto(nt) do |i|
   t = i * dt

   x = x2-x1
   y = y2-y1

   px1 = px1 + dt * (-2*x)
   py1 = py1 + dt * (-2*y)
   pa1 = pa1 + dt * (v1*(-px1*Math.sin(a)+py1*Math.cos(a)))
 
   px2 = px2 + dt * (2*x)
   py2 = py2 + dt * (2*y)
   pa2 = pa2 + dt * (v1*(-px2*Math.sin(a)+py2*Math.cos(a)))

=begin
   px1 = -2*x
   py1 = -2*y
   pa1 = v1*(-px1*Math.sin(a)+py1*Math.cos(a))

   px2 = 2*x
   py2 = 2*y
   pa2 = v1*(-px2*Math.sin(a)+py2*Math.cos(a))
=end

   alpha = 0.5
=begin
   px1 = (1-alpha)*px1 + alpha*(-2)*x
   py1 = (1-alpha)*py1 + alpha*(-2)*y
   pa1 = (1-alpha)*pa1 + alpha*v1*(-px1*Math.sin(a)+py1*Math.cos(a))

   px2 = (1-alpha)*px2 + alpha*2*x
   py2 = (1-alpha)*py2 + alpha*2*y
   pa2 = (1-alpha)*pa2 + alpha*v1*(-px2*Math.sin(a)+py2*Math.cos(a))
=end

=begin
   if u1=="undefined"
    # u1 = -0.5*v1**2/ll*(-px1*Math.sin(a)+py1*Math.cos(a)) 
    u1 = 0 
   end
   u1 = (-0.5) * pa1 * v1 / ll
   u2 = Math.atan(py2/px2)
   if px2<0
    if py2>=0
     u2=u2+Math::PI
    else
     u2=u2-Math::PI 
    end
   end
=end 

   u = solve(x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
   u1 = u[0]
   u2 = u[1]

   x1 = x1 + dt * v1*Math.cos(a)
   y1 = y1 + dt * v1*Math.sin(a)
   x2 = x2 + dt * v2*Math.cos(u2)
   y2 = y2 + dt * v2*Math.sin(u2)
   x = x2-x1
   y = y2-y1

   xx1 = Integer(xxmin + (xxmax-xxmin) * (x1-xmin) / (xmax-xmin))
   yy1 = Integer(yymin + (yymax-yymin) * (y1-ymin) / (ymax-ymin))
   xx2 = Integer(xxmin + (xxmax-xxmin) * (x2-xmin) / (xmax-xmin))
   yy2 = Integer(yymin + (yymax-yymin) * (y2-ymin) / (ymax-ymin))
  
   canvas.drawCircle(xx1,yy1,1,@paint1)
   canvas.drawCircle(xx2,yy2,1,@paint2)
     
   a = a + dt * v1/ll*u1
 
   sl1 = sl1 + dt * (u1 ** 2 + x ** 2 + y ** 2)
   sl2 = sl2 + dt * (- x**2 - y**2)

   @h1 = u1**2 + x**2 + y**2 + px1*(-v1*Math.cos(a)+v2*Math.cos(u2)) + py1*(-v1*Math.sin(a)+v2*Math.sin(u2)) + pa1*v1/ll*u1
   @h2 = -x**2 - y**2 + px2*(-v1*Math.cos(a)+v2*Math.cos(u2)) + py2*(-v1*Math.sin(a)+v2*Math.sin(u2)) + pa2*v1/ll*u1
 
  end

  # @ruboto_view.invalidate 
  @tt = @tt + @dtt
  if @tt >= ttt
   # print "p1[0]=",@p1[0]
   print "\n"
   print "\npx1=",@p1[0],"\npy1=",@p1[1],"\npa1=",@p1[2],"\npx2=",@p2[0],"\npy2=",@p2[1],"\npa2=",@p2[2]
   print "\ndp=",@dp
   print "\nsl1=",sl1,"\nsl2=",sl2 
   print "\nslo1=",@slo1,"\nslo2=",@slo2
   print "\njp=",@jp

   @tt = 0

   vdpa = 1.51
   vdpna = 0.5

   if @slo1 == []
    @slo1 = sl1
   end

   if @jp==1
    if sl1 < @slo1
     @slo1 = sl1
     @slo2 = sl2
     @po1 = @p1.clone
     # @po2 = @p2.clone
     @amelior = 1
     @dp = @dp * vdpa
     print "\namelioration 1"
     # canvas.drawColor(@color1)
     canvas.drawCircle(15,15,10,@paint1)
    else
     @p1 = @po1.clone
    end
   end

   if @slo2 == []
    @slo2 = sl2
   end

   if @jp==2
    if sl2 < @slo2
     @slo1 = sl1
     @slo2 = sl2
     # @po1 = @p1.clone
     @po2 = @p2.clone
     @amelior = 1
     @dp = @dp * vdpa
     print "\namelioration 2" 
     # canvas.drawColor(@color2)
     canvas.drawCircle(15,15,10,@paint2)
    else
     @p2 = @po2.clone
    end
   end
  
   if @j==1
    @p1[@i] = @p1[@i] + @s * @dp
   else
    @p2[@i] = @p2[@i] + @s * @dp
   end

   @jp = @j

   print "\n*** amelior=",@amelior

   if @amelior==0
    # canvas.drawColor(@color3)
    canvas.drawCircle(15,15,10,@paint3)
    @dp = @dp * vdpna
   else
    # canvas.drawColor(Color::BLUE)
   end

   if @s == 1
    @s = -1
   else
    @s = 1
    # @j = 3-@j
    # if @j==1
    @i = @i + 1
    if @i > 2
      @i = 0
      if @dp < 0.001
       @j = 3-@j 
       @dp = @dp0
      end
      # if @j==1
       if @amelior==0
        # @dp = @dp/2.0
        display=0
        # canvas.drawColor(Color::RED)
       else
        # @dp = @dp*1.5
        display=1
       end
       @amelior=0
      # end
    # end
    end
   end
 
  end
 
  @paint4.setTextSize(20)
  text = "%6.3f ; %6.3f ; %6.3f" % [@dp, sl1, sl2] 
  canvas.drawText(text,40,20,@paint4)
  text = "%6.3f ; %6.3f" % [@h1, @h2]
  canvas.drawText(text,40,50,@paint4)

  # if display!=0
   @ruboto_view.invalidate
  # end
 else

  ttt = 12.0

  px1 = @po1[0]
  py1 = @po1[1]
  pa1 = @po1[2]
  px2 = @po2[0]
  py2 = @po2[1]
  pa2 = @po2[2]

  # print "\n*** px1=",px1

  sl1 = 0.0
  sl2 = 0.0
  x1 = 0.0
  y1 = 0.0
  x2 = -3.0
  y2 = 1.0
  x = x2 - x1
  y = y2 - y1
  a = 0.0
  
  @tt = ttt
  nt = @tt/dt
  0.upto(nt) do |i|
   t = i * dt

   u1 = -0.5 * pa1 * v1 / ll
   u2 = Math.atan(py2/px2)
   if px2<0
    if py2>=0
     u2=u2+Math::PI
    else
     u2=u2-Math::PI 
    end
   end
   x1 = x1 + dt * v1*Math.cos(a)
   y1 = y1 + dt * v1*Math.sin(a)
   x2 = x2 + dt * v2*Math.cos(u2)
   y2 = y2 + dt * v2*Math.sin(u2)
   x = x2-x1
   y = y2-y1

   xx1 = Integer(xxmin + (xxmax-xxmin) * (x1-xmin) / (xmax-xmin))
   yy1 = Integer(yymin + (yymax-yymin) * (y1-ymin) / (ymax-ymin))
   xx2 = Integer(xxmin + (xxmax-xxmin) * (x2-xmin) / (xmax-xmin))
   yy2 = Integer(yymin + (yymax-yymin) * (y2-ymin) / (ymax-ymin))
  
   canvas.drawCircle(xx1,yy1,1,@paint1)
   canvas.drawCircle(xx2,yy2,1,@paint2)
     
   a = a + dt * v1/ll*u1
 
   px1 = px1 + dt * (-2*x)
   py1 = py1 + dt * (-2*y)
   pa1 = pa1 + dt * (v1*(-px1*Math.sin(a)+py1*Math.cos(a)))
 
   px2 = px2 + dt * (2*x)
   py2 = py2 + dt * (2*y)
   pa2 = pa2 + dt * (v1*(-px2*Math.sin(a)+py2*Math.cos(a)))

   sl1 = sl1 + dt * (u1 ** 2 + x ** 2 + y ** 2)
   sl2 = sl2 + dt * (- x**2 - y**2)

  end

 end

 end

 end

 def solve(x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
   u1 = (-0.5) * pa1 * v1 / ll
   u2 = Math.atan(py2/px2)
   if px2<0
    if py2>=0
     u2=u2+Math::PI
    else
     u2=u2-Math::PI 
    end
   end 
   return [u1,u2]
 end

 def solve2(x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
   du = 1
   u1 = 0
   u2 = 0
   uo2 = u2
   vo = value(u1,u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
   cont = 1
   while cont!=0 do
    u2=u2+du
    u1=u1fromu2(u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
    v = value(u1,u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
    if v<vo
     vo = v
     uo2 = u2
     du = du*1.5
    else
     u2=u2-2*du
     u1=u1fromu2(u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
     v = value(u1,u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
     if v<vo
      vo = v
      uo2 = u2
      du = du*1.5
     else
      u2=u2+du
      du=du*0.5
      if du<0.05
       cont=0
       uo1=u1fromu2(uo2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
       return [uo1,uo2]
      end
     end
    end
   end
 end 

 def value(u1,u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
   h1 = u1**2 + x**2 + y**2 + px1*(-v1*Math.cos(a)+v2*Math.cos(u2)) + py1*(-v1*Math.sin(a)+v2*Math.sin(u2)) + pa1*v1/ll*u1
   h2 = -x**2 - y**2 + px2*(-v1*Math.cos(a)+v2*Math.cos(u2)) + py2*(-v1*Math.sin(a)+v2*Math.sin(u2)) + pa2*v1/ll*u1
   return h1**2 + h2**2
 end

 def u1fromu2(u2,x,y,a,px1,py1,pa1,px2,py2,pa2,v1,v2,ll)
  return -ll/pa1/v1*(x**2+y**2-v1*(px1*Math.cos(a)+py1*Math.sin(a))+v2*(px2*Math.cos(u2)+py2*Math.sin(u2)))
 end

end 
