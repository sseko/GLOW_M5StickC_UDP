from __future__ import print_function
import socket
import time
from contextlib import closing

def main():
  host = '192.168.4.1' # IPアドレス
  port = 10000 # ポート番号
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  with closing(sock):
    while True:
      func = 1
      group = 2
      id = 3
      comm = 0x1c7
      message = (str(func)+","+str(group)+","+str(id)+","+str(comm)).encode('utf-8')
      # print(message)
      sock.sendto(message, (host, port))
      time.sleep(2)
  return

if __name__ == '__main__':
  main()
