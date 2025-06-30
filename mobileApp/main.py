from kivy.app import App
import display
from kivy.utils import platform


class MainApp(App):
  def build(self):
    return display.sm


if __name__=="__main__":
  MainApp().run()