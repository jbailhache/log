
require 'ruboto'
ruboto_import "org.ruboto.RubotoView"

require 'ruboto/widget'
require 'ruboto/util/toast'

ruboto_import_widgets :Button, :LinearLayout, :TextView

# http://xkcd.com/378/

class QuickStartActivity1
  # def on_create(bundle)
  def initialize
    # super
    # set_title 'Domo arigato, Mr Ruboto!'

    @cv = linear_layout :orientation => :vertical do
          @text_view = text_view :text => 'What hath Matz wrought?', :id => 42, :width => :match_parent,
                                 :gravity => :center, :text_size => 48.0
          button :text => 'M-x butterfly', :width => :match_parent, :id => 43, :on_click_listener => proc { butterfly }
        end
    self.content_view = @cv
  rescue
    puts "Exception creating activity: #{$!}"
    puts $!.backtrace.join("\n")
  end

  def getcv
    return @cv
  end

  private

  def butterfly
    @text_view.text = 'What hath Matz wrought!'
    toast 'Flipped a bit via butterfly'
  end

end

=begin
class QuickStartActivity
  def on_create(bundle)
    super

  end
end
=end

$activity.start_ruboto_activity "$quick_start" do
 setup_content do
   @act = QuickStartActivity1.new
   @act.getcv
 end
end
