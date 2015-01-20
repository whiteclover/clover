#!/usr/bin/env python
# encoding: utf-8

from gevent import socket, spawn, joinall,sleep
import sys
import random
import struct
import os
import time

def send(client, code, msg):
    leng = len(msg)
    null = struct.pack('<B',  0)
    head = struct.pack('<HH', code, int(len(msg))+len(null))
    client.sendall(head+msg+null)

def recv(client):
    data = client.recv(4)
    cmd, leng  = struct.unpack("<HH", data)
    data = client.recv(leng)
    #print 'cmd : %d, msg : %s' %(cmd, data[:-1])

def client_send(s):
    for i in range(100):
        sleep(random.random()*3)
        msg = os.urandom(15).encode('hex')
        send(s, 13, msg)

def client_recv(s):
    for i in range(100):
        recv(s)

jobs = []

start = time.time()
for x in xrange(5000):
    ss = socket.socket()
    ss.connect(('127.0.0.1', 9999))
    jobs.append(spawn(client_send, ss))
    jobs.append(spawn(client_recv, ss))

print "Done"
joinall(jobs)
print "Spend : ", time.time() - start