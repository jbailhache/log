
require 'ruboto'
ruboto_import_widgets :TextView, :LinearLayout, :Button, :EditText
load 'pwa.rb'

$activity.start_ruboto_activity "$add" do

 def on_click
  t = String(Float($a.gettext)+Float($b.gettext))
  $c.settext(t)
 end 

 def on_create(bundle)

  pw = PW.new("Add",self)
  top = pw.frame 

  pw.label(top, "Premier nombre")
  $a = pw.edit(top)
  pw.label(top, "Deuxieme nombre")
  $b = pw.edit(top)
  pw.label(top, "Resultat")
  $c = pw.edit(top)
  $c.settext("Test")
  pw.button(top, "Additionner", 
   proc { on_click })
  pw.display(top)

  self.content_view = top
 end

end


  

