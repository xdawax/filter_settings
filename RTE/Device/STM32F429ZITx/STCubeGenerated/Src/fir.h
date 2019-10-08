#ifndef __FIR_H_
#define __FIR_H_

#include "arm_math.h"

#define BUFFER_SIZE 	256												// Full buffer size
#define DATA_SIZE		 	BUFFER_SIZE / 2						// Half buffer size that user can access
#define BLOCK_SIZE		32												// The size of each block to be filtered

/// \brief Initializes the FIR filters, LP (to add HP, BP, NOTCH)
/// 
/// \param void
///
/// \returns void
void fir_init(void);		

/// \brief
/// 
/// \param[in] *unfilteredSignal is a pointer to the beginning of the noisy signal to be filtered
/// \param[out] *filteredSignal is a pointer to the beginning of the filtered signal
/// \param[in] signalLength is the number of samples to be filtered
///
/// \returns void
void fir_lp_filter(float32_t *unfilteredSignal, float32_t *filteredSignal, uint32_t signalLength);




#endif

