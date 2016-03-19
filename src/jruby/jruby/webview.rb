java_import "android.util.Log"
java_import "com.mcgavren.jay.ruboto.MyActivity"

java_import "android.webkit.WebView"

class MyActivity
  def on_create(bundle)
    web = WebView.new(self)
    web.load_url 'http://mountainrb.com'
    self.content_view = web
  end
end
