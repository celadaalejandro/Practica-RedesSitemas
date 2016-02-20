#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //close(),write()
#include <string.h> //strlen
#include <stdlib.h> //system("clear")
#define CON_MAX 5
#define SIZE_MAX_RECV 32

int main(int argc , char * argv[])
{
	//int socket_desc, new_socket, c;
	//char client_message[2000], *message;

	system("clear");
	if(argc!=2)
	{
		printf("ERROR:USO INCORRECTO DEL PROGRAMA ES: PUERTO\n");
		exit(1);
	}
	int fd;
	//Creamos el socket
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)		
	{
		perror("No se ha podido crear el socket");
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
	//Cremaos la cola de la escucha
	if(listen(fd, CON_MAX)<0)
	{
		perror("No se ha podido enlazar el socket");
		close(fd);
		return 0;
	}
	printf("------------>A la escucha de peticiones TCP por el puerto %d<------------\n",atoi(argv[1]));
	struct sockaddr_in client;
	unsigned int size_client = sizeof(client);
	//Aceptar conexiones 
	if((fd=accept(fd, (struct sockaddr *)&client,&size_client))<0)
	{
		perror("No se ha podido aceptar la conexion en el server");
		close(fd);
		return 0;
	}
	else
	{
		printf("Aceptamos la conexion\n");
	}
	int flag=0;
	char datos[SIZE_MAX_RECV];
	int size_message;
	while(1)
	{

		while(!flag)
		{
			//Recibir mensaje del cliente
			if((size_message = recv(fd,datos,sizeof(datos),0))==-1)
			{
				perror("No se ha podido recibir los datos del cliente");
				close(fd);
				return 0;
			}
			else if (size_message==0)
			{
				printf("Se ha recibido 0 bytes. Cerramos la conexion TCP.\n\n");
				close(fd);
				return 0;
			}
			else
			{
				printf("+++ Se ha recibido %d  bytes de la IP %s de su puerto %d.\n",size_message,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
			}
			//Reenviamos el mesaje si no se ha solitado el cirre (0 bytes), y asi damos paso a la siguiente conexion
			if (send(fd, datos, size_message,0)< 0)
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
	close(fd);
	printf("Se ha cerrado el socket\n");
	return 0;
}

	     
	

