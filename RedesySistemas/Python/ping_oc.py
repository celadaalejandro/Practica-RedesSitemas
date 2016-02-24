import signal
import socket
import sys
import os
import time
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
sumador_rtt = 0
contador = 0
v_max = 0
v_min = 0

for x in xrange(int(sys.argv[3])):
	mensaje = str(mensaje).zfill(int(sys.argv[4])) 
	try: 
		s.send(mensaje)
		time_u = time.clock()
		#print '>>>Se ha enviado '+ str(len(mensaje)) +' bytes a la IP '+host_ip+':'+sys.argv[2]+'.'
	except socket.error, msg:
		print '\nNo se ha podido enviar los datos.\nTipo de error: ' + msg[1]
		break

	try:
		mensaje = s.recv((len(mensaje)))
		rtt = time.clock() - time_u
		print '<<<Se ha recibido '+ str(len(mensaje)) +' bytes de la IP '+ host_ip +':'+ sys.argv[2]+'. Seq= '+str(x+1)+' RTT: '+ str(rtt*1000000) +' us'
		sumador_rtt += rtt
		contador += 1
		if contador == 1:
			v_max = v_min = rtt
		if v_max < rtt:
			v_max = rtt
		if v_min > rtt:
			v_min = rtt
	except socket.error, msg:		
		print '\nNo se ha podido recibir datos o se ha pulsado CTRL+C.\nTipo de error: ' + msg[1]
		break
	if len(mensaje) == 0:
		break
	time.sleep(1)
	#Estadistica

print '\n\n-------------------------------------------------------------'
print 'El RTT de las '+str(contador)+' muestras es de '+ str((sumador_rtt/contador)*1000000) +' us.'
print 'Tiempo de rafaga total es: '+ str(sumador_rtt*1000000) +' us.'
print 'RTT max: '+str(v_max*1000000)+' us. RTT min: '+str(v_min*1000000)+' us.'
print '------------------------------------- --------------------------'
s.close()