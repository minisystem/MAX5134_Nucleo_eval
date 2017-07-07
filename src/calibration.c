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
channel[0].zero_offset = 3119;
channel[0].code_interval = 4748;
channel[0].pitch_table[0] = 3158;
channel[0].pitch_table[1] = 7889;
channel[0].pitch_table[2] = 12635;
channel[0].pitch_table[3] = 17395;
channel[0].pitch_table[4] = 22156;
channel[0].pitch_table[5] = 26906;
channel[0].pitch_table[6] = 31638;
channel[0].pitch_table[7] = 36365;
channel[0].pitch_table[8] = 41105;
channel[0].pitch_table[9] = 45868;
channel[0].pitch_table[10] = 50627;
channel[1].zero_offset = 3194;
channel[1].code_interval = 4740;
channel[1].pitch_table[0] = 3213;
channel[1].pitch_table[1] = 7950;
channel[1].pitch_table[2] = 12687;
channel[1].pitch_table[3] = 17425;
channel[1].pitch_table[4] = 22162;
channel[1].pitch_table[5] = 26899;
channel[1].pitch_table[6] = 31636;
channel[1].pitch_table[7] = 36370;
channel[1].pitch_table[8] = 41109;
channel[1].pitch_table[9] = 45849;
channel[1].pitch_table[10] = 50588;
channel[2].zero_offset = 3199;
channel[2].code_interval = 4746;
channel[2].pitch_table[0] = 3228;
channel[2].pitch_table[1] = 7976;
channel[2].pitch_table[2] = 12718;
channel[2].pitch_table[3] = 17458;
channel[2].pitch_table[4] = 22197;
channel[2].pitch_table[5] = 26939;
channel[2].pitch_table[6] = 31686;
channel[2].pitch_table[7] = 36429;
channel[2].pitch_table[8] = 41175;
channel[2].pitch_table[9] = 45916;
channel[2].pitch_table[10] = 50653;
channel[3].zero_offset = 3313;
channel[3].code_interval = 0;
channel[3].pitch_table[0] = 3325;
channel[3].pitch_table[1] = 8038;
channel[3].pitch_table[2] = 12755;
channel[3].pitch_table[3] = 17474;
channel[3].pitch_table[4] = 22193;
channel[3].pitch_table[5] = 26906;
channel[3].pitch_table[6] = 31620;
channel[3].pitch_table[7] = 36327;
channel[3].pitch_table[8] = 41043;
channel[3].pitch_table[9] = 45763;
channel[3].pitch_table[10] = 50483;
// END PYTHON REPLACE HERE
}
