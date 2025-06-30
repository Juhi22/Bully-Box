import jnius
from jnius import autoclass
import threading
from kivy.properties import ObjectProperty

BluetoothAdapter = autoclass('android.bluetooth.BluetoothAdapter')
BluetoothDevice = autoclass('android.bluetooth.BluetoothDevice')
BluetoothSocket = autoclass('android.bluetooth.BluetoothSocket')
UUID = autoclass('java.util.UUID')

class Bluetooth():
  bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
  recv_stream = None
  send_stream = None
  response_text = ''

  def set_response_text(self, text: str):
    Bluetooth.response_text = text

  def get_socket_stream(name):
    remoteDevice = Bluetooth.bluetoothAdapter.getRemoteDevice("98:D3:11:FC:59:B7")
    socket = None
    print("KKKKK")
    print(remoteDevice.getName())
    # if remoteDevice != None and remoteDevice.getName() == 'HC-05':
    if remoteDevice != None:
      socket = remoteDevice.createRfcommSocketToServiceRecord(
        UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))
      recv_stream = socket.getInputStream()
      send_stream = socket.getOutputStream()
    socket.connect()
    return recv_stream, send_stream

  # recv_stream, send_stream = get_socket_stream('Lali', bluetoothAdapter)

  # def on_pre_enter(self):
  #   # self.bluetoothAdapter.enable()
  #   bluetoothAdapter.enable()

  def connect_bt(self):
      #TODO dj do we need this if?????????? do I need this try and except????????
      print("PPPPPPPPPP")
      if(threading.Thread(target=Bluetooth.stream_reader, name="bt_loop").is_alive()):
        threading.Thread(target=Bluetooth.stream_reader, name="bt_loop").stop()

      if not Bluetooth.bluetoothAdapter.isEnabled():
        Bluetooth.bluetoothAdapter.enable()

      while not Bluetooth.bluetoothAdapter.isEnabled():
        pass

      
      print("kikiki")
      if Bluetooth.recv_stream == None or Bluetooth.send_stream == None:
        Bluetooth.recv_stream, Bluetooth.send_stream = Bluetooth.get_socket_stream('Lali')
      # stopped = threading.Event()
      # threading.Thread(target=Bluetooth.stream_reader, args=(5, stopped)).start()
      print("oooooooooo")
      print(Bluetooth.recv_stream == None)
      print(Bluetooth.send_stream == None)

      threading.Thread(target=Bluetooth.stream_reader, name="bt_loop").start()
       # sm.current = "First"
    # except:
    #   Bluetooth.reconnect();
    #   #POPUP is NEEDED!
    #   print("BBB")
    #   pass
  
  # def reconnect():
  #   Bluetooth.connect_bt()

  def stream_reader():
    while True:
      newText = ''
      stream = ''
      while Bluetooth.recv_stream.available() > 0 and Bluetooth.response_text == '':
        try:
          stream = Bluetooth.recv_stream.read()
          # this is is needed to read X, previosuly many times the X was not read properly it was set to empty string
          if(chr(stream) == 'X'):
            Bluetooth.response_text = 'X'
            break
          if(stream == 10 or stream == 13):
            Bluetooth.response_text = newText
            break
          newText += chr(stream)
        except Exception as e:
          print(e)
        except jnius.JavaException as e:
          print("JavaException: "+ e.message)
        except:
          print("error")

          # except IOException as e:
          #   print ("IOException: "+ e.message)
          # except jnius.JavaException as e:
          #   print ("JavaException: "+ e.message)
          # except:
          #   print ("Misc error: "+ sys.exc_info()[0])

          # try:
          #   start = stream.rindex("<") + 1
          #   end = stream.rindex(">", start)
          #   # stopped.got_response(stream[start:end])
          # except ValueError:
          #   pass

  #delete it is not working / restart bt if it is not running
  def restart_bluetooth(self):
      if not MainMenuScreen.bluetoothAdapter.isEnabled():
        MainMenuScreen.bluetoothAdapter.enable()

  # def read():
  #   try:
  #         stream = Bluetooth.recv_stream.read()
  #         if stream > 32:
  #           Bluetooth.response_text = chr(stream)
  #   except self.IOException as e:
  #         print ("IOException: "+ e.message)
  #   except jnius.JavaException as e:
  #         print ("JavaException: "+ e.message)
  #   except:
  #         print ("Misc error: "+ sys.exc_info()[0])

  def send(self, level, msg: str):
    try:
      Bluetooth.send_stream.write(msg.encode())
      # while(Bluetooth.response_text == None):
      #   print("Response is None")
      level.response.text = Bluetooth.response_text
      Bluetooth.send_stream.flush()
    except:
        #POPUP NEEDED
        pass

