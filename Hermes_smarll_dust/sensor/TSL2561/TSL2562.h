#ifndef __TSL2561_2_H
#define	__TSL2561_2_H

#define LUX_SCALE_2 14           // scale by 2^14
#define RATIO_SCALE_2 9          // scale ratio by 2^9
#define CH_SCALE_2 10            // scale channel values by 2^10
#define CHSCALE_TINT0_2 0x7517   // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1_2 0x0fe7   // 322/81 * 2^CH_SCALE

#define K1T_2 0x0040   // 0.125 * 2^RATIO_SCALE
#define B1T_2 0x01f2   // 0.0304 * 2^LUX_SCALE
#define M1T_2 0x01be   // 0.0272 * 2^LUX_SCALE
#define K2T_2 0x0080   // 0.250 * 2^RATIO_SCA
#define B2T_2 0x0214   // 0.0325 * 2^LUX_SCALE
#define M2T_2 0x02d1   // 0.0440 * 2^LUX_SCALE
#define K3T_2 0x00c0   // 0.375 * 2^RATIO_SCALE
#define B3T_2 0x023f   // 0.0351 * 2^LUX_SCALE
#define M3T_2 0x037b   // 0.0544 * 2^LUX_SCALE
#define K4T_2 0x0100   // 0.50 * 2^RATIO_SCALE
#define B4T_2 0x0270   // 0.0381 * 2^LUX_SCALE
#define M4T_2 0x03fe   // 0.0624 * 2^LUX_SCALE
#define K5T_2 0x0138   // 0.61 * 2^RATIO_SCALE
#define B5T_2 0x016f   // 0.0224 * 2^LUX_SCALE
#define M5T_2 0x01fc   // 0.0310 * 2^LUX_SCALE
#define K6T_2 0x019a   // 0.80 * 2^RATIO_SCALE
#define B6T_2 0x00d2   // 0.0128 * 2^LUX_SCALE
#define M6T_2 0x00fb   // 0.0153 * 2^LUX_SCALE
#define K7T_2 0x029a   // 1.3 * 2^RATIO_SCALE
#define B7T_2 0x0018   // 0.00146 * 2^LUX_SCALE
#define M7T_2 0x0012   // 0.00112 * 2^LUX_SCALE
#define K8T_2 0x029a   // 1.3 * 2^RATIO_SCALE
#define B8T_2 0x0000   // 0.000 * 2^LUX_SCALE
#define M8T_2 0x0000   // 0.000 * 2^LUX_SCALE

#define K1C_2 0x0043   // 0.130 * 2^RATIO_SCALE
#define B1C_2 0x0204   // 0.0315 * 2^LUX_SCALE
#define M1C_2 0x01ad   // 0.0262 * 2^LUX_SCALE
#define K2C_2 0x0085   // 0.260 * 2^RATIO_SCALE
#define B2C_2 0x0228   // 0.0337 * 2^LUX_SCALE
#define M2C_2 0x02c1   // 0.0430 * 2^LUX_SCALE
#define K3C_2 0x00c8   // 0.390 * 2^RATIO_SCALE
#define B3C_2 0x0253   // 0.0363 * 2^LUX_SCALE
#define M3C_2 0x0363   // 0.0529 * 2^LUX_SCALE
#define K4C_2 0x010a   // 0.520 * 2^RATIO_SCALE
#define B4C_2 0x0282   // 0.0392 * 2^LUX_SCALE
#define M4C_2 0x03df   // 0.0605 * 2^LUX_SCALE
#define K5C_2 0x014d   // 0.65 * 2^RATIO_SCALE
#define B5C_2 0x0177   // 0.0229 * 2^LUX_SCALE
#define M5C_2 0x01dd   // 0.0291 * 2^LUX_SCALE
#define K6C_2 0x019a   // 0.80 * 2^RATIO_SCALE
#define B6C_2 0x0101   // 0.0157 * 2^LUX_SCALE
#define M6C_2 0x0127   // 0.0180 * 2^LUX_SCALE
#define K7C_2 0x029a   // 1.3 * 2^RATIO_SCALE
#define B7C_2 0x0037   // 0.00338 * 2^LUX_SCALE
#define M7C_2 0x002b   // 0.00260 * 2^LUX_SCALE
#define K8C_2 0x029a   // 1.3 * 2^RATIO_SCALE
#define B8C_2 0x0000   // 0.000 * 2^LUX_SCALE
#define M8C_2 0x0000   // 0.000 * 2^LUX_SCALE

void TSL2561_INIT_2(void);
void TSL2561_Read_2(void);
void GZ_Calculate_2(void);
int TSL2561_ReturnL2(void);
unsigned long TSL2561_CalculateLux_2(unsigned int iGain, unsigned int tInt,int iType);

#endif


