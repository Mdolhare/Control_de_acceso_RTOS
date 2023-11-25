#include "card_app.h"
#include "CardReaderDrv.h"

uint64_t convertToID2(uint32_t* buff) {
	uint64_t ids = 0;
	int32_t j = SIMBOL_SIZE;
	uint8_t simbol = 0;
	for(j = SIMBOL_SIZE; j < 50 ;j+=SIMBOL_SIZE) {
		ids *= 10;
		ids += simbol;
		simbol = 0;
		for (int i=0; i<SIMBOL_SIZE-1; i++) {
			simbol += (!buff[j+i])<<i;
		}
	}
	return ids;
}






