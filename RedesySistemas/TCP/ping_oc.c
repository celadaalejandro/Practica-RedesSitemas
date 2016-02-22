#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //close()
#include <string.h> //strlen
#include <stdlib.h> //system("clear")
#include <unistd.h> //sleep()
#include <sys/time.h>//struct timeval
#include <signal.h> //  Señal para capturar CTRL+C
#define SIZE_SEND 32
int flag_CTRL_C=0;

void control(int sig)
{
 	flag_CTRL_C=1;
}

int main (int argc, char *argv[])
{
	system("clear");
	if(argc!=4)
	{
		printf("ERROR:USO INCORRECTO DEL PROGRAMA ES:\n\n--->>ping_noc.c IP PUERTO N_REPETICIONES<<---\n\n");
		exit(1);
	}

	signal(SIGINT, control);
	char message[SIZE_SEND];
	memset((char*)&message, 0, sizeof(message));
	int fd;
	//Creamos el socket
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)		
	{
		perror("No se ha podido crear el socket");
		close(fd);
		return 0;
	}
	//Estructura que identificara al server
	struct sockaddr_in server;
	//Rellenamos los datos que identificaran al servidor	
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	//Conexion al servidor
	if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("No se ha podido crear el socket");
		close(fd);
		return 0;
	}
	char datos[SIZE_SEND];
	int size_data;
	//Datos para el timer
	struct timeval t_ini, t_fin;
	double v_max,v_min;
  	double secs;
  	double media=0;
	printf("-------->Mandando una rafaga de %d pings a la direción IP %d:%d<--------\n\n",atoi(argv[3]),atoi(argv[1]),atoi(argv[2]));
	int flag = atoi(argv[3]);
	int i = 0;
	int contador_real=0;
	do
	{
		if(!flag_CTRL_C)
		{
			i++;
			size_data=sizeof(datos);
			//Enviamos el mesaje. Si se ha solitado el cirre (0 bytes).
			if (send(fd, datos,size_data,0)< 0)
			{
				perror("No se ha podido responder al cliente");
				close(fd);
				return 0;
			}
			else
			{
				gettimeofday(&t_ini, NULL);
				//printf(">>> Se han enviado %d bytes a la IP %s a su puerto %d.\n",size_data,inet_ntoa(server.sin_addr),ntohs(server.sin_port));
			}
			//Recibir mensaje del cliente
			int size_message;
			if((size_message = recv(fd,datos,sizeof(datos),0))==-1)
			{
				perror("No se ha podido recibir los datos del cliente");
				close(fd);
				return 0;
			}
			else if (size_message==0)
			{

				printf("\nCancelada la conexion TCP por el servidor. Cerramos el socket\n\n");
				i=flag;
				close(fd);
			}
			else
			{

				contador_real++;
				gettimeofday(&t_fin, NULL);
				secs =((double)(t_fin.tv_sec + (double)t_fin.tv_usec/1000000)-(double)(t_ini.tv_sec + (double)t_ini.tv_usec/1000000))*1000000;
				media=media+secs;
				printf("%d bytes de la IP %s:%d seq=%d RTT:  %f micrisegundos\n",size_data,inet_ntoa(server.sin_addr),ntohs(server.sin_port),i,secs);	
				if(contador_real==1)
					v_max=v_min=secs;
				if(v_max<secs)
					v_max=secs;
				if(v_min>secs)
					v_min=secs;

			}
			sleep(1);
		}else
		{
				printf("\nCancelada la conexion TCP por el cliente. Cerramos el socket\n\n");
				i=flag;
				close(fd);

		}
	}while(i<flag);
	//printf("\nEnviamos 0 bytes y cerramos el socket del cliente\n");
	close(fd);
	//Estadistica
	printf("\n-------------------------------------------------------------\n");
	printf("El RTT de las %d muestras es de  %f microsegundos.\n",contador_real,media/contador_real);
	printf("Tiempo de rafaga: %f microsegundos.\n",media);
	printf("RTT max: %f microsegundos. RTT min: %f microsegundos.\n",v_max,v_min);
	printf("------------------------------------- --------------------------\n");
	return 0;
}

