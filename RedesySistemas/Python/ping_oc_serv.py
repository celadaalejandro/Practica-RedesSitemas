#!/usr/bin/env python
import signal
import socket
import sys
import os

os.system('clear')

if len(sys.argv) != 2:
	print '\nEL USO CORRECTO ES: python %s <Puerto>' % (sys.argv[0])
	sys.exit()

try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
	print '\nNo se ha podido crear el socket.\nTipo de error: ' + msg[1]
	sys.exit()

try:
	s.bind(('', int(sys.argv[1])))
except socket.error, msg:
	print '\nNo se ha podido enlazar el socket.\nTipo de error: ' + msg[1]
	s.close()
	sys.exit()

try:
	s.listen(1)
except socket.error, msg:
	print '\nNo se ha podido crear la cola.\nTipo de error: ' + msg[1]
	s.close()
	sys.exit()

print'------------>A la escucha de peticiones TCP por el puerto '+ str(int(sys.argv[1])) +'<------------'


try:
	sc, addr = s.accept()
except socket.error, msg:
	print '\nNo se ha aceptado la conexion.\nTipo de error: ' + msg[1]
	s.close()
	sys.exit()

 
while True:
	#Enviamos
	try:
		recibido = sc.recv(1024)
		print '>>>Se ha recibido '+ str(len(recibido)) +' bytes de la IP '+str(addr[0])+':'+str(addr[1])+'.'
	except socket.error, msg:
		print '\nNo se ha podido recibir datos o se ha pulsado CTRL+C.\nTipo de error: ' + msg[1]
		break

	if len(recibido) == 0:
		break
	#Reenviamos
	try:
		sc.send(recibido)
		print '<<<Se ha enviado '+ str(len(recibido)) +' bytes a la IP '+str(addr[0])+':'+str(addr[1])+'.'
	except socket.error, msg:
		print 'No se ha podido enviar.\nTipo de error: ' + msg[1]
		break



#Cerramos la instancia del socket cliente y servidor
print 'Cancelada la conexion TCP por el cliente. Cerramos el socket.\n\n'
sc.close()
s.close()