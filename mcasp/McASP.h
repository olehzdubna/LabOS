#ifndef __McASP_H__
#define __McASP_H__

class McASP
{
public:
    McASP();
    ~McASP();

    bool initialize();
    uint32_t getTxStatus();
    uint32_t getRxStatus();
    uint32_t getGlobalStatus();
    bool setTxStatus(uint32_t status);
    bool setRxStatus(uint32_t status);
    bool setFormat();
    bool setI2STxFormat();
    bool setI2SRxFormat();
    bool setTxFrameSync();
    bool setRxFrameSync();
    bool setTxTimeSlot();
    bool setRxTimeSlot();
    bool setTxClock();
    bool setRxClock();
    bool setTxClockPolarity();
    bool setRxClockPolarity();
    bool configTxClockChk();
    bool configRxClockChk();
    bool startTxClock();
    bool startRxClock();
    bool setTxSerializer();
    bool setRxSerializer();
    uint32_t getTxSerializer();
    uint32_t getRxSerializer();
    uint32_t getSerializer(uint32_t i);
    bool activateTxSerializer();
    bool activateRxSerializer();
    bool setMcAspPin();
    bool setGpioPin();
    bool setPinDirIn();
    bool setPinDirOut();
    bool enableTx();
    bool enableRx();
    uint32_t readData();
    bool writeData(uint32_t data);
    bool enableTxInterrupt();
    bool disableTxInterrupt();

    bool setDout(uint32_t data);
    bool getDin(uint32_t& data);

    bool enableTransfers();
    bool runTest();

    size_t read(void* const* buffs, const size_t numElems, int numBufs);

private:
    int mcaspFd;
    size_t available;
    size_t offset;
    size_t dataLen;
    int numChannels;
    const static int BUF_SIZE = 16768;
    int32_t buf[BUF_SIZE];
};

#endif // __McASP_H__
