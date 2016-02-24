import signal
import socket
import sys
import os
import time


os.system('clear')
if len(sys.argv) != 5:
	print '\nEL USO CORRECTO ES: python %s <IP/Host> <Puerto> <Repeticiones> <Bytes>' % (sys.argv[0])
	sys.exit()

try:
	host_ip = socket.gethostbyname(sys.argv[1])
except socket.gaierror:
	print 'No se ha podido realizar la resolucion. Saliendo...'
	sys.exit()
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
	print '\nNo se ha podido crear el socket.\nTipo de error: ' + msg[1]
	sys.exit()

try:
	s.connect((host_ip, int(sys.argv[2])))
except socket.error, msg:
	print '\nNo se ha podido crear el socket.\nTipo de error: ' + msg[1]
	sys.exit()

print '-------->Mandando una rafaga de'+sys.argv[4]+'pings a la direcion IP '+ host_ip +':'+ sys.argv[2]+'<-----------'
mensaje = 0
aux=0
for x in xrange(int(sys.argv[3])):
	mensaje = str(mensaje).zfill(int(sys.argv[4])) 
	try: 
		s.send(mensaje)
		print '>>>Se ha enviado '+ str(len(mensaje)) +' bytes a la IP '+host_ip+':'+sys.argv[2]+'.'
	except socket.error, msg:
		print '\nNo se ha podido enviar los datos.\nTipo de error: ' + msg[1]
		break

	try:
		mensaje = s.recv((len(mensaje)))
		print '<<<Se ha recibido '+ str(len(mensaje)) +' bytes de la IP '+ host_ip +':'+ sys.argv[2]+'.'
	except socket.error, msg:		
		print '\nNo se ha podido recibir datos o se ha pulsado CTRL+C.\nTipo de error: ' + msg[1]
		break
	if len(mensaje) == 0:
		break
	time.sleep(1)
print 'METER ESTADSISTICA'
s.close()