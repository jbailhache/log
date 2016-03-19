
require "ruboto.rb" 
confirm_ruboto_version(6) 
ruboto_import_widgets :LinearLayout, :EditText, :TextView 

$activity.start_ruboto_activity "$testui" do

 setup_content do 
  linear_layout(:orientation => LinearLayout::VERTICAL) do 
   @et = edit_text 
   linear_layout do 
    button :text => "Button 1" 
   end
   @tv = text_view :text => "Hello" 
  end
 end 

 handle_create_options_menu do |menu|
  add_menu("Menu 1") {menu1}
  add_menu("Quit") {finish}
  true
 end

 handle_click do |view|
  case view.getText
   when "Button 1"
    button1
   end 
 end

 def self.button1
  toast "Button 1 clicked"
  @et.setText(@et.getText.to_s + "- Button 1 clicked")
 end

 def self.menu1
  toast "Menu 1 clicked"
  @et.setText("- Menu 1 clicked")
 end

end 
