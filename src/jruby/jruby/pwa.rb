
require 'ruboto'
ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText

$activity.start_ruboto_activity "$add" do
 def on_create(bundle)
  @ctx = self
  pwMain = PWMain.new(self)
 end
end

def startpw
end

class Edit

 def initialize(e)
  @e = e
 end

 def gettext
  return @e.getText.toString
 end

 def settext(t)
  @e.text = t
 end

end

class PW
 
 def initialize (t,ctx)
  @ctx = ctx
 end

 def frame
  f = LinearLayout.new(@ctx)
  f.orientation = LinearLayout::VERTICAL
  return f
 end

 def label (f, t)
  l = TextView.new(@ctx)
  l.text = t
  f.add_view(l)
  return l
 end

 def edit (f)
  e = EditText.new(@ctx)
  f.add_view(e)
  return Edit.new(e)
 end

 def button (f, t, c)
  b = Button.new(@ctx)
  b.text = t
  b.on_click_listener = c
  f.add_view(b)
  return b
 end

 def display (f)
  @ctx.content_view = f
 end  

end



