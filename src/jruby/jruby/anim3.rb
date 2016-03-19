
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

  layout
 end 

 @ruboto_view.handle_draw do |canvas|
  canvas.drawColor(Color::BLACK)
  canvas.drawCircle(@x+ @t, @y, 40, @paint)
       
  @ruboto_view.invalidate 
  @t = @t + 1 
 end

end 
