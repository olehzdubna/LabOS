/**
 * @brief 
 * @return 
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/spigenio.h>
#include <string.h>

#include "McASP.h"
#include "mcasp_api_defs.h"
#include "mcasp_hw.h"
#include "mcasp_ioctl.h"

/* Number of channels, L & R */
#define NUM_I2S_CHANNELS 2
#define I2S_SLOTS ((1 << NUM_I2S_CHANNELS) - 1)

/* McASP Serializer for Receive */
#define MCASP_XSER_RX                     (1u)

/* McASP Serializer for Transmit */
#define MCASP_XSER_TX                     (0u)


class transfer;
McASP::McASP()
  : mcaspFd(-1)
  , available(0)
  , offset(0)
  , dataLen(8192)
  , numChannels(2)
{
}

McASP::~McASP()
{
    ::close(mcaspFd);
}

bool McASP::initialize()
{
    static const char* ipiName = "/dev/mcasp0"; 
    
    mcaspFd = ::open(ipiName, 0);
    if(mcaspFd < 0)
    {
        perror("+++ Cannot open McASP device");
        return false;
    }
    
//    if(::ioctl(spiFd, MCASP_IOCTL_TX_RESET) < 0)
//    {
//        perror("+++ McASP: Cannot reset TX");
//        return false;
//    }
//
//    printf("McASP TX was reset\n");
//
//    if(::ioctl(spiFd, MCASP_IOCTL_RX_RESET) < 0)
//    {
//        perror("+++ McASP: Cannot reset RX");
//        return false;
//    }
//    printf("McASP TX was reset\n");

    printf("*** McASP initialized ***\n");

    return true;
}

uint32_t McASP::getTxStatus() {
	uint32_t status = 0;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_STATUS_GET, &status) < 0)
    {
        perror("+++ McASP: Cannot get TX status");
        return false;
    }

	return status;
}

uint32_t McASP::getRxStatus() {
	uint32_t status = 0;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_STATUS_GET, &status) < 0)
    {
        perror("+++ McASP: Cannot get RX status");
        return false;
    }

	return status;
}

uint32_t McASP::getGlobalStatus() {
	uint32_t status = 0;

	if(::ioctl(mcaspFd, MCASP_IOCTL_GLOBAL_STATUS_GET, &status) < 0)
    {
        perror("+++ McASP: Cannot get Global status");
        return false;
    }

	return status;
}


bool McASP::setTxStatus(uint32_t status) {

	status = \
			 MCASP_RX_STAT_STARTOFFRAME |
			 MCASP_RX_STAT_DATAREADY |
			 MCASP_RX_STAT_LASTSLOT |
			 MCASP_TX_STAT_UNDERRUN;
	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_STATUS_SET, &status) < 0)
    {
        perror("+++ McASP: Cannot set TX status");
        return false;
    }

	return true;
}

bool McASP::setRxStatus(uint32_t status) {

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_STATUS_SET, &status) < 0)
    {
        perror("+++ McASP: Cannot set RX status");
        return false;
    }

	return true;
}

bool McASP::setFormat() {

	uint32_t mask = 0xffffffff;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_FMT_MASK_SET, &mask) < 0)
    {
        perror("+++ McASP: Cannot set TX format mask");
        return false;
    }

	uint32_t format = 0x000000f0;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_FMT_SET, &format) < 0)
    {
        perror("+++ McASP: Cannot set TX format");
        return false;
    }

	return true;
}

bool McASP::setI2STxFormat() {

	mcasp_fmt_i2s_t format;
	format.wordSize = 24;
	format.slotSize = 32;
	format.txMode   = MCASP_TX_MODE_NON_DMA;
//	format.txMode   = MCASP_TX_MODE_DMA;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_FMT_I2S_SET, &format) < 0)
    {
        perror("+++ McASP: Cannot set I2S TX format");
        return false;
    }

	return true;
}

bool McASP::setI2SRxFormat() {

	mcasp_fmt_i2s_t format;
	format.wordSize = 24;
	format.slotSize = 32;
	format.txMode   = MCASP_TX_MODE_NON_DMA;
//	format.txMode   = MCASP_TX_MODE_DMA;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_FMT_I2S_SET, &format) < 0)
    {
        perror("+++ McASP: Cannot set I2S RX format");
        return false;
    }

	return true;
}

bool McASP::setTxFrameSync() {

	mcasp_frame_sync_t frameSync;
	frameSync.fsMode = 0x02; // 2-slot TDM (I2S mode)
	frameSync.fsWidth = MCASP_TX_FS_WIDTH_WORD;
	frameSync.fsSetting = MCASP_TX_FS_INT_BEGIN_ON_RIS_EDGE;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_FRAME_SYNC_CFG, &frameSync) < 0)
    {
        perror("+++ McASP: Cannot set TX frame sync");
        return false;
    }

	return true;
}

bool McASP::setRxFrameSync() {

	mcasp_frame_sync_t frameSync;
	frameSync.fsMode = 0x02; // 2-slot TDM (I2S mode)
	frameSync.fsWidth = MCASP_RX_FS_WIDTH_WORD;
	frameSync.fsSetting = MCASP_RX_FS_INT_BEGIN_ON_FALL_EDGE;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_FRAME_SYNC_CFG, &frameSync) < 0)
    {
        perror("+++ McASP: Cannot set RX frame sync");
        return false;
    }

	return true;
}

bool McASP::setTxClock() {

//	if(::ioctl(spiFd, MCASP_IOCTL_TX_RX_CLK_SYNC_ENABLE) < 0)
	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_RX_CLK_SYNC_DISABLE) < 0)
	{
        perror("+++ McASP: Cannot configure clocks sync");
        return false;
    }

	mcasp_clk_t clock;
//    clock.clkSrc = MCASP_TX_CLK_INTERNAL;
//    clock.clkSrc    = MCASP_TX_CLK_EXTERNAL;
    clock.clkSrc    = MCASP_TX_CLK_MIXED;
    clock.auxClkDiv = 4;
    clock.mixClkDiv = 4;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_CLK_CFG, &clock) < 0)
    {
        perror("+++ McASP: Cannot configure TX clock");
        return false;
    }

	return true;
}

bool McASP::setRxClock() {

	mcasp_clk_t clock;
//    clock.clkSrc = MCASP_TX_CLK_INTERNAL;
//    clock.clkSrc = MCASP_RX_CLK_EXTERNAL;
    clock.clkSrc    = MCASP_RX_CLK_MIXED;
    clock.auxClkDiv = 4;
    clock.mixClkDiv = 4;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_CLK_CFG, &clock) < 0)
    {
        perror("+++ McASP: Cannot configure RX clock");
        return false;
    }

	return true;
}

bool McASP::setTxClockPolarity() {

	uint32_t polarity = MCASP_TX_CLK_POL_FALL_EDGE;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_CLK_POLARITY_SET, &polarity) < 0)
    {
        perror("+++ McASP: Cannot configure TX clock polarity");
        return false;
    }

	return true;
}

bool McASP::setRxClockPolarity() {

	uint32_t polarity = MCASP_RX_CLK_POL_RIS_EDGE;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_CLK_POLARITY_SET, &polarity) < 0)
    {
        perror("+++ McASP: Cannot configure RX clock polarity");
        return false;
    }

	return true;
}

bool McASP::configTxClockChk() {

	mcasp_clk_check_t clockChk;
    clockChk.boundMin = 0;
    clockChk.boundMax = 255;
    clockChk.clkDiv = MCASP_TX_CLKCHCK_DIV8;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_CLK_CHECK_CONFIG, &clockChk) < 0)
    {
        perror("+++ McASP: Cannot configure TX clockChk");
        return false;
    }

	return true;
}

bool McASP::configRxClockChk() {

	mcasp_clk_check_t clockChk;
    clockChk.boundMin = 0;
    clockChk.boundMax = 255;
    clockChk.clkDiv = MCASP_RX_CLKCHCK_DIV8;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_CLK_CHECK_CONFIG, &clockChk) < 0)
    {
        perror("+++ McASP: Cannot configure RX clockChk");
        return false;
    }

	return true;
}

bool McASP::setTxTimeSlot() {

	uint32_t slot = I2S_SLOTS;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_TIME_SLOT_SET, &slot) < 0)
    {
        perror("+++ McASP: Cannot set TX slots");
        return false;
    }

	return true;
}

bool McASP::setRxTimeSlot() {

	uint32_t slot = I2S_SLOTS;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_TIME_SLOT_SET, &slot) < 0)
    {
        perror("+++ McASP: Cannot set RX slots");
        return false;
    }

	return true;
}

bool McASP::startTxClock() {

//	uint32_t clockSrc = MCASP_TX_CLK_INTERNAL;
	uint32_t clockSrc = MCASP_RX_CLK_MIXED;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_CLK_START, &clockSrc) < 0)
    {
        perror("+++ McASP: Cannot start TX clock");
        return false;
    }

	return true;
}

bool McASP::startRxClock() {

//	uint32_t clockSrc = MCASP_RX_CLK_INTERNAL;
	uint32_t clockSrc = MCASP_RX_CLK_MIXED;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_CLK_START, &clockSrc) < 0)
    {
        perror("+++ McASP: Cannot start RX clock");
        return false;
    }

	return true;
}

bool McASP::setTxSerializer() {

    uint32_t serNum = MCASP_XSER_TX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_SERIALIZER_TX_SET, &serNum) < 0)
    {
        perror("+++ McASP: Cannot set TX serializer");
        return false;
    }

	return true;
}

bool McASP::setRxSerializer() {

    uint32_t serNum = MCASP_XSER_RX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_SERIALIZER_RX_SET, &serNum) < 0)
    {
        perror("+++ McASP: Cannot set RX serializer");
        return false;
    }

	return true;
}

uint32_t McASP::getTxSerializer() {

	mcasp_buf_t buf;
	buf.serNum = MCASP_XSER_TX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_SERIALIZER_GET, &buf) < 0)
    {
        perror("+++ McASP: Cannot get TX serializer");
        return false;
    }

	return buf.data;
}

uint32_t McASP::getRxSerializer() {

	mcasp_buf_t buf;
	buf.serNum = MCASP_XSER_RX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_SERIALIZER_GET, &buf) < 0)
    {
        perror("+++ McASP: Cannot get RX serializer");
        return false;
    }

	return buf.data;
}

uint32_t McASP::getSerializer(uint32_t i) {

	mcasp_buf_t buf;
	buf.serNum = i;

	if(::ioctl(mcaspFd, MCASP_IOCTL_SERIALIZER_GET, &buf) < 0)
    {
        perror("+++ McASP: Cannot get serializer");
        return false;
    }

	return buf.data;
}

bool McASP::setMcAspPin() {

	uint32_t pinMask =
	MCASP_PIN_AFSR |
	MCASP_PIN_AHCLKR |
	MCASP_PIN_ACLKR |
	MCASP_PIN_AFSX |
	MCASP_PIN_AHCLKX |
	MCASP_PIN_ACLKX |
	MCASP_PIN_AXR(MCASP_XSER_TX) |
	MCASP_PIN_AXR(MCASP_XSER_RX);

	if(::ioctl(mcaspFd, MCASP_IOCTL_PIN_MCASP_SET, &pinMask) < 0)
    {
        perror("+++ McASP: Cannot set McASP pins");
        return false;
    }

	return true;
}

bool McASP::setGpioPin() {

	uint32_t pinMask =
			MCASP_PIN_AFSR |
			MCASP_PIN_AHCLKR |
			MCASP_PIN_ACLKR |
			MCASP_PIN_AXR(MCASP_XSER_TX);

	if(::ioctl(mcaspFd, MCASP_IOCTL_PIN_GPIO_SET, &pinMask) < 0)
    {
        perror("+++ McASP: Cannot set GPIO pins");
        return false;
    }

	return true;
}

bool McASP::setPinDirIn() {

	uint32_t pinMask =
	MCASP_PIN_AFSR |
	MCASP_PIN_AHCLKR |
	MCASP_PIN_ACLKR |
	MCASP_PIN_AFSX |
	MCASP_PIN_AHCLKX |
	MCASP_PIN_ACLKX |
	MCASP_PIN_AXR(MCASP_XSER_RX);

	if(::ioctl(mcaspFd, MCASP_IOCTL_PIN_DIR_INPUT_SET, &pinMask) < 0)
	{
	    perror("+++ McASP: Cannot set input pins");
	    return false;
	}
	return true;
}

bool McASP::setPinDirOut() {

	uint32_t pinMask =
	MCASP_PIN_AFSR |
	MCASP_PIN_AHCLKR |
	MCASP_PIN_ACLKR |
	MCASP_PIN_AFSX |
//	MCASP_PIN_AHCLKX |
	MCASP_PIN_ACLKX |
	MCASP_PIN_AXR(MCASP_XSER_TX);

	if(::ioctl(mcaspFd, MCASP_IOCTL_PIN_DIR_OUTPUT_SET, &pinMask) < 0)
	{
	    perror("+++ McASP: Cannot set output pins");
	    return false;
	}
	return true;
}

bool McASP::activateTxSerializer() {

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_SER_ACTIVATE) < 0)
    {
        perror("+++ McASP: Cannot activate TX serializer");
        return false;
    }

	return true;
}

bool McASP::activateRxSerializer() {

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_SER_ACTIVATE) < 0)
    {
        perror("+++ McASP: Cannot activate RX serializer");
        return false;
    }

	return true;
}

bool McASP::enableTx() {
	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_ENABLE) < 0)
    {
        perror("+++ McASP: Cannot enable TX");
        return false;
    }

	return true;
}

bool McASP::enableRx() {
	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_ENABLE) < 0)
    {
        perror("+++ McASP: Cannot enable RX");
        return false;
    }

	return true;
}

uint32_t McASP::readData() {

	mcasp_buf_t buf;
	buf.serNum = MCASP_XSER_RX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_RX_BUF_READ, &buf) < 0)
    {
        perror("+++ McASP: Cannot read data");
        return 0;
    }

	return buf.data;
}

bool McASP::writeData(uint32_t data) {

	mcasp_buf_t buf;
	buf.data = data;
	buf.serNum = MCASP_XSER_TX;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_BUF_WRITE, &buf) < 0)
    {
        perror("+++ McASP: Cannot write data");
        return false;
    }

	return true;
}

bool McASP::enableTxInterrupt() {
	 uint32_t intrMask =
//	          MCASP_TX_STARTOFFRAME |
//	          MCASP_TX_DATAREADY |
//	          MCASP_TX_LASTSLOT |
//	          MCASP_TX_DMAERROR |
//	          MCASP_TX_CLKFAIL |
//	          MCASP_TX_SYNCERROR |
	          MCASP_TX_UNDERRUN;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_INT_ENABLE, &intrMask) < 0)
	{
	    perror("+++ McASP: Cannot enable TX interrupt");
	    return false;
	}
	return true;
}

bool McASP::disableTxInterrupt() {
	 uint32_t intrMask =
	          MCASP_TX_STARTOFFRAME |
	          MCASP_TX_DATAREADY |
	          MCASP_TX_LASTSLOT |
	          MCASP_TX_DMAERROR |
	          MCASP_TX_CLKFAIL |
	          MCASP_TX_SYNCERROR |
	          MCASP_TX_UNDERRUN;

	if(::ioctl(mcaspFd, MCASP_IOCTL_TX_INT_DISABLE, &intrMask) < 0)
	{
	    perror("+++ McASP: Cannot enable TX interrupt");
	    return false;
	}
	return true;
}

bool McASP::setDout(uint32_t data) {
	if(::ioctl(mcaspFd, MCASP_IOCTL_DOUT_SET, &data) < 0)
    {
        perror("+++ McASP: Cannot set DOUT");
        return false;
    }

	return true;
}

bool McASP::getDin(uint32_t& data) {
	if(::ioctl(mcaspFd, MCASP_IOCTL_DIN_GET, &data) < 0)
    {
        perror("+++ McASP: Cannot get DIN");
        return false;
    }

	return true;
}

bool McASP::enableTransfers() {
	if(::ioctl(mcaspFd, MCASP_IOCTL_ACTIVATE_TXRX) < 0)
    {
        perror("+++ McASP: Cannot enable transfers");
        return false;
    }

	return true;
}

bool McASP::runTest() {
	if(::ioctl(mcaspFd, MCASP_IOCTL_RUN_TEST) < 0)
    {
        perror("+++ McASP: Cannot run the test");
        return false;
    }

	return true;
}

size_t McASP::read(void* const* buffs, const size_t numElems, int numBufs) {

	if(__builtin_expect((numElems == 0), 0))
		return 0;

	if(available == 0) {
	  dataLen = ::read(mcaspFd, &buf[0], sizeof(buf));
	  available = dataLen;
	  offset = 0;
	}


	size_t chAvail = available/sizeof(int32_t)/numChannels;
	size_t nToCopy = ((chAvail>numElems) ? numElems : chAvail);

	if (__builtin_expect(nToCopy != 0, 1)) {
		for (int j = 0; j < nToCopy * numChannels; j++, j++) {
			for (int i = 0; i < numChannels; ++i) {
				((int32_t*) buffs[i])[j / 2] = ((int32_t*) (buf + offset))[i + j];
			}
		}

		offset += nToCopy * numChannels;
		available -= nToCopy * sizeof(int32_t) * numChannels;
	}

	return nToCopy;
}

