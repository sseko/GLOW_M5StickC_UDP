# GLOW_M5StickC_UDP
## UDPまたはシリアルを使ってPythonからGLOWをコントロールする
##### プログラムの内容
UDPまたはシリアルを使ってPythonからM5StickC経由でGLOWをコントロールします。<br>
M5StickCをブリッジとして使うM5StickC用のプログラムです。

Pythonからのサンプルプログラムは以下の通りです。

##### UDPで通信する場合

<pre>
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
</pre>

##### シリアルで通信する場合

<pre>
pip install pyserial
</pre>

<pre>
import serial
import time

def main():
  ser = serial.Serial("COM7",115200,timeout=None)
  func = 1
  group = 2
  id = 3
  comm = 0x1c7
  message = (str(func)+","+str(group)+","+str(id)+","+str(comm)+"\n").encode('utf-8')
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
</pre>


UDPでもシリアルでも基本的に以下のフォーマットで送信します。

<pre>
function,groupID,deviceID,command
</pre>

- シリアルで通信する場合のボーレートは115200bpsです。
- シリアルで通信する場合、commandは10進数でも16進数でも指定できます。16進数の場合は最初に0xを付けます。

コマンド例
<pre>
1,1,2,0x1c7
1,1,2,455
0,1,2,0
</pre>

### GLOW
##### GLOWとは？
GLOWは Yume Cloud 社が開発したユニークなLEDキューブです。<br>
４つの高輝度カラーLEDを様々な光パターンで光らせることができるキューブ型デバイスです。

以下の特徴を持っています。

- 7つのグルーピングが可能
- １つのグループに1023個のデバイスIDを登録可能
- トータル７千１６１個のキューブを同時にコントロールできる
- ４つの高輝度カラーLEDを使って様々な光パターンを表現できる
- デバイスID、デバイスグループ、光パターンのすべてがプログラム可能
- 特に７千個以上のデバイスを同時にプログラムでコントロールできるデバイスは非常にユニークです。

これを使って大きなイベントを盛り上げたりするなど、使い方はアイディア次第です。

##### Arduinoからのコントロール

M5StickCのArduinoで書かれたプログラムコードによるコマンドに応じてGLOWをBluetooth経由でコントロールします。<br>
Arduinoで送るコマンドは以下のような書式になっています。<br>
sendGlowData(func,group,id,command);

コマンドは以下の４つの引数を持っています。

- ファンクション
- グループID
- デバイスID
- コマンド
- それぞれについての詳細を以下に説明します。

##### ファンクション
ファンクションは制御対象のGLOWへ送られる機能（命令）を示します。

今後機能が増えるにしたがって拡張されていきますが、現在は以下の6つの機能のみです。

- ０：GLOWを消灯する
- １：GLOWを点灯する
- ２：グループID設定スタート
- ３：グループID設定終了
- ４：デバイスID設定スタート
- ５：デバイスID設定終了

##### グループID
グループIDは制御対象のGLOWのグループを設定します。

1～7までの7つのグループを指定できます。

後に説明するデバイスIDが同じでもグループIDが異なれば制御対象から外れます。<br>
これを使って、複数のグループが同時にGLOWを制御しすることができます。<br>
また、グループIDに０を指定するとすべてのグループが制御対象となります。

##### デバイスID
デバイスの個別IDです。1～1023までの1023個指定できます。<br>
グループIDの場合と同じく、0を指定するとすべてのデバイスが制御対象になります。<br>
グループIDとデバイスIDの組み合わせで、<br>
最大、7 X 1023 = 7161 個のGLOWが同時に制御できます<br>
たぶん、これほどの数を同時にリアルタイムに制御できるデバイスは世の中にあまり例をみないと思います。

##### コマンド
GLOWの0x0011、0x000a、0x000b といった様に光パターンを指定します。

## GLOWの設定

GLOWは最初にグループIDとデバイスIDを設定する必要があります。<br>
設定はスマートフォンから簡単に行えます。 

- GLOW SDK for Android (https://play.google.com/store/apps/details?id=com.gadgetlabo.glow_sdk2)
- GLOW SDK for iOS (https://apps.apple.com/us/app/glow-sdk/id1575874546?ign-itsct=apps_box_link&ign-itscg=30200)

具体的な設定方法については以下のビデオを参照してください。

https://youtu.be/-QoyVQH0mNI

## GLOWについての詳細

https://www.yumecloud.net/glow/
