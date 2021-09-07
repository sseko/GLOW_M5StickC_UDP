import serial
import time

def main():
  ser = serial.Serial("COM7",115200,timeout=None)
  func = 1
  group = 2
  id = 3
  comm = 0x1c7
  message = (str(func)+","+str(group)+","+str(id)+","+str(comm)).encode('utf-8')
  ser.write(message)
  time.sleep(5)
  func = 0
  group = 2
  id = 3
  comm = 0
  message = (str(func)+","+str(group)+","+str(id)+","+str(comm)+"\n").encode('utf-8')
  ser.write(message)
  ser.close()
  return

if __name__ == '__main__':
  main()
