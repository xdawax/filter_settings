#include "fir.h"

#define TAPS					101												// The number of taps in the FIR filter
arm_fir_instance_f32 fir_lp;
arm_fir_instance_f32 fir_hp;
arm_fir_instance_f32 fir_notch;

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

const float32_t firCoeffs_HP_5kHz[TAPS] = { 0.00030992,0.00016888,-3.5982e-18,-0.00019182,-0.00039773,-0.00060271,-0.00078446,-0.00091398,-0.0009582,-0.00088462,-0.00066761,-0.00029536,0.0002237,
																						0.00085622,0.0015424,0.0021984,0.0027234,0.003011,0.0029635,0.0025085,0.0016148,0.00030542,-0.0013347,-0.0031581,-0.0049625,-0.0065082,-0.0075422,-0.0078294,
																						-0.0071853,-0.0055087,-0.0028089,0.00077694,0.0049779,0.009399,0.013545,0.01686,0.018775,0.018767,0.016412,0.011443,0.0037884,-0.0064034,-0.018755,-0.032679,
																						-0.047412,-0.062072,-0.075727,-0.08747,-0.0965,-0.10219,0.8955,-0.10219,-0.0965,-0.08747,-0.075727,-0.062072,-0.047412,-0.032679,-0.018755,-0.0064034,0.0037884,
																						0.011443,0.016412,0.018767,0.018775,0.01686,0.013545,0.009399,0.0049779,0.00077694,-0.0028089,-0.0055087,-0.0071853,-0.0078294,-0.0075422,-0.0065082,-0.0049625,
																						-0.0031581,-0.0013347,0.00030542,0.0016148,0.0025085,0.0029635,0.003011,0.0027234,0.0021984,0.0015424,0.00085622,0.0002237,-0.00029536,-0.00066761,-0.00088462,
																						-0.0009582,-0.00091398,-0.00078446,-0.00060271,-0.00039773,-0.00019182,-3.5982e-18,0.00016888,0.00030992
																					};

const float32_t firCoeffs_NOTCH_3000Hz[TAPS] = { 0.00086578,0.0009632,0.0010504,0.0011223,0.0011694,0.0011777,0.0011301,0.0010068,0.00078832,0.00045698,-7.3302e-18,-0.00058801,-0.0013027,-0.002128,-0.0030351,
																								-0.0039825,-0.0049165,-0.0057738,-0.0064841,-0.0069749,-0.0071761,-0.0070253,-0.0064734,-0.0054898,-0.0040664,-0.0022212,-1.6215e-17,0.0025238,0.0052508,0.0080594,
																								 0.010811,0.013358,0.01555,0.017245,0.018316,0.018662,0.018213,0.016937,0.014845,0.01199,0.0084684,0.0044157,-3.2583e-17,-0.0045859,-0.0091342,-0.013433,-0.017278,
																								-0.020483,-0.022893,-0.024388,0.97091,-0.024388,-0.022893,-0.020483,-0.017278,-0.013433,-0.0091342,-0.0045859,-3.2583e-17,0.0044157,0.0084684,0.01199,0.014845,0.016937,
																								 0.018213,0.018662,0.018316,0.017245,0.01555,0.013358,0.010811,0.0080594,0.0052508,0.0025238,-1.6215e-17,-0.0022212,-0.0040664,-0.0054898,-0.0064734,-0.0070253,-0.0071761,
																								-0.0069749,-0.0064841,-0.0057738,-0.0049165,-0.0039825,-0.0030351,-0.002128,-0.0013027,-0.00058801,-7.3302e-18,0.00045698,0.00078832,0.0010068,0.0011301,0.0011777,0.0011694,
																								 0.0011223,0.0010504,0.0009632,0.00086578
																							 };
																				
																					
volatile float32_t fir_lp_state[TAPS + BLOCK_SIZE - 1];
volatile float32_t fir_hp_state[TAPS + BLOCK_SIZE - 1];
volatile float32_t fir_notch_state[TAPS + BLOCK_SIZE - 1];

void fir_filter_signal(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength, arm_fir_instance_f32 filter);

void fir_init(void) {
	arm_fir_init_f32(&fir_lp, TAPS, (float32_t *)&firCoeffs_LP_5kHz[0], (float32_t *)&fir_lp_state[0], BLOCK_SIZE); 
	arm_fir_init_f32(&fir_hp, TAPS, (float32_t *)&firCoeffs_HP_5kHz[0], (float32_t *)&fir_hp_state[0], BLOCK_SIZE);
	arm_fir_init_f32(&fir_notch, TAPS, (float32_t *)&firCoeffs_NOTCH_3000Hz[0], (float32_t *)&fir_notch_state[0], BLOCK_SIZE);
}
			

void fir_lp_filter(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength) {
	fir_filter_signal(unfilteredSignal, filteredSignal, signalLength, fir_lp);
}

void fir_hp_filter(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength) {
	fir_filter_signal(unfilteredSignal, filteredSignal, signalLength, fir_hp);
}

void fir_notch_filter(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength) {
	fir_filter_signal(unfilteredSignal, filteredSignal, signalLength, fir_notch);
}

void fir_filter_signal(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength, arm_fir_instance_f32 filter) {
	int i = 0;
	uint32_t numBlocks = signalLength / BLOCK_SIZE;
	
	
	for (i = 0; i < numBlocks; i++) {
		arm_fir_f32(&filter, unfilteredSignal + (i * BLOCK_SIZE), filteredSignal + (i * BLOCK_SIZE), BLOCK_SIZE);
	}
}


