/** \file main.c
 * \brief Sample LPC1769 project
 * \details This file holds a very basic code for LPC1769. This code
 * configures Flash wait-states and enables the PLL0 to achieve the highest
 * allowed frequency for LPC1769 (120MHz). Main code block just blinks the LED.
 * The LED port and pin are defined in config.h file. Target core frequency and
 * quartz crystal resonator frequency are defined there as well.
 *
 * \author Freddie Chopin, http://www.freddiechopin.info/
 * \date 2012-04-07
 */

/******************************************************************************
* project: lpc1769_blink_led
* chip: LPC1769
* compiler: arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors) 4.6.2
* 	20110921 (release) [ARM/embedded-4_6-branch revision 182083]
*
* prefix: (none)
*
* available global functions:
* 	int main(void)
*
* available local functions:
* 	static void flash_latency(uint32_t frequency)
* 	static void pll0_feed(void)
* 	static uint32_t pll0_start(uint32_t crystal, uint32_t frequency)
* 	static void system_init(void)
*
* available interrupt handlers:
******************************************************************************/

/*
+=============================================================================+
| includes
+=============================================================================+
*/

#include <stdint.h>

#include "inc/LPC17xx.h"

#include "config.h"

#include "hdr/hdr_sc.h"

#include "gpio.h"

/*
+=============================================================================+
| module variables
+=============================================================================+
*/

/*
+=============================================================================+
| local functions' declarations
+=============================================================================+
*/

static void flash_latency(uint32_t frequency);
static void pll0_feed(void);
static uint32_t pll0_start(uint32_t crystal, uint32_t frequency);
static void system_init(void);

/*
+=============================================================================+
| global functions
+=============================================================================+
*/

/*------------------------------------------------------------------------*//**
* \brief main code block
* \details Call some static initialization functions and blink the led with
* frequency defined via count_max variable.
*//*-------------------------------------------------------------------------*/

int main(void)
{
	volatile uint32_t count, count_max = 1000000;

	system_init();
	pll0_start(CRYSTAL, FREQUENCY);

	gpio_pin_cfg(LED_GPIO, LED_pin, GPIO_OUT_PP);

	while (1)
	{
		for (count = 0; count < count_max; count++);	// delay
		LED_bb = 1;
		for (count = 0; count < count_max; count++);	// delay
		LED_bb = 0;
	}
}

/*
+=============================================================================+
| local functions
+=============================================================================+
*/

/*------------------------------------------------------------------------*//**
* \brief Configures Flash latency
* \details Configures Flash latency (wait-states) which allows the chip to run
* at higher speeds
*
* \param [in] frequency defines the target frequency of the core
*//*-------------------------------------------------------------------------*/

static void flash_latency(uint32_t frequency)
{
	uint32_t wait_states;

	wait_states = frequency / 20000000;		// 1 CLK per 20MHz

	if (wait_states > 5)					// 5 CLKs is the highest reasonable value, works for up to 120MHz
		wait_states = 5;

	LPC_SC->FLASHCFG = ((wait_states - 1) << LPC_SC_FLASHCFG_FLASHTIM_bit) | LPC_SC_FLASHCFG_RESERVED_value;	// set the latency
}

/*------------------------------------------------------------------------*//**
* \brief PLL0 feed sequence
* \details Issues a special "PLL0 feed sequence" which validates the changes to
* PLL0 control registers
*//*-------------------------------------------------------------------------*/

static void pll0_feed(void)
{
	LPC_SC->PLL0FEED = LPC_SC_PLL0FEED_FIRST;
	LPC_SC->PLL0FEED = LPC_SC_PLL0FEED_SECOND;
}

/*------------------------------------------------------------------------*//**
* \brief Starts the PLL0
* \details Configure and enable PLL0 to achieve some frequency with some
* crystal. Before the speed change Flash latency is configured via
* flash_latency(). PLL0 parameters prediv and mul and core clock divider are
* based on both function parameters. The PLL0 is set up, enabled and connected.
*
* \param [in] crystal is the frequency of the crystal resonator connected to the
* LPC1769
* \param [in] frequency is the desired target frequency after enabling the PLL0
*
* \return real frequency that was set
*//*-------------------------------------------------------------------------*/

static uint32_t pll0_start(uint32_t crystal, uint32_t frequency)
{
	uint32_t prediv, mul, corediv, fcco, core_frequency;
	uint32_t best_prediv = 0, best_mul = 0, best_corediv = 0, best_core_frequency = 0;

	if (crystal > 20000000)					// change OSCRANGE for crystals over 20MHz
		LPC_SC_SCS_OSCRANGE_bb = 1;

	LPC_SC_SCS_OSCEN_bb = 1;				// enable main oscillator

	flash_latency(frequency);				// set flash latency

	while (LPC_SC_SCS_OSCSTAT_bb == 0);		// wait for main oscillator to start up

	LPC_SC->CLKSRCSEL = LPC_SC_CLKSRCSEL_CLKSRC_MAIN;	// set main oscillator as PLL0 clocksource

	for (prediv = 1; prediv <= 32; prediv++)	// NSEL0 in [1; 32]
		for (mul = 6; mul <= 512; mul++)		// MSEL0 in [5; 512]
		{
			fcco = 2 * mul * crystal / prediv;	// calculate PLL output frequency

			if ((fcco < 275000000) || (fcco > 550000000))	// skip invalid settings - fcco must be in [275M; 550M]
				continue;

			for (corediv = 1; corediv <= 256; corediv++)	// CCLKSEL in [1; 256]
			{
				core_frequency = fcco / corediv;	// calculate core frequency

				if (core_frequency > frequency)		// skip frequencies above desired value
					continue;

				if (core_frequency > best_core_frequency)	// is this configuration better than previous one?
				{
					best_core_frequency = core_frequency;	// yes - save values
					best_prediv = prediv;
					best_mul = mul;
					best_corediv = corediv;

					if (core_frequency == frequency)	// is this configuration "perfect"?
						break;				// yes - skip further search
				}
			}
		}

	LPC_SC->PLL0CFG = ((best_prediv - 1) << LPC_SC_PLL0CFG_NSEL0_bit) | ((best_mul - 1) << LPC_SC_PLL0CFG_MSEL0_bit);	// set NSEL0 and MSEL0
	pll0_feed();							// validate change in PLL0CFG
	LPC_SC_PLL0CON_PLLE0_bb = 1;			// enable PLL0
	pll0_feed();							// validate change in PLL0CON
	LPC_SC->CCLKCFG = (best_corediv - 1) << LPC_SC_CCLKCFG_CCLKSEL_bit;	// set core clock divider CCLKSEL

	while (LPC_SC_PLL0STAT_PLOCK0_bb == 0);	// wait for PLL0 lock

	LPC_SC_PLL0CON_PLLC0_bb = 1;			// connect PLL0 as clock source
	pll0_feed();							// validate connection

	return best_core_frequency;
}

/*------------------------------------------------------------------------*//**
* \brief Initializes system
* \details Changes all PCLKs to 1:1 with core
*//*-------------------------------------------------------------------------*/

static void system_init(void)
{
	LPC_SC->PCLKSEL0 = LPC_SC_PCLKSEL0_PCLK_WDT_DIV1 |LPC_SC_PCLKSEL0_PCLK_TIMER0_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_TIMER1_DIV1 | LPC_SC_PCLKSEL0_PCLK_UART0_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_UART1_DIV1 | LPC_SC_PCLKSEL0_PCLK_PWM1_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_I2C0_DIV1 | LPC_SC_PCLKSEL0_PCLK_SPI_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_SSP1_DIV1 | LPC_SC_PCLKSEL0_PCLK_DAC_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_ADC_DIV1 | LPC_SC_PCLKSEL0_PCLK_CAN1_DIV1 |
			LPC_SC_PCLKSEL0_PCLK_CAN2_DIV1 | LPC_SC_PCLKSEL0_PCLK_ACF_DIV1;
	LPC_SC->PCLKSEL1 = LPC_SC_PCLKSEL1_PCLK_QEI_DIV1 | LPC_SC_PCLKSEL1_PCLK_GPIOINT_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_PCB_DIV1 | LPC_SC_PCLKSEL1_PCLK_I2C1_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_SSP0_DIV1 | LPC_SC_PCLKSEL1_PCLK_TIMER2_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_TIMER3_DIV1 | LPC_SC_PCLKSEL1_PCLK_UART2_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_UART3_DIV1 | LPC_SC_PCLKSEL1_PCLK_I2C2_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_I2S_DIV1 | LPC_SC_PCLKSEL1_PCLK_RIT_DIV1 |
			LPC_SC_PCLKSEL1_PCLK_SYSCON_DIV1 | LPC_SC_PCLKSEL1_PCLK_MC_DIV1;
}

/*
+=============================================================================+
| ISRs
+=============================================================================+
*/

/******************************************************************************
* END OF FILE
******************************************************************************/
