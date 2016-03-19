
load 'pwa.rb'

class PWMain
 
 def initialize(ctx)

  def on_click
   t = String(Float($a.gettext)+Float($b.gettext))
   $c.settext(t)
  end 

  pw = PW.new("Add",ctx)
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

 end
  
end

startpw

