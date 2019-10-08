#include "fir.h"

#define TAPS					101												// The number of taps in the FIR filter
arm_fir_instance_f32 fir_lp;

// Coefficients generated in MatLab
const float32_t firCoeffs_LP_5kHz[TAPS] = { -0.000309531785296974, -0.000168667396354186, 3.39020149475193e-19,	0.000191580488973905, 0.000397225781615615,	0.000601942766960649,
																						 0.000783465527039160,	0.000912825475234056,	0.000956983042844503,	0.000883496893850959,	0.000666768612907606,	0.000294986928352886,	
																						-0.000223416789531796, -0.000855133303977255,	-0.0015404206803902,	-0.00219558911055248,	-0.00271997559986430,	-0.00300719835647546,
																						-0.00295975576727258, -0.00250535108560389,	-0.00161279174888754,	-0.000305030310868361, 0.00133304629034695,	0.00315407974503938,
																						 0.00495622748610603, 0.00649994719467331,	0.00753269432313480, 0.00781947425338200,	0.00717615800017595, 0.00550173092857441,	
																						0.00280532299070886, -0.000775953797538112,	-0.00497163908000145,	-0.00938713848945521,	-0.0135280953023939,	-0.0168387631503456,
																						-0.0187514894883213, -0.0187428961702717,	-0.0163912288115399,	-0.0114287993050265,	-0.00378356356065284,	0.00639531893671674,	
																						0.0187316656972300,	0.0326379928784657,	0.0473523693729736,	0.0619937386270715,	0.0756309963637119,	0.0873595124824335,	0.0963778250094682,
																						0.102057035459590,	0.103996035066078,	0.102057035459590,	0.0963778250094682,	0.0873595124824335,	0.0756309963637119,	0.0619937386270715,
																						0.0473523693729736,	0.0326379928784657,	0.0187316656972300,	0.00639531893671674,	-0.00378356356065284,	-0.0114287993050265,	-0.0163912288115399,
																						-0.0187428961702717,	-0.0187514894883213,	-0.0168387631503456,	-0.0135280953023939,	-0.00938713848945521,	-0.00497163908000145,	-0.000775953797538112,
																						0.00280532299070886,	0.00550173092857441,	0.00717615800017595,	0.00781947425338200,	0.00753269432313480,	0.00649994719467331,	0.00495622748610603,
																						0.00315407974503938,	0.00133304629034695,	-0.000305030310868361,	-0.00161279174888754,	-0.00250535108560389,	-0.00295975576727258,	-0.00300719835647546,
																						-0.00271997559986430,	-0.00219558911055248,	-0.00154042068039020,	-0.000855133303977255,	-0.000223416789531796,	0.000294986928352886,	0.000666768612907606,
																						0.000883496893850959,	0.000956983042844503,	0.000912825475234056,	0.000783465527039160,	0.000601942766960649,	0.000397225781615615,	0.000191580488973905,
																						3.39020149475193e-19,	-0.000168667396354186,	-0.000309531785296974
																					};

volatile float32_t fir_lp_state[TAPS + BLOCK_SIZE - 1];

void fir_init() {
	arm_fir_init_f32(&fir_lp, TAPS, (float32_t *)&firCoeffs_LP_5kHz[0], (float32_t *)&fir_lp_state[0], BLOCK_SIZE); 
}
																					
void fir_lp_filter(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength) {
	int i = 0;
	uint32_t numBlocks = signalLength / BLOCK_SIZE;
	
	for (i = 0; i < numBlocks; i++) {
		arm_fir_f32(&fir_lp, &unfilteredSignal[i * BLOCK_SIZE], &filteredSignal[i * BLOCK_SIZE], BLOCK_SIZE);
	}
}


