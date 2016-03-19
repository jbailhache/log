
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

 setup_content do 

  @paint = Paint.new
  @paint.setAntiAlias(true)
  @paint.setStyle(Paint::Style::FILL)
  @paint.setColor(Color::YELLOW)

  layout = LinearLayout.new(self)
  layout.orientation = LinearLayout::VERTICAL

  t1 = TextView.new(self)
  t1.text = "a"
  layout.add_view(t1)

  @a = EditText.new(self)
  layout.add_view(@a)

  t2 = TextView.new(self)
  t2.text = "b"
  layout.add_view(t2)

  @b = EditText.new(self)
  layout.add_view(@b)

  @r = EditText.new(self)
  layout.add_view(@r)

  b = Button.new(self)
  b.text = "+" 
  b.on_click_listener = proc do
   @r.setText(String(Float(@a.getText.toString) + Float(@b.getText.toString)))
   @x = Integer(@a.getText.toString)
   @y = Integer(@a.getText.toString)
   @ruboto_view.invalidate
  end 
  layout.add_view(b)

  layout.add_view(@ruboto_view)

  layout
 end 

 @ruboto_view.handle_draw do |canvas|
        canvas.drawColor(Color::BLUE)
        canvas.drawCircle(@x, @y, 40, @paint) 
        @ruboto_view.invalidate
 end

end 
