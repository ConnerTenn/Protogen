/**
 * \file
 *
 * \brief Low-level initialization functions called upon chip startup.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "samd21.h"
// #include "component/pm.h"

/**
 * Initial system clock frequency. The System RC Oscillator (RCSYS) provides
 *  the source for the main clock at chip startup.
 */
#define __SYSTEM_CLOCK    (1000000)

uint32_t SystemCoreClock = __SYSTEM_CLOCK;/*!< System Clock Frequency (Core Clock)*/

/**
 * \brief SystemInit() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - OSC8M clock source is enabled with a divider by 8 (1MHz).
 * - Generic Clock Generator 0 (GCLKMAIN) is using OSC8M as source.
 * We need to:
 * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator), will be used as DFLL48M reference.
 * 2) Put XOSC32K as source of Generic Clock Generator 1
 * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
 * 4) Enable DFLL48M clock
 * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
 * 6) Modify PRESCaler value of OSCM to have 8MHz
 * 7) Put OSC8M as source for Generic Clock Generator 3
 */
// Constants for Clock generators
#define GENERIC_CLOCK_GENERATOR_MAIN      (GCLK_CLKCTRL_GEN_GCLK0_Val)
#define GENERIC_CLOCK_GENERATOR_XOSC32K    (GCLK_CLKCTRL_GEN_GCLK1_Val)
// #define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M     (GCLK_CLKCTRL_GEN_GCLK3_Val)
// Frequency of the board main oscillator
#define VARIANT_MAINOSC (32768ul)
// Master clock frequency
#define VARIANT_MCK     (48000000ul)
// Constants for Clock multiplexers
// #define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)

void SystemInit( void )
{
	/* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

	/* Turn on the digital interface clock */
	// PM->APBAMASK.reg = PM->APBAMASK.reg | PM_APBAMASK_GCLK;
	PM->APBAMASK.bit.GCLK_ = 1;

	// ----------------------------------------------------------------------------------------------
	// 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)

	//Startup 65536 cycles, output enable, external crystal enable
	SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP(0x6u) | SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K ;
	SYSCTRL->XOSC32K.bit.ENABLE = 1; //Enable. Made as separate call

	// Wait for oscillator stabilization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY) == 0) { }

	// Software reset the module to ensure it is re-initialized correctly
	// Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
	// CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
	GCLK->CTRL.reg = GCLK_CTRL_SWRST;
	// Wait for reset to complete
	while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)) { }


	// ----------------------------------------------------------------------------------------------
	// 2) Put XOSC32K as source of Generic Clock Generator 1

	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_XOSC32K); // Generic Clock Generator 1
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	/* Write Generic Clock Generator 1 configuration */
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_XOSC32K) | // Generic Clock Generator 1
						GCLK_GENCTRL_SRC_XOSC32K | // Selected source is External 32KHz Oscillator
						GCLK_GENCTRL_GENEN;
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	// ----------------------------------------------------------------------------------------------
	// 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_DFLL48) | // Generic Clock Multiplexer 0
						GCLK_CLKCTRL_GEN_GCLK1 | // Generic Clock Generator 1 is source
						GCLK_CLKCTRL_CLKEN;
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	// ----------------------------------------------------------------------------------------------
	// 4) Enable DFLL48M clock
	// Will use Closed-Loop Mode however " The DFLL48M must be correctly configured before closed-loop operation can be enabled."

	// Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	// Wait for synchronization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0) { }

	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP(31) | // Coarse step is 31, half of the max value
							SYSCTRL_DFLLMUL_FSTEP(511) | // Fine step is 511, half of the max value
							SYSCTRL_DFLLMUL_MUL((VARIANT_MCK + VARIANT_MAINOSC/2) / VARIANT_MAINOSC); // External 32KHz is the reference
	// Wait for synchronization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0) { }


	// Write full configuration to DFLL control register 
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | // Enable the closed loop mode
							SYSCTRL_DFLLCTRL_WAITLOCK |
							SYSCTRL_DFLLCTRL_QLDIS; // Disable Quick lock
	// Wait for synchronization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0) { }

	// Enable the DFLL
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;
	// Wait for locks flags
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 || (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0) { }
	// Wait for synchronization
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 ) { }

	
	// ----------------------------------------------------------------------------------------------
	// 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.

	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_MAIN); // Generic Clock Generator 0
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	/* Write Generic Clock Generator 0 configuration */
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_MAIN) | // Generic Clock Generator 0
						GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
						GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
						GCLK_GENCTRL_GENEN;
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }


	// ----------------------------------------------------------------------------------------------
	// 6) Modify PRESCaler value of OSC8M to have 8MHz
	SYSCTRL->OSC8M.bit.PRESC = SYSCTRL_OSC8M_PRESC_0_Val;
	SYSCTRL->OSC8M.bit.ONDEMAND = 0 ;

	/* ----------------------------------------------------------------------------------------------
	* 7) Put OSC8M as source for Generic Clock Generator 3
	*/
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_OSC8M); // Generic Clock Generator 3

	/* Write Generic Clock Generator 3 configuration */
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_OSC8M) | // Generic Clock Generator 3
						GCLK_GENCTRL_SRC_OSC8M | // Selected source is RC OSC 8MHz (already enabled at reset)
						GCLK_GENCTRL_GENEN;
	// Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }


	// Now that all system clocks are configured, we can set CPU and APBx BUS clocks.
	// There values are normally the one present after Reset.
	PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1;
	PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val;
	PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val;
	PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val;

	SystemCoreClock=VARIANT_MCK;


	// ----------------------------------------------------------------------------------------------
	// 8) Load ADC factory calibration values

	// ADC Bias Calibration
	uint32_t bias = (*((uint32_t *)ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
	// ADC Linearity bits 4:0
	uint32_t linearity = (*((uint32_t *)ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
	// ADC Linearity bits 7:5
	linearity |= ((*((uint32_t *)ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;
	ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);

	// ----------------------------------------------------------------------------------------------
	// 9) Disable automatic NVM write operations
	NVMCTRL->CTRLB.bit.MANW = 1;
}

/**
 * Update SystemCoreClock variable
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate(void)
{
	// Not implemented
	SystemCoreClock = __SYSTEM_CLOCK;
	return;
}
