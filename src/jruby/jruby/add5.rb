
require 'ruboto'
ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText

$activity.start_ruboto_activity "$add3" do
 def on_create(bundle)
  btn_listener = proc do
   @r.setText(String(Float(@a.getText.toString) + Float(@b.getText.toString)))
  end
  self.content_view = linear_layout :orientation => LinearLayout::VERTICAL do 
   text_view :text => "a" 
   @a = edit_text 
   text_view :text => "b" 
   @b = edit_text 
   text_view :text => "a+b"
   @r = edit_text 
   button :text => "+",
          :on_click_listener => btn_listener
  end  
 end
end

