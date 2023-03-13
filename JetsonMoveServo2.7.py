
from time import sleep; from serial import Serial;import struct
class LX16A:
  def __init__(self,Port="/dev/ttyUSB0",Baudrate=115200, Timeout= 0.001):
     self.serial = Serial(Port,baudrate=Baudrate,timeout=Timeout)
     self.serial.setDTR(1);self.TX_DELAY_TIME = 0.00002
     self.Header = struct.pack("<BB",0x55,0x55)
  def sendPacket(self,packet):
     packet1=bytearray(packet);sum=0
     for a in packet1: sum=sum+a
     fullPacket = bytearray(self.Header + packet + struct.pack("<B",(~sum) & 0xff))
     self.serial.write(fullPacket); sleep(self.TX_DELAY_TIME)
     for b in fullPacket:
       print(hex(b)[2:].zfill(2))
  def sendReceivePacket(self,packet,receiveSize):
     t_id = packet[0];t_command = packet[2]
     self.serial.flushInput();self.serial.timeout=0.1;self.sendPacket(packet)
     r_packet = self.serial.read(receiveSize+3); return r_packet 
  def moveServo(self,id,position,rate=1000):
     packet = struct.pack("<BBBHH",id,7,1,position,rate)
     self.sendPacket(packet)  # Move servo 0-1000, rate(ms) 0-30000(slow)
  def close(self):
    self.serial.close()
    #123 left 456 right

if __name__ == '__main__':
   m1 = LX16A() # creat a servo class object
   r=raw_input ("Press enter to move")
   while True:
     m1.moveServo(6,256,0)
     sleep(0.5)
     m1.moveServo(6,800,0)
     r=raw_input ("Press enter to move")     
   m1.close() 
