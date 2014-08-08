/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **                STMicroelectronics STM32F4xx Standard Peripherals Library
 **
 **  Distribution: The file is distributed �as is,� without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. Distribution of this file (unmodified or modified) is not
 **  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
 **  rights to distribute the assembled, compiled & linked contents of this
 **  file as part of an application binary file, provided that it is built
 **  using the Atollic TrueSTUDIO(R) toolchain.
 **
 **
 *****************************************************************************
 */

/* Includes */
#include "stm32f10x.h"

#include "string.h"
#include "Delay.h"
#include "EtherShield.h"
#include "stm32f10x_conf.h"
/* Private macro */
/* Private variables */
// please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static uint8_t mymac[6] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x25 };

/*static uint8_t myip[4] = {
 192,168,0,15};*/

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE + 1];

uint16_t plen, dat_p;

/* Private function prototypes */
/* Private functions */
uint16_t http200ok(void);
uint16_t print_webpage(uint8_t *buf);
uint16_t print_webpage2(uint8_t *buf);

/* For DHCP */
static uint8_t myip[4] = { 0, 0, 0, 0 };
static uint8_t mynetmask[4] = { 0, 0, 0, 0 };

// IP address of the host being queried to contact (IP of the first portion of the URL):
static uint8_t websrvip[4] = { 0, 0, 0, 0 };

// Default gateway. The ip address of your DSL router. It can be set to the same as
// websrvip the case where there is no default GW to access the
// web server (=web server is on the same lan as this host)
static uint8_t gwip[4] = { 192, 168, 2, 1 };

static uint8_t dnsip[4] = { 0, 0, 0, 0 };
static uint8_t dhcpsvrip[4] = { 0, 0, 0, 0 };


void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

    /* Reset the RCC clock configuration to default reset state */
    RCC_DeInit();

    /* Configure the High Speed External oscillator */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait for HSE start-up */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        /* Set the code latency value: FLASH Two Latency cycles */
        FLASH_SetLatency(FLASH_Latency_2);

        /* Configure the AHB clock(HCLK): HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* Configure the High Speed APB2 clcok(PCLK2): PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* Configure the Low Speed APB1 clock(PCLK1): PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* Configure the PLL clock source and multiplication factor     */
        /* PLLCLK = HSE*PLLMul = 8*9 = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3);

        /* Enable PLL   */
        RCC_PLLCmd(ENABLE);

        /* Check whether the specified RCC flag is set or not */
        /* Wait till PLL is ready       */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Get System Clock Source */
        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08);
    }
}


/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void) {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_Configuration();
	/* SysTick end of count event each 1ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 1000);

	//ENC28J60_GPIO_Configuration();

	// By customising __initialize_args() it is possible to pass arguments,
	  // for example when running tests with semihosting you can pass various
	  // options to the test.
	  // trace_dump_args(argc, argv);

	  // Send a greeting to the trace device (skipped on Release).
	  trace_puts("Hello ARM World!");

	  // The standard output and the standard error should be forwarded to
	  // the trace device. For this to work, a redirection in _write.c is
	  // required.
	  puts("Standard output message.");
	  fprintf(stderr, "Standard error message.\n");

	  // At this stage the system clock should have already been configured
	  // at high speed.
	  trace_printf("System clock: %uHz\n", SystemCoreClock);
	  trace_printf("HSE = %d\n", HSE_VALUE);
	// Initialise SPI interface
	ES_enc28j60SpiInit();

// initialize enc28j60
	ES_enc28j60Init(mymac);

// init the ethernet/ip layer:
//ES_init_ip_arp_udp_tcp(mymac,myip, MYWWWPORT);

	if (ES_enc28j60Revision() <= 0) {
		// Failed to access ENC28J60
		while (1)
			;    // Just loop here
	}

// Get IP Address details
	if (allocateIPAddress(buf, BUFFER_SIZE, mymac, 80, myip, mynetmask, gwip,
			dhcpsvrip, dnsip) > 0) {
		// Display the results:
	} else {
		// Failed to get IP address"
		while (1)
			;
	}

	while (1) {
		// read packet, handle ping and wait for a tcp packet:
		dat_p = ES_packetloop_icmp_tcp(buf,
				ES_enc28j60PacketReceive(BUFFER_SIZE, buf));

		/* dat_p will be unequal to zero if there is a valid
		 * http get */
		if (dat_p == 0) {
			// no http request
			continue;
		}
		// tcp port 80 begin
		if (strncmp("GET ", (char *) &(buf[dat_p]), 4) != 0) {
			// head, post and other methods:
			dat_p = http200ok();
			dat_p = ES_fill_tcp_data(buf, dat_p, "<h1>200 OK</h1>");
			goto SENDTCP;
		}
		// just one web page in the "root directory" of the web server
		if (strncmp("/ ", (char *) &(buf[dat_p + 4]), 2) == 0) {
			dat_p = print_webpage(buf);
			goto SENDTCP;
		} else if (strncmp("/test ", (char *) &(buf[dat_p + 4]), 2) == 0) {
			dat_p = print_webpage2(buf);
			goto SENDTCP;
		} else {
			dat_p =
					ES_fill_tcp_data(buf, 0,
							"HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>");
			goto SENDTCP;
		}
		SENDTCP: ES_www_server_reply(buf, dat_p); // send web page data
		// tcp port 80 end

	}

}

uint16_t http200ok(void) {
	return (ES_fill_tcp_data(buf, 0,
			"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf) {
	uint16_t plen;
	plen = http200ok();
	plen =
			ES_fill_tcp_data(buf, plen,
					"<html><head><title>STM32F4 ENC28J60 Ethernet Demo</title></head><body>");
	plen = ES_fill_tcp_data(buf, plen,
			"<center><h1>Welcome to STM32F4 ENC28J60 Ethernet Demo</h1>");
	plen = ES_fill_tcp_data(buf, plen,
			"<br> Control digital outputs (not implemented yet)");
	plen = ES_fill_tcp_data(buf, plen,
			"<br> Read digital analog inputs (not implemented yet)");
	plen = ES_fill_tcp_data(buf, plen, "<br></font></h2>");
	plen = ES_fill_tcp_data(buf, plen, "</center><hr>");
	plen = ES_fill_tcp_data(buf, plen, "</body></html>");

	return (plen);
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage2(uint8_t *buf) {
	uint16_t plen;
	plen = http200ok();
	plen = ES_fill_tcp_data(buf, plen,
			"<html><head><title>Test page</title></head><body>");
	plen = ES_fill_tcp_data(buf, plen, "<center><h1>Second page</h1>");
	plen = ES_fill_tcp_data(buf, plen, "<br>Hello World");
	plen = ES_fill_tcp_data(buf, plen, "<br></font></h2>");
	plen = ES_fill_tcp_data(buf, plen, "</center><hr>");
	plen = ES_fill_tcp_data(buf, plen, "</body></html>");

	return (plen);
}

void ES_PingCallback(void) {
	//STM_EVAL_LEDToggle(LED4);
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	/* TODO, implement your code here */
	return -1;
}
