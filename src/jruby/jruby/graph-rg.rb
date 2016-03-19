
require 'ruboto'

# def self.arcs(context)
    java_import "android.graphics.Color"
    java_import "android.graphics.Paint"
    java_import "android.graphics.RectF"

    ruboto_import "org.ruboto.RubotoView"

    # context.start_ruboto_activity "$arcs" do
    $activity.start_ruboto_activity "$arcs" do
      setTitle "Graphics/Arcs"

      @ruboto_view = RubotoView.new(self)

      setup_content do
       @paint = Paint.new
       @paint.setAntiAlias(true)
       @paint.setStyle(Paint::Style::FILL)
       @paint.setColor(Color::YELLOW)
       @ruboto_view
      end

      @ruboto_view.handle_draw do |canvas|
        canvas.drawColor(Color::BLACK)
        1.upto(255) { |x|
         1.upto(255) { |y|
          @paint.setColor(Color.argb(255,x,y,0))
          canvas.drawPoint(x,y,@paint)
         }}
        # canvas.drawCircle(60, 50, 40, @paint) 
        # @ruboto_view.invalidate
      end

    end
  
