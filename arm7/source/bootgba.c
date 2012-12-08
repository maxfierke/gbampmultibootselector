#include <nds.h>

/*void BootGBAcheck(void) {
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;
	irqSet(IRQ_FIFO_NOT_EMPTY, BootGBA);
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ; // enable FIFO IRQ
}*/

void BootGBA(void) {



	uint8 current, backlight;
	if(PersonalData->_user_data.gbaScreen) {
		backlight = ~PM_BACKLIGHT_TOP;
	}
	else {
		backlight = ~PM_BACKLIGHT_BOTTOM;
	}
	// Reset the clock if needed
	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);

	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = 0x80;

	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz ;
	REG_SPIDATA = 0;

	SerialWaitBusy();
	current = REG_SPIDATA & 0xff;

	current = current & backlight;

	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHZ | SPI_CONTINUOUS;
	REG_SPIDATA = 0;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHZ;
	REG_SPIDATA = current;

	SerialWaitBusy();
	swiSwitchToGBAMode();


}

