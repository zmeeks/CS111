#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <mraa/aio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

int sockfd, logfd;
int volatile stop = 0; //default
int cent = 0;	//default
int period = 3; //default
SSL *ssl;
const int B=4275;   //used in temp calculation

void *Servr_W()
{
	char str[19];		 
	int i, n;
	uint16_t value;
	int uid = XXXXXX115;  //note: I commented out my real UID for github
	mraa_aio_context temp;
	temp = mraa_aio_init(0);
	while(1)
	{
		while(stop){
			//cycling until stop == 0
		}			
		setenv("TZ", "PST8PST", 1);   // set TZ
		tzset();                // recognize TZ
		time_t lt=time(NULL);   //epoch seconds
		struct tm *p=localtime(&lt); // get local time struct tm
		char tmp[80]={0x0};
		strftime(tmp, 80, "%T", p);  // format time use format string, %c 		
		value = mraa_aio_read(temp);
		float R = 1023.0/((float)value)-1.0;
		R = 100000.0*R;		
		float temp_C=1.0/(log(R/100000.0)/B+1/298.15)-273.15;
		//^convert to temperature via datasheet
		float temp_F = temp_C*1.8 + 32.0;					
		if(cent){
			sprintf(str, "104502115 TEMP=%0.1f\n", temp_C);
			dprintf(logfd, "%s %0.1f\n", tmp, temp_C);
		}
		else{
			sprintf(str, "104502115 TEMP=%0.1f\n", temp_F);
			dprintf(logfd, "%s %0.1f\n", tmp, temp_F);
		}
		/* Send message to the server */
   		n = SSL_write(ssl, str, strlen(str)); 
   		if (n < 0) {
      		perror("ERROR writing to socket");
      		exit(1);
   		}
		sleep(period);
	}
}

void *Servr_R()
{
	int same, n, k, i, isdigit;
	int nada = 1;
	int num = 0;
	char *str;
	char buffer[256];
	char substr[8];
	char *sub_str;
	while(1)
	{
		/* Now read server response */
		memset(buffer, 0, 256);
		n = SSL_read(ssl, buffer, sizeof buffer);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		if(n > 0){
			str = malloc(n*sizeof(char));
			strncpy(str, buffer, n); 
			same = strcmp("OFF", str);
			if(same == 0){
				dprintf(logfd, "OFF");
				exit(0);
			}
			same = strcmp("STOP", str);
			if(same == 0){
				stop = 1;
				nada = 0;
				dprintf(logfd, "STOP\n");
			}
			same = strcmp("START", str);
			if(same == 0){
				stop = 0;
				nada = 0;
				dprintf(logfd, "START\n");
			}
			same = strcmp("SCALE=F", str);
			if(same == 0){
				cent = 0;
				nada = 0;
				dprintf(logfd, "SCALE=F\n");
			}
			same = strcmp("SCALE=C", str);
			if(same == 0){
				cent = 1;
				nada = 0;
				dprintf(logfd, "SCALE=C\n");
			}
			if(n > 7  && n <12){
				strncpy(substr, str, 7);
				substr[7] = '\0';
				same = strcmp("PERIOD=", substr);
				if(same == 0){
					k = n-7;
					sub_str = malloc((k+1) * sizeof(char));
					isdigit=1;
					memcpy(sub_str, &str[7], k);
					sub_str[k] = '\0';
					for(i=0;i<k;i++){
						if(sub_str[i] < '0' || sub_str[i] > '9')
							isdigit = 0;
						if(isdigit){
							num = num*10 + atoi(&sub_str[i]);
						}
					}
					if(isdigit && num <= 3600 && num >= 1){
						period = num;
						nada = 0;
						dprintf(logfd, "PERIOD=%d\n", num);
					}
				}
			}
			if(nada){
				dprintf(logfd, "%s I\n", str);
			}
		}//end of: if(N > 0)
   }//end of while(1)
}

int main() 
{
	int portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	pthread_t threads[2];
	logfd = open("lab4_3.log", O_TRUNC | O_CREAT | O_RDWR | O_APPEND);
	SSL_CTX *ctx;	
	portno = 17000;  
   /* Create a socket point */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
	}
	server = gethostbyname("r01.cs.ucla.edu");
	if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
	}
   /* Now time for SSL stuff */
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	if(SSL_library_init() < 0)
		fprintf(stderr, "Could not initialize the OpenSSL library !\n");
	if ( (ctx = SSL_CTX_new(TLSv1_client_method())) == NULL)
		fprintf(stderr, "Unable to create a new SSL context structure.\n");
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sockfd);
	if ( SSL_connect(ssl) != 1 )
    	fprintf(stderr, "Error: Could not build a SSL session\n");
	//**** Note: above code unnecessary since code works ****//
 	//  
	/* Now: execution of reads and writes to server via multi-threading */  
   	pthread_create(&threads[0], NULL, Servr_W, NULL);
	pthread_create(&threads[1], NULL, Servr_R, NULL);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);  
	return 0;
}


