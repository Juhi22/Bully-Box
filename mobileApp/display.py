import random
import threading
from main import MainApp
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.properties import ObjectProperty
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.uix.dropdown import DropDown
from kivy.loader import Loader
import json
import jnius
from jnius import autoclass
from plyer import sms

from kivy.utils import platform
from android.permissions import request_permissions, check_permission, Permission

from models.quiz import Quiz
from models.smsdata import SmsData

from bt import Bluetooth

bt = Bluetooth()
bluetoothAdapter = bt.bluetoothAdapter

contacts = {}
smsData = SmsData()

quiz = Quiz()

with open('resources/default/quiz.json') as quizData:
  quiz.questions = json.load(quizData)

with open('resources/default/smsdata.json') as givenSmsData:
  currentSmsData = json.load(givenSmsData)
  smsData.names = currentSmsData['names']
  smsData.message = currentSmsData['message']

class MainMenuScreen(Screen):

  response = ObjectProperty(None)

  def on_enter(self, *args):
    if platform == 'android':
      if not check_permission('android.permission.READ_CONTACTS') or not check_permission('android.permission.SEND_SMS'):
        request_permissions([Permission.READ_CONTACTS, Permission.SEND_SMS])

  def start_quiz(self):
    if check_permission('android.permission.READ_CONTACTS') and check_permission('android.permission.SEND_SMS'):
      connect_bt()
      sm.current = "Quiz"
    else:
      App.get_running_app().stop()

class QuizScreen(Screen):

  question = ObjectProperty(None)
  answer1 = ObjectProperty(None)
  answer2 = ObjectProperty(None)
  answer3 = ObjectProperty(None)
  popupWindow = Popup() 

  def on_pre_enter(self, *args):
    threading.Thread(target=self.quizLoop, name="quiz_loop").start()
    # TODO does this send required here? the one in on_enter is required
    send_bt(self, 'R')

  def on_enter(self, *args):
    send_bt(self, 'R')

  def on_pre_leave(self, *args):
    threading.Thread(target=self.quizLoop).stop()

  def quizLoop(self):
    quiz_number_prefix = 'quiz_number:'
    while True:
      if(bt.response_text != ''):
        if(bt.response_text == 'end_it'):
          App.get_running_app().stop()
        if(bt.response_text.startswith(quiz_number_prefix)):
          quiz.number = int(bt.response_text[len(quiz_number_prefix):None])
          self.refresh()
        if(bt.response_text == 'X' and quiz.number is not None):
          self.popupWindow.dismiss()
          quiz.number = quiz.number + 1
          self.refresh()
        bt.set_response_text('')
        

  def refresh(self):
    if quiz.number is not None:
      try:
        main_text = list(quiz.questions[str(quiz.number)])[0]
        if main_text == 'task':
          self.showPopUp(main_text, list(quiz.questions[str(quiz.number)])[1])
        else:
          # get/set question
          self.question.text = main_text
          # get/set answers
          self.answer1.text = list(quiz.questions[str(quiz.number)][self.question.text][0])[0]
          self.answer2.text = list(quiz.questions[str(quiz.number)][self.question.text][1])[0]
          self.answer3.text = list(quiz.questions[str(quiz.number)][self.question.text][2])[0]
          # send number of the quiz
          if(quiz.number < 10):
            send_bt(self, '0')
            send_bt(self, str(quiz.number))
          else:
            send_bt(self, str(quiz.number)[0])
            send_bt(self, str(quiz.number)[1])
          # send the index of the good answer
          send_bt(self, str(self.find_good_answer(list(quiz.questions[str(quiz.number)][self.question.text]))))
      except KeyError as e:
        print(e)
      if quiz.number == 1:
        self.send_sms()

  def find_good_answer(self, answers: any):
    index = 0;
    for answer in answers:
      if list(answer.values())[0] == "good":
        return index
      index = index + 1

  def showPopUp(self, taskTitle: str, taskSign: str):
    show = PopUpContent()
    show.popup_text.text = quiz.questions[str(quiz.number)][taskSign]
    self.popupWindow = Popup(title=quiz.questions[str(quiz.number)][taskTitle], content=show)
    self.popupWindow.open()
    send_bt(self, taskSign)

  def send_bt(self, msg: str):
    send_bt(self, msg)

  def send_sms(self):
    self.set_contacts()
    if check_permission('android.permission.SEND_SMS'):
      contact_keys = contacts.keys()
      for contact in contacts:
        if contact.upper() in map(str.upper, smsData.names):
          sms.send(contacts[contact], smsData.message)

  def set_contacts(self):
    if check_permission('android.permission.READ_CONTACTS'):
      PythonActivity=autoclass("org.kivy.android.PythonActivity")
      Phone=autoclass("android.provider.ContactsContract$CommonDataKinds$Phone")
      Contacts=autoclass("android.provider.ContactsContract$Contacts")

      cr = PythonActivity.mActivity.getContentResolver()
      null = None # this will help to convert java examples into python ones
      cur = cr.query(Contacts.CONTENT_URI, null, null, null, null)
      if (cur.getCount() > 0):
          while (cur.moveToNext()):
            id = cur.getString(cur.getColumnIndex('_id'))
            name = cur.getString(cur.getColumnIndex('display_name'))

            if (cur.getInt(cur.getColumnIndex('has_phone_number')) > 0):
              pCur = cr.query(
                        Phone.CONTENT_URI,
                        null,
                        'contact_id' + ' = ?',
                        [id], null);
              while (pCur.moveToNext()):
                phoneNumber = pCur.getString(pCur.getColumnIndex(Phone.NUMBER))
                contacts[name] = phoneNumber
              pCur.close()
      cur.close()

def send_bt(self, msg: str):
  bt.send(self, msg)

def connect_bt():
  bt.connect_bt()

class PopUpContent(FloatLayout):
  popup_text = ObjectProperty(None)

class WindowManager(ScreenManager):
  pass

kv = Builder.load_file("display.kv")

sm = WindowManager()

screens = [MainMenuScreen(name="MainMenu"),
          QuizScreen(name="Quiz")]

for screen in screens:
    sm.add_widget(screen)

sm.current = "MainMenu"