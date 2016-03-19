
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

  @paint1 = Paint.new
  @paint1.setAntiAlias(true)
  @paint1.setStyle(Paint::Style::FILL)
  @paint1.setColor(Color.argb(255,210,120,40))

  @paint2 = Paint.new
  @paint2.setAntiAlias(true)
  @paint2.setStyle(Paint::Style::FILL)
  @paint2.setColor(Color::GREEN)

  layout = LinearLayout.new(self)
  layout.orientation = LinearLayout::VERTICAL

  layout.add_view(@ruboto_view)
  
  @p1 = [0.0, 0.0, 0.0]
  @p2 = [1.01, 1.01, 1.0] 
  # @p2 = [0.01, 0.01, 0.01]
  # @p1 = [-0.23, -0.06, -0.03] 
  # @p2 = [1.52, 1.13, 1.0] 
  # @p1 = [-3.375, -0.375, -3.75] 
  # @p2 = [-2.615, -0.365, 1.0] 

  @po1 = @p1.clone
  @po2 = @p2.clone
  @i = 0
  @s = 1
  @j = 2
  @slo1 = []
  @slo2 = []
  @dp = 60.0
  @amelior = 0
  @jp = 0
  @ip = -1
  @k = 0

  layout
 end 

 @ruboto_view.handle_draw do |canvas|
  canvas.drawColor(Color::BLACK)
  # canvas.drawCircle(@x+ @t, @y, 40, @paint)

 
  dt = 0.04
  v1 = 6.0
  v2 = 2.0
  ll = 40.0
  xmin = -40.0
  xmax = 40.0
  ymin = 40.0
  ymax = -40.0
  xxmin = 0
  xxmax = 400
  yymin = 0
  yymax = 400 

 # 0.upto(200) do |k|
 @k = @k+1
 # if @k <= 200
 if 0==0

  ttt = 12.0

  # px1 = @p1[0]
  # py1 = @p1[1]
  # pa1 = @p1[2]
  px2 = @p2[0]
  py2 = @p2[1]
  pa2 = @p2[2]

  # px2 = -px1
  # py2 = -py1
  # pa2 = -pa1

  px1 = -px2
  py1 = -py2
  pa1 = -pa2

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
  
  if @k<1000
   @tt = ttt 
  end

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

=begin
   print "\ndt=",dt
   print "\nv1=",v1
   print "\nv2=",v2
   print "\na=",a
   print "\nu2=",u2
   print "\npx2=",px2
   print "\npy2=",py2

   print "\nx1=",x1
   print "\ny1=",y1
   print "\nx2=",x2
   print "\ny2=",y2
=end

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

   # px2 = x
   # py2 = y

   sl1 = sl1 + dt * (u1 ** 2 + x ** 2 + y ** 2)
   sl2 = sl2 + dt * (- x**2 - y**2)

  end

  # @ruboto_view.invalidate 
  @tt = @tt + 1  
  if @tt >= ttt
   # print "p1[0]=",@p1[0]
   print "\n"
   print "\npx1=",@p1[0],"\npy1=",@p1[1],"\npa1=",@p1[2],"\npx2=",@p2[0],"\npy2=",@p2[1],"\npa2=",@p2[2]
   print "\ndp=",@dp
   print "\nsl1=",sl1,"\nsl2=",sl2 
   print "\nslo1=",@slo1,"\nslo2=",@slo2
   print "\njp=",@jp

   @tt = 0

   if @slo1 == []
    @slo1 = sl1
   end

   # if @jp==1
   if @ip==2
    if sl1 < @slo1
     @slo1 = sl1
     @slo2 = sl2
     @po1 = @p1.clone
     @amelior = 1
     print "\namelioration 1"
    else
     @p1 = @po1.clone
    end
   end

   if @slo2 == []
    @slo2 = sl2
   end

   # if @jp==2
   if (@ip==0) || (@ip==1)
    if sl2 < @slo2
     @slo1 = sl1
     @slo2 = sl2
     @po2 = @p2.clone
     @amelior = 1
     print "\namelioration 2"
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
   @ip = @i
   
   if @s == 1
    @s = -1
   else
    @s = 1
    # @j = 3-@j
    # if @j==1
     @i = @i + 1
     if @i > 2
      @i = 0
      # @j = 3-@j 
      if @j==1
       if @amelior==0
        @dp = @dp/2
       else
        @dp = @dp*0.95
       end
       @amelior=0
      end
     end
    # end
   end
 
  end
  @ruboto_view.invalidate
  
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
