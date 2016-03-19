
require 'ruboto' 

ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText 

java_import "android.graphics.Color"
java_import "android.graphics.Paint"
java_import "android.graphics.RectF"

ruboto_import "org.ruboto.RubotoView"

$activity.start_ruboto_activity "$add" do 

 @ruboto_view = RubotoView.new(self)
 @x = 60
 @y = 50 
 @t = 0 

 setup_content do 

  @paint = Paint.new
  @paint.setAntiAlias(true)
  @paint.setStyle(Paint::Style::FILL)
  @paint.setColor(Color::YELLOW)

  layout = LinearLayout.new(self)
  layout.orientation = LinearLayout::VERTICAL

  layout.add_view(@ruboto_view)
 
  @v1 = []
  @v2 = []
  0.upto(200) do |x1|
   @v1[x1] = []
   @v2[x1] = []
   0.upto(200) do |x2|
    if (x1-100)*(x1-100)+(x2-100)*(x2-100) > 10000 
     @v1[x1][x2] = x1
     @v2[x1][x2] = x2
    else
     @v1[x1][x2] = 0
     @v2[x1][x2] = 0
    end
   end
  end
  
  layout
 end 

 @ruboto_view.handle_draw do |canvas|
  canvas.drawColor(Color::BLACK)
  # canvas.drawCircle(@x+ @t, @y, 40, @paint)

  1.upto(199) do |x1|
   1.upto(199) do |x2|
    -1.upto(1) do |u|
     if @v1[x1][x2+u] > @v1[x1][x2]
      @v1[x1][x2] = @v1[x1][x2+u]
     end
     if @v2[x1+u][x2] > @v2[x1][x2]
      @v2[x1][x2] = @v2[x1+u][x2]
     end
    end
   end
  end

  0.upto(200) do |x1|
   0.upto(200) do |x2|
    @paint.setColor(Color.argb(255,@v1[x1][x2],@v2[x1][x2],0))
    canvas.drawPoint(x1,x2,@paint)    
   end
  end 

  # @ruboto_view.invalidate 
  @t = @t + 1 
 end

end 
