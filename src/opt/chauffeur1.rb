
require 'ruboto' 

ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText 

java_import "android.graphics.Color"
java_import "android.graphics.Paint"
java_import "android.graphics.RectF"

ruboto_import "org.ruboto.RubotoView"

$activity.start_ruboto_activity "$add" do 

 @ruboto_view = RubotoView.new(self)
  
 @T = 0 

 setup_content do 

  @paint1 = Paint.new
  @paint1.setAntiAlias(true)
  @paint1.setStyle(Paint::Style::FILL)
  @paint1.setColor(Color::YELLOW)

  @paint2 = Paint.new
  @paint2.setAntiAlias(true)
  @paint2.setStyle(Paint::Style::FILL)
  @paint2.setColor(COLOR::GREEN)

  layout = LinearLayout.new(self)
  layout.orientation = LinearLayout::VERTICAL

  layout.add_view(@ruboto_view)
   
  layout
 end 

 @ruboto_view.handle_draw do |canvas|
  canvas.drawColor(Color::BLACK)
  # canvas.drawCircle(@x+ @t, @y, 40, @paint)

  T = 2.0

  px1 = 0.0
  py1 = 0.0
  pa1 = 0.0
  px2 = 1.01
  py2 = 1.01
  pa2 = 1.0

  dt = 0.04
  v1 = 5.0
  v2 = 3.0
  L = 2.0
  xmin = -10.0
  xmax = 10.0
  ymin = 10.0
  ymax = -10.0
  xxmin = 0
  xxmax = 400
  yymin = 0
  yymax = 400

  sl1 = 0.0
  sl2 = 0.0
  x1 = 0.0
  y1 = 0.0
  x2 = -3.0
  y2 = 1.0
  x = x2 - x1
  y = y2 - y1
  a = 0.0
  
  nt = @T/dt
  0.upto(nt) do |i|
   t = i * dt

   u1 = -0.5 * pa1 * V1 / L
   u2 = Math.atan(py2/px2)
   x1 = x1 + dt * v1*Math.cos(a)
   y1 = y1 + dt * v1*Math.sin(a)
   x2 = x2 + dt * v2*Math.cos(u2)
   y2 = y2 + dt * v2*Math.sin(u2)
   x = x2-x1
   y = y2-y1

   xx1 = Integer(xxmin + (xxmax-xxmin) * (x-xmin) / (xmax-xmin))
   yy1 = Integer(yymin + (yymax-yymin) * (y-ymin) / (ymax-ymin))
   canvas.drawCircle(xx1,yy1,6,paint1)
   canvas.drawCircle(xx2,yy2,4,paint2)
     
   a = a +dt * v1/L*U1
 
   px1 = px1 + dt * (-2*x)
   py1 = py1 + dt * (-2*y)
   pa1 = pa1 + dt * (v1*(-px1*Math.sin(a)py1*Math.cos(a)))
 
   px2 = px2 + dt * (2*x)
   py2 = py2 + dt * (2*y)
   pa2 = pa2 + dt * (v1*(-px2*Math.sin(a)+py2*Math.cos(a)))

   sl1 = sl1 + dt * (u1 ** 2 + x ** 2 + y ** 2)
   sl2 = sl2 + dt * (- x**2 - y**2)

  end

  @ruboto_view.invalidate 
  @T = @T + 1 
 end

end 
