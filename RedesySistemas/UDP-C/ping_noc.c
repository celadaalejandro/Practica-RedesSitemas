#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //close()
#include <string.h> //strlen
#include <stdlib.h> //system("clear")
#include <unistd.h> //sleep()
#include <sys/time.h>//struct timeval
#define SIZE_SEND 32
int main (int argc, char *argv[])
{
	system("clear");
	if(argc!=4)
	{
		printf("ERROR:USO INCORRECTO DEL PROGRAMA ES:\n\n--->>ping_noc.c IP PUERTO N_REPETICIONES<<---\n\n");
		exit(1);
	}
	int i;
	char message[SIZE_SEND];
	memset((char*)&message, 0, sizeof(message));
	int fd;
	//Creamos el socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)		
	{
		perror("No se ha podido crear el socket");
		return 0;
	}
	//Estructura que nos identifica
	struct sockaddr_in client;				
	//Rellenamos los datos que identificaran al servidor	
	memset((char *)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(0);
		
	if (bind(fd, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		perror("No se ha podido enlazar el socket");
		close(fd);
		return 0;
	}
	//Estructura que identificara al server
	struct sockaddr_in server;
	//Rellenamos los datos que identificaran al servidor	
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	client.sin_addr.s_addr = inet_addr(argv[1]);
	//Datos recibidos del servidor
	char datos[100];
	int size_data;
	//Datos para el timer
	struct timeval t_ini, t_fin;
  	double secs;
  	double media=0;
	for(i=0;i<atoi(argv[3]);i++)
	{	
		secs=0;
		//Enviamos el ping
		
		if (sendto(fd, message, sizeof(message), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			gettimeofday(&t_ini, NULL);
			perror("Enrror al enviar los datos al servidor");
			close(fd);
			return 0;
		}
		else
		{
			gettimeofday(&t_ini, NULL);
			printf(">>> Se han enviado %d bytes a la IP %s a su puerto %d.\n",(int)sizeof(message),inet_ntoa(client.sin_addr),ntohs(server.sin_port));
		}
		//Recibimos datos
		int size_client=sizeof(server);
		if((size_data=(recvfrom(fd,datos,sizeof(datos),0,(struct sockaddr *)&server,&size_client)))==-1)
		{
			perror("Se ha recibido mal los datos");
			close(fd);
			return 0;
		}
		else
		{
			gettimeofday(&t_fin, NULL);
			secs =((double)(t_fin.tv_sec + (double)t_fin.tv_usec/1000000)-(double)(t_ini.tv_sec + (double)t_ini.tv_usec/1000000))*1000000;//Micrisegundos
			media=media+secs;
			printf("Se ha recibido %d  bytes de la IP %s de su puerto %d. RTT:  %f micrisegundos.\n",size_data,inet_ntoa(server.sin_addr),ntohs(server.sin_port),secs);
		}
		sleep(0,2);
	}

	printf("La media de RTT de las %d muestas es de %f microsegundos.\n",atoi(argv[3]),media/atoi(argv[3]));

	close(fd);
	return 0;
}

