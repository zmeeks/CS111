#include <math.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <mraa/aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int B=4275;                 // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int A0 = 0;     // Grove - Temperature Sensor connect to A0

int main()
{
	int i;
	uint16_t value;
	mraa_aio_context temp;
	temp = mraa_aio_init(0);
	int fd = open("lab4_1.log", O_TRUNC | O_CREAT | O_RDWR | O_APPEND);
	for(i=0; i<60; i++)
	{
		value = mraa_aio_read(temp);	
		float R = 1023.0/((float)value)-1.0;
		R = 100000.0*R;	
		float temp_C=1.0/(log(R/100000.0)/B+1/298.15)-273.15;
		//^convert to temperature via datasheet	
		float temp_F = temp_C*1.8 + 32.0;		
		time_t now; 
		time(&now);
		long long hrs = now/3600;
		int remainder = now - hrs*3600;
		int mins = remainder/60;
		int secs = remainder - mins*60;			
		setenv("TZ", "PST8PST", 1);   // set TZ
		tzset();                // recognize TZ
		time_t lt=time(NULL);   //epoch seconds
		struct tm *p=localtime(&lt); // get local time struct tm
		char tmp[80]={0x0};
		strftime(tmp, 80, "%T", p);  // format time use format string, %c 
		dprintf(fd, "%s %0.1f\n", tmp, temp_F);
		sleep(1);
	}
	mraa_aio_close(temp);
	return 0;
}
