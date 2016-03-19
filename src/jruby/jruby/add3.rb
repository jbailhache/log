
require 'ruboto'
ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText

$activity.start_ruboto_activity "$add3" do
 def on_create(bundle)
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
  end 
  layout.add_view(b)

  self.content_view = layout
 end
end

