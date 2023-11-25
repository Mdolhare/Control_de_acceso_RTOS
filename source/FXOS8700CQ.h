#ifndef FXOS8700CQ_H_
#define FXOS8700CQ_H_

//arreglo de 4 slave
#define CANT_SLAVE 4
#define FXOS8700CQ_SLAVE_ADDR0 (0x1E) //SA0 = 0, SA1 = 0
#define FXOS8700CQ_SLAVE_ADDR1 (0x1D) //SA0 = 1, SA1 = 0
#define FXOS8700CQ_SLAVE_ADDR2 (0x1C) //SA0 = 0, SA1 = 1
#define FXOS8700CQ_SLAVE_ADDR3 (0x1F) //SA0 = 1, SA1 = 1

//-------------------------------------REGISTERS-------------
#define FXOS8700CQ_WHO_AM_I 0x0D

//Data registers accel
#define FXOS8700CQ_DR_STATUS 0x00
#define FXOS8700CQ_OUT_X_MSB 0x01
//auto incremento para otras coordenadas hasta 0x38

//Data registers mag
#define FXOS8700CQ_M_DR_STATUS 0x32
#define FXOS8700CQ_M_OUT_X_MSB 0x33
//auto incremento para otras coordenadas hasta 0x38

//Data register accel hybrid mode
#define FXOS8700CQ_CMP_X_MSB  0x39
//auto incremento para otras coordenadas hasta 0x3E



//control register
//accel
#define FXOS8700CQ_CTRL_REG1  0x2A
#define FXOS8700CQ_CTRL_REG2  0x2B
#define FXOS8700CQ_CTRL_REG3  0x2C
#define FXOS8700CQ_CTRL_REG4  0x2D
#define FXOS8700CQ_CTRL_REG5  0x2E

//mag
#define FXOS8700CQ_M_CTRL_REG1	0x5B
#define FXOS8700CQ_M_CTRL_REG2	0x5C
#define FXOS8700CQ_M_CTRL_REG3	0x5D
#define FXOS8700CQ_M_INT_SRC	0x5E


//accel offset reg
#define FXOS8700CQ_OFF_X  0x2F
#define FXOS8700CQ_OFF_Y  0x30
#define FXOS8700CQ_OFF_Z  0x31

//mag offset reg
#define FXOS8700CQ_M_OFF_X_MSB  0x3F
//auto incremento para otras coordenadas hasta 0x44



//--------------------------------------------------------
//-------------------------------------MACROS-------------
//MACROS para CTRL_REG1
#define FXOS_CR1_DR_SHIFT (0x03)
#define FXOS_CR1_DR(x) ((((uint8_t)(x)) & 0b111)<< FXOS_CR1_DR_SHIFT) //ODR selector
#define FXOS_CR1_F_READ_MASK (0x02)		//fast read mode = 1
#define FXOS_CR1_ACTIVE_MASK (0x01)		//active mode = 1
//DR value for ODR (Output Data Rate) freq.
enum DR_sel_accel_mode {ODR_800,ODR_400,ODR_200,ODR_100,ODR_50,ODR_12_5,ODR_6_25,ODR_1_5625};
enum DR_sel_hybrid_mode {ODRH_400,ODRH_200,ODRH_100,ODRH_50,ODRH_25,ODRH_6_25,ODRH_3_125,ODRH_0_7873};

//MACROS para CTRL_REG2
#define FXOS_CR2_MODS(x) ((uint8_t)(x) & 0b11)	 //oversampling mode selector
enum mods_sel {NORMAL_M,LN_LP_M,HIGHRES_M,LP_M};

//MACROS para CTRL_REG3
#define FXOS_CR3_IPOL_MASK (0x02)		//polaridad active high de INT1/2 (defoult active low)
#define FXOS_CR3_PP_OD_MASK (0x01)		//push-pull(=0) or open-drain(=1) INT1/2

//MACROS para CTRL_REG4
#define FXOS_CR4_DRDY_MASK (0x01)		//data ready interrupt enable

//MACROS para CTRL_REG5
#define FXOS_CR5_CFG_DRDY_MASK (0x01)	//data ready interrupt routing to INT1, defoult(=0) INT2

//MACROS para M_CTRL_REG1
#define FXOS_M_CR1_HMS_MASK(x) ((uint8_t)(x) & 0b11)	//mode:accel,mag or accel/mag (hybrid)

//MACROS para M_CTRL_REG2
#define FXOS_M_CR2_H_AUTOINC_MASK (0x20)	//activar autoincremento en modo hybrid

//MACROS para DR_STATUS
#define FXOS_DRS_XYZDR_MASK (0x08)		//data ready bit mask for DR_STATUS


#endif /* FXOS8700CQ_H_ */
