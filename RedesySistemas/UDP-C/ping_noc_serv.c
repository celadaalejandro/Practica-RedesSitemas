#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //close()
#include <string.h> //strlen
#include <stdlib.h> //system("clear")
#define SIZE_MAX_RECV 1000

int main (int argc, char *argv[])
{
	system("clear");
	if(argc!=2)
	{
		printf("ERROR:USO INCORRECTO DEL PROGRAMA ES: PUERTO\n");
		exit(1);
	}
	//char message[]="No gods or kings";//server_reply[2000];
	int fd;
	//Creamos el socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)		
	{
		perror("cannot create socket");
		return 0;
	}
	//Estructura que nos identifica
	struct sockaddr_in server;				
	//Rellenamos los datos que identificaran al servidor	
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(atoi(argv[1]));
		
	if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("No se ha podido enlazar el socket");
		close(fd);
		return 0;
	}
	printf("------------>Esperando datos por el puerto %d .\n",atoi(argv[1]));
	while(1)
	{
		//Estructura que identificara a los clientes
		struct sockaddr_in client;
		int size_message;
		char datos[SIZE_MAX_RECV];
		unsigned int size_client = sizeof(client);
		//Recibir mensaje de respuesta
		if((size_message = recvfrom(fd,datos,sizeof(datos),0,(struct sockaddr *)&client, &size_client))==-1)
		{
			perror("No se ha podido enlazar el socket");
			close(fd);
			return 0;
		}
		else if (size_message==0)
		{
			printf("Se ha recibido 0 bytes. Cerramos el servidor.");
			close(fd);
			return 0;
		}
		else
		{
			printf("+++ Se ha recibido %d  bytes de la IP %s de su puerto %d.\n",size_message,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		}
		
		//Enviamos el ping
		if (sendto(fd, datos, size_message, 0, (struct sockaddr *)&client, sizeof(client)) < 0)
		{
			perror("No se ha podido responder al cliente");
			close(fd);
			return 0;
		}
		else
		{
			printf(">>> Se han enviado %d bytes a la IP %s a su puerto %d.\n",size_message,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		}

		
	}
}
