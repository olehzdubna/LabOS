/**
 * @brief 
 * @param argc
 * @param argv
 * @return 
 */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "McASP.h"
#include "mcasp_hw.h"
#include "mcasp_api_defs.h"

int mcaspTest2()
{
    McASP mcasp;
    std::cout << "McASP test 2" << std::endl;

    mcasp.initialize();
    printf("+++ McASP was initialized\n");

//    mcasp.setMcAspPin();
//    printf("+++ McASP configured McASP pins\n");
//
//    mcasp.setPinDirIn();
//    printf("+++ McASP configured McASP pins as inputs\n");
//
//    mcasp.setPinDirOut();
//    printf("+++ McASP configured McASP pins as outputs\n");
//

    mcasp.enableTransfers();
    printf("+++ McASP transfers enabled\n");

    std::ofstream ofs;

    ofs.open("/appl/test_data.out");

    if(!ofs.is_open()) {
        printf("+++ McASP cannot open log file\n");
        return -1;
    }

    char buf1[65536];
    char buf2[65536];
    std::vector<void *> buffs(2);
    buffs[0] = buf1;
    buffs[1] = buf2;

    for(int i = 0; i < 100; i++) {

    	mcasp.runTest();

//  	printf("+++ McASP serializer, TX: %8.8x, RX: %8.8x\n", mcasp.getTxSerializer(), mcasp.getRxSerializer());
//
//    	printf("***********************************************************************************************\n");

    	struct timespec ts1, ts2;
    	clock_gettime(CLOCK_MONOTONIC, &ts1);
        int nToCopy = mcasp.read(buffs.data(), (uint64_t)200, 2);
    	clock_gettime(CLOCK_MONOTONIC, &ts2);

    	printf("+++ time delta: %lld\n", (ts2.tv_sec - ts1.tv_sec)*1000000000 + ts2.tv_nsec - ts1.tv_nsec);

  	    for(int i=0; i < nToCopy; ++i) {
  		  ofs.write((char*) &((int32_t*)buffs[0])[i], sizeof(uint32_t));
  	    }

//
//    	ser = mcasp.getTxSerializer();
//
//    	if(ser & 0x10) {
//
//		uint32_t txStatus = mcasp.getTxStatus();
//
//		  mcasp.setTxStatus(0);
//
//		printf("+++ McASP status, TX: %8.8x, RX: %8.8x, Global: %8.8x\n", txStatus, mcasp.getRxStatus(), mcasp.getGlobalStatus());
//
//       	  if(txStatus & 0x01) {
//     	 mcasp.setTxStatus(0xffffffff);
//       	  }
//    	}
//    }

//       usleep(10000);
    }

	return 0;
}

int mcaspTest3()
{
    McASP mcasp;
    std::cout << "McASP test 3" << std::endl;

    mcasp.initialize();
    printf("+++ McASP was initialized\n");

    mcasp.setGpioPin();

    for(int i = 0; i < 100000; i++) {
    	mcasp.setDout((i&1)?0xffffffff:0x00000000);
        usleep(10000);
     }

 	return 0;
}

int mcaspTest4()
{
    McASP mcasp;
    std::cout << "McASP test 4" << std::endl;

    mcasp.initialize();
    printf("+++ McASP was initialized\n");

    mcasp.setGpioPin();

    for(int i = 0; i < 100000; i++) {
    	uint32_t data = 0;
    	mcasp.getDin(data);
        usleep(100000);
        printf("McASP data: %8.8x\n", data);
     }

 	return 0;
}


int mcaspTest()
{
    McASP mcasp;
    
    std::cout << "McASP test" << std::endl;

    mcasp.initialize();
    printf("+++ McASP was initialized\n");

    mcasp.setI2STxFormat();
    printf("+++ McASP I2S TX format was set\n");

    mcasp.setI2SRxFormat();
    printf("+++ McASP I2S RX format was set\n");

    mcasp.setTxFrameSync();
    printf("+++ McASP TX frame sync was set\n");

    mcasp.setRxFrameSync();
    printf("+++ McASP RX frame sync was set\n");

    mcasp.setTxClock();
    printf("+++ McASP TX clock was set\n");

    mcasp.setRxClock();
    printf("+++ McASP RX clock was set\n");

    mcasp.setTxClockPolarity();
    printf("+++ McASP TX clock polarity was set\n");

	mcasp.setRxClockPolarity();
    printf("+++ McASP RX clock polarity was set\n");

    mcasp.configTxClockChk();
    printf("+++ McASP configured TX clock check\n");

    mcasp.configRxClockChk();
    printf("+++ McASP configured RX clock check\n");

    mcasp.setTxTimeSlot();
    printf("+++ McASP TX time slot set\n");

    mcasp.setRxTimeSlot();
    printf("+++ McASP RX time slot set\n");

    mcasp.setMcAspPin();
    printf("+++ McASP configured McASP pins\n");

    mcasp.setPinDirIn();
    printf("+++ McASP configured McASP pins as inputs\n");

    mcasp.setPinDirOut();
    printf("+++ McASP configured McASP pins as outputs\n");

    mcasp.setTxSerializer();
    printf("+++ McASP TX serializer set\n");

    mcasp.setRxSerializer();
    printf("+++ McASP RX serializer set\n");

    mcasp.startTxClock();
    printf("+++ McASP TX clock started\n");

    mcasp.startRxClock();
    printf("+++ McASP RX clock started\n");

    mcasp.enableTxInterrupt();

    mcasp.activateTxSerializer();
    printf("+++ McASP activated TX serializer\n");

//    mcasp.activateRxSerializer();
//    printf("+++ McASP activated RX serializer\n");

    uint32_t txStatus = 0;


    while(1) {
    	txStatus = mcasp.getTxStatus();
		printf("+++ McASP status, TX: %8.8x\n", txStatus);
     	if(!(txStatus & MCASP_TX_DATAREADY))
     		break;
    }

    mcasp.enableTx();
    printf("+++ McASP enabled TX\n");

//    mcasp.enableRx();
//    printf("+++ McASP enabled RX\n");


//    uint32_t ser = 0;

//    uint32_t data = 0xaa555a5a;
    for(int i = 0; i < 1000; i++) {
    	printf("+++ McASP serializer, TX: %8.8x, RX: %8.8x\n", mcasp.getTxSerializer(), mcasp.getRxSerializer());
//
//    	ser = mcasp.getTxSerializer();
//
//    	if(ser & 0x10) {
//    	  mcasp.writeData(data);
//
		  txStatus = mcasp.getTxStatus();
//
//		  mcasp.setTxStatus(0);
//
		  printf("+++ McASP status, TX: %8.8x, RX: %8.8x, Global: %8.8x\n", txStatus, mcasp.getRxStatus(), mcasp.getGlobalStatus());
//
//       	  if(txStatus & 0x01) {
//     	 mcasp.setTxStatus(0xffffffff);
//       	  }
//    	}
//    }

//       usleep(100000);
    }

    mcasp.disableTxInterrupt();

    return 0;
}

