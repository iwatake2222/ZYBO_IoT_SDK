#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <errno.h>
#include <string.h>
#include "DevUIOMyIP.h"
#include "DevUIOPWM.h"

#define TIMEOUT (200)	// 200msec
DevUIOMyIP *pDevUIOMyIP;
DevUIOPWM *pDevUIOPWM;

int recvPipe(char *buf, int bufNumMax)
{
	int len = 0;
	int  fdP2C;	// pipe from Python to C  (read)
	if ((fdP2C = open("/home/root/pipe_p2c", O_RDWR)) < 0) {
		perror("open");
		exit(-1);
	}
	struct pollfd pfd;
	pfd.fd = fdP2C;
	pfd.events = POLLIN;
	if(poll(&pfd, 1, TIMEOUT) != 0) {
		len = read(fdP2C, buf, bufNumMax - 1);
		buf[len] = '\0';
	} else {
		printf("timeout\n");
	}
	close(fdP2C);
	return len;
}

void sendPipe(char *buf)
{
	int  fdC2P;	// pipe from C to Python  (write)
	if ((fdC2P = open("/home/root/pipe_c2p", O_RDWR)) < 0) {
		perror("open");
		exit(-1);
	}
	write(fdC2P, buf, strlen(buf));
	close(fdC2P);
}

void treatCmd(char *buf)
{
	/* command format = "LD0,LD1,LD2,LD3,R0,G0,B0,R1,G1,B1" */
	/* e.g. "ON,OFF,ON,OFF,100,100,100,100,100,100" */
	char separators[] = ",";
	char *prm;

	/* LEDs */
	if ((prm = strtok(buf, separators)) == NULL) {printf("err: %s\n", buf); return;}
	for (int indexLed = 0; indexLed < 4; indexLed++) {
		if (strcmp(prm, "ON") == 0) {
			pDevUIOMyIP->setLedBit(indexLed);
		} else if (strcmp(prm, "OFF") == 0) {
			pDevUIOMyIP->clrLedBit(indexLed);
		} else {
			printf("err: %s\n", buf);
		}
		if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	}

	/* Color LED0 */
	int r = 0;
	int g = 0;
	int b = 0;
	r = atoi(prm);
	if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	g = atoi(prm);
	if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	b = atoi(prm);
	printf("%d %d %d\n", r,g,b);
	pDevUIOPWM->setLed0(r, g, b);

	/* Color LED1 */
	if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	r = atoi(prm);
	if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	g = atoi(prm);
	if ((prm = strtok(NULL, separators)) == NULL) {printf("err: %s\n", buf); return;}
	b = atoi(prm);
	printf("%d %d %d\n", r,g,b);
	pDevUIOPWM->setLed1(r, g, b);
}

void sendStatus()
{
	/* get button / SW status */
	int btn = pDevUIOMyIP->getBtn();
	int sw = pDevUIOMyIP->getSw();

	/* generate JSON format string */
	/* e.g. "{"btn0":"OFF", "btn1":"OFF", "btn2":"OFF", "btn3":"OFF", "sw0":"OFF", "sw1":"OFF", "sw2":"OFF", "sw3":"OFF"}" */
	const char *btn0 = (btn & 0x01) ? "ON" : "OFF";
	const char *btn1 = (btn & 0x02) ? "ON" : "OFF";
	const char *btn2 = (btn & 0x04) ? "ON" : "OFF";
	const char *btn3 = (btn & 0x08) ? "ON" : "OFF";
	const char *sw0 = (sw & 0x01) ? "ON" : "OFF";
	const char *sw1 = (sw & 0x02) ? "ON" : "OFF";
	const char *sw2 = (sw & 0x04) ? "ON" : "OFF";
	const char *sw3 = (sw & 0x08) ? "ON" : "OFF";
//	printf("%s %s %s %s / %s %s %s %s\n", btn0, btn1, btn2, btn3, sw0, sw1, sw2, sw3);
	char buf[256];
	sprintf(buf, "{\
\"btn0\":\"%s\", \
\"btn1\":\"%s\", \
\"btn2\":\"%s\", \
\"btn3\":\"%s\", \
\"sw0\":\"%s\", \
\"sw1\":\"%s\", \
\"sw2\":\"%s\", \
\"sw3\":\"%s\"\
}", btn0, btn1, btn2, btn3, sw0, sw1, sw2, sw3);
//	printf("%s\n", buf);

	sendPipe(buf);
}

int main(int argc, char **argv)
{
	printf("Hello World!\n");
	char buf[256];
	pDevUIOMyIP = new DevUIOMyIP("/dev/uio0");
	pDevUIOPWM = new DevUIOPWM("/dev/uio1");

	while(1){
		int len = recvPipe(buf, sizeof(buf));
		if(len > 0) {
			printf("len = %d, str = %s\n", len, buf);
			treatCmd(buf);
		} else {
			sendStatus();
		}
	}

	return 0;
}

void testMyIP()
{
	DevUIOMyIP devUIOMyIP("/dev/uio0");
	if( !devUIOMyIP.isValid() ) {
		printf("Error to open DevUIOMyIP\n");
		exit(-1);
	}
	while(1) {
		devUIOMyIP.setLed(devUIOMyIP.getBtn());
		printf("%d\n", devUIOMyIP.getSw());
	}
}

void testPWM()
{
	DevUIOPWM devUIOPWM("/dev/uio1");
	if( !devUIOPWM.isValid() ) {
		printf("Error to open DevUIOPWM\n");
		exit(-1);
	}
	while(1) {
		devUIOPWM.setLed0(100, 0, 0);
		devUIOPWM.setLed1(100, 0, 0);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(0, 100, 0);
		devUIOPWM.setLed1(0, 100, 0);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(0, 0, 100);
		devUIOPWM.setLed1(0, 0, 100);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(0, 100, 100);
		devUIOPWM.setLed1(0, 100, 100);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(100, 0, 100);
		devUIOPWM.setLed1(100, 0, 100);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(100, 100, 0);
		devUIOPWM.setLed1(100, 100, 0);
		usleep(1*1000*1000);
		devUIOPWM.setLed0(100, 100, 100);
		devUIOPWM.setLed1(100, 100, 100);
		usleep(1*1000*1000);
	}
}
