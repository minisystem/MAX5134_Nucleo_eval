/*
 * calibration.c
 *
 *  Created on: Jun 26, 2017
 *      Author: omar
 *      This file has the text between the lines
 *      // PYTHON REPLACE HERE
 *      and
 *      // END PYTHON REPLACE HERE
 *      replaced by code generated in generate_calibration_arrays.py
 */
#include "system.h"

volatile struct ChannelStruct channel[NUM_CHANNELS];

void setupCalibration() {
// PYTHON REPLACE HERE
channel[0].zero_offset = 3124;
channel[0].code_interval = 4749;
channel[0].offset[0] = 33;
channel[0].offset[1] = 19;
channel[0].offset[2] = 17;
channel[0].offset[3] = 28;
channel[0].offset[4] = 42;
channel[0].offset[5] = 43;
channel[0].offset[6] = 28;
channel[0].offset[7] = 14;
channel[0].offset[8] = 5;
channel[0].offset[9] = 14;
channel[0].offset[10] = 28;
channel[1].zero_offset = 3124;
channel[1].code_interval = 4749;
channel[1].offset[0] = 33;
channel[1].offset[1] = 19;
channel[1].offset[2] = 17;
channel[1].offset[3] = 28;
channel[1].offset[4] = 42;
channel[1].offset[5] = 43;
channel[1].offset[6] = 28;
channel[1].offset[7] = 14;
channel[1].offset[8] = 5;
channel[1].offset[9] = 14;
channel[1].offset[10] = 28;
channel[2].zero_offset = 3124;
channel[2].code_interval = 4749;
channel[2].offset[0] = 33;
channel[2].offset[1] = 19;
channel[2].offset[2] = 17;
channel[2].offset[3] = 28;
channel[2].offset[4] = 42;
channel[2].offset[5] = 43;
channel[2].offset[6] = 28;
channel[2].offset[7] = 14;
channel[2].offset[8] = 5;
channel[2].offset[9] = 14;
channel[2].offset[10] = 28;
channel[3].zero_offset = 3124;
channel[3].code_interval = 4749;
channel[3].offset[0] = 33;
channel[3].offset[1] = 19;
channel[3].offset[2] = 17;
channel[3].offset[3] = 28;
channel[3].offset[4] = 42;
channel[3].offset[5] = 43;
channel[3].offset[6] = 28;
channel[3].offset[7] = 14;
channel[3].offset[8] = 5;
channel[3].offset[9] = 14;
channel[3].offset[10] = 28;
// END PYTHON REPLACE HERE
}
