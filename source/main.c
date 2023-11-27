/*******************************************************************************
  @file     +main.c+
  @brief    +App+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "encoder_app.h"
#include "board.h"
#include "Encoder.h"
#include "CardReaderDrv.h"
#include "DVR_Displays.h"
#include "card_app.h"
#include "timer.h"
#include "Leds_stat.h"
#include "hardware.h"
#include "tick.h"
#include "ThingSpeak.h"
#include "keepAlive.h"

#include <os.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define QSIZE 2
#define MAX_USERS 20
#define MAX_NUMBER 10e5

#define CODIFICACIONDEBORRAR 8
#define CODIFICACIONDEENTER 8
#define ID_IDENTIFICATION 187

/* LEDs */
#define LED_R_PORT            PORTB
#define LED_R_GPIO            GPIOB
#define LED_G_PORT            PORTE
#define LED_G_GPIO            GPIOE
#define LED_B_PORT            PORTB
#define LED_B_GPIO            GPIOB
#define LED_R_PIN             22
#define LED_G_PIN             26
#define LED_B_PIN             21
#define LED_B_ON()           (LED_B_GPIO->PCOR |= (1 << LED_B_PIN))
#define LED_B_OFF()          (LED_B_GPIO->PSOR |= (1 << LED_B_PIN))
#define LED_B_TOGGLE()       (LED_B_GPIO->PTOR |= (1 << LED_B_PIN))
#define LED_G_ON()           (LED_G_GPIO->PCOR |= (1 << LED_G_PIN))
#define LED_G_OFF()          (LED_G_GPIO->PSOR |= (1 << LED_G_PIN))
#define LED_G_TOGGLE()       (LED_G_GPIO->PTOR |= (1 << LED_G_PIN))
#define LED_R_ON()           (LED_R_GPIO->PCOR |= (1 << LED_R_PIN))
#define LED_R_OFF()          (LED_R_GPIO->PSOR |= (1 << LED_R_PIN))
#define LED_R_TOGGLE()       (LED_R_GPIO->PTOR |= (1 << LED_R_PIN))

/* Task Start */
#define TASKSTART_STK_SIZE 		1024u
#define TASKSTART_PRIO 			4u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];

#define TASKCLOUD_STK_SIZE			256u
#define TASKCLOUD_STK_SIZE_LIMIT	(TASKCLOUD_STK_SIZE / 10u)
#define TASKCLOUD_PRIO              2u
OS_TCB Task_Cloud_TCB;
CPU_STK Task_Cloud_Stk[TASKCLOUD_STK_SIZE];

#define TASKKEEPALIVE_STK_SIZE			256u
#define TASKKEEPALIVE_STK_SIZE_LIMIT	(TASKKEEPALIVE_STK_SIZE / 10u)
#define TASKKEEPALIVE_PRIO              3u
OS_TCB Task_KeepAlive_TCB;
CPU_STK Task_KeepAlive_Stk[TASKKEEPALIVE_STK_SIZE];





/* Example semaphore */
static OS_SEM semTest;
static OS_SEM semCom;

/* Queue */

#define QUEUE_SIZE 10
static OS_Q queue;
static OS_Q queue_encoder;

/* mutex */
static OS_MUTEX mutex;



enum fm1_states{INITIAL, MENU, GET_ID, CHECK_ID, GET_PIN, CHECK_PIN, TRY_AGAIN, OK, NOT_OK, HOLD, ERASE_MENU, INTENSITY_MENU};
enum fm2_states{NONE, ENCODER, TARJETA};
enum reg_status {FULL, NO_FULL, MISTAKE, GO_BACK, CHECK};
enum menu_options{NUM, DONE, DELETE, INTENSITY};
enum delete_options{DIGIT,ALL_DIGITS,VOLVER_B,CANCELAR};
enum go_back_flags{NO_BOTON, BOTON_BACK, BOTON_NUM};
typedef struct User {
  uint64_t ID;
  uint64_t PIN;
  int counter_block;
  bool in;
  int floor;
} User;


static unsigned fm1_state = INITIAL;


static User* last_user;
static User users[MAX_USERS];
static User aux_mistake;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 /* return: 	TRUE si ocurrio un evento el SW
 * 			FALSE si no ocurrio y realizo las operaciones
 */



/**
 * @brief Obtiene el ID ingresado
 * @param ID variable donde se guarda el ID ingresado
 * @param disp_show puntero al arreglo de los símbolos a mostrar en el display
 * @param go_back flag para saber si se debe volver al menú principal o no
 * @return int  estado del ID ( reg_status )
 */
static int get_ID(uint64_t* ID, uint8_t* disp_show, int* go_back);


/**
 * @brief Chequea si el ID es correcto o no
 * @param ID ID a chequear
 * @return User* puntero a un usuario (si es correcto manda el puntero al usuario del ID y si es incorrecto manda &aux_mistake)
 */
static User* check_ID(uint64_t ID, bool* new_user_flag);


/**
 * @brief Obtiene el PIN ingresado
 * @param PIN variable donde se guarda el PIN ingresado
 * @param disp_show puntero al arreglo de los símbolos a mostrar en el display
 * @param go_back flag para saber si se debe volver al menú principal o no
 * @return int  estado del PIN ( reg_status )
 */

static int get_PIN(uint64_t* PIN,uint8_t * disp_show,int * go_back);


/**
 * @brief Chequea si el PIN es correcto o no
 * @param ID PIN a chequear
 * @param user usuario correspondiente al ID ingresado anteriormente
 * @return bool (false si el PIN es incorrecto , true si es correcto)
 */
static bool check_PIN(uint64_t PIN, User* user, bool new_user_flag);


/**
 * @brief Guarda un dígito en el registro requerido
 * @param pointer puntero al registro donde se desea guarda el dígito
 * @param number dígito a guardar
 * @param max máximo número de potencia de 10 que admite el registro
 * @param max_dig máxima cantidad de dígitos que admite el registro
 * @return int estado del registro (reg_status)
 */
static int save_number_encoder(uint64_t*pointer, int number, uint32_t max, uint8_t max_dig);


static User* create_new_user(void);

/**
 * @brief Elimina el último dígito del registro
 * @param reg registro
 */
static void delete_last_digit(uint64_t*reg);

/**
 * @brief analiza el dígito ingresado por el encoder
 * @param reg variable donde se guarda el el dígito ingresado
 * @param number dígito leido por el encoder
 * @param disp_show puntero al arreglo de los símbolos a mostrar en el display
 * @param go_back flag para saber si se debe volver al menú principal o no
 * @param max_num máximo número de potencia de 10 que admite el registro
 * @param max_dig máxima cantidad de dígitos que admite el registro
 * @return uint8_t estado del registro (reg_status)
 */
static uint8_t get_digit(uint64_t * reg ,int8_t * number, uint8_t * disp_show, int * go_back, uint64_t max_num,uint8_t max_dig);



/**
 * @brief Escribe en el display los 4 números queridos
 * @param d0 número para el display D0
 * @param d1 número para el display D1
 * @param d2 número para el display D2
 * @param d3 número para el display D3
 */

static void setDigitDisp(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3);


/**
 * @brief Determina lo que se debe mostrar en el display según el ID y cantidad de dígitos
 * @param disp_show puntero al arreglo de los simbolos a mostrar en el display
 * @param reg registro del ID
 */
static void ID_to_disp(uint8_t * disp_show, uint64_t * reg );



/**
 * @brief Determina lo que se debe mostrar en el display según el PIN y cantidad de dígitos
 * @param disp_show puntero al arreglo de los simbolos a mostrar en el display
 * @param simbol símbolo que reemplaza a los números
 */
static void PIN_to_disp(uint8_t * disp_show, uint8_t simbol );


/**
 * @brief Determina el piso del usuario ingreso
 * @param ID id del usuario
 * @return int:
 *             1 -> 0 personas en el piso 1
 *             2 -> 0 personas en el piso 2
 *             3 -> 0 personas en el piso 3
 *             4 -> 1 persona en el piso 1
 *             5 -> 1 persona en el piso 2
 *             6 -> 1 persona en el piso 3
 *             7 -> 2 personas en el piso 1
 *             8 -> 2 personas en el piso 2
 *             9 -> 2 personas en el piso 3
 */
static int defineFloor(uint64_t ID);


static int8_t counter = 0;
static int8_t counter_users = 6;
static bool tarjeta_ID = false;








static void TaskStart(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;

    /* Initialize the uC/CPU Services. */
    CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    /* (optional) Compute CPU capacity with no task running */
    OSStatTaskCPUUsageInit(&os_err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    //Creación de las task para enviar a la nube

     OSTaskCreate(&Task_Cloud_TCB, 			//tcb
                   "Task Cloud",		//name
				   	Task_Cloud,			//func
                    &queue,					//arg
                    TASKCLOUD_PRIO,			//prio
                   &Task_Cloud_Stk[0u],			//stack
                    TASKCLOUD_STK_SIZE_LIMIT,	//stack limit
                    TASKCLOUD_STK_SIZE,		//stack size
                    0u,
                    0u,
                    0u,
                   (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                   &os_err);

     OSTaskCreate(&Task_KeepAlive_TCB, 			//tcb
					"Task Keep Alive",		//name
					Task_KeepAlive,			//func
					 NULL,					//arg
					 TASKKEEPALIVE_PRIO,			//prio
					&Task_KeepAlive_Stk[0u],			//stack
					 TASKKEEPALIVE_STK_SIZE_LIMIT,	//stack limit
					 TASKKEEPALIVE_STK_SIZE,		//stack size
					 0u,
					 0u,
					 0u,
					(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
					&os_err);


     //PARÁMETROS
	uint64_t ID = 0;
	uint64_t PIN = 0;
	int try = 0;
	User* user = &aux_mistake ;
	bool new_user_flag = false;
	last_user = &users[0];
	bool ok;
	int ID_status;
	int PIN_status;

	//USUARIOS
	User one;
	one.ID = 46605701;
	one.PIN = 11111;
	users[1] = one;
	users[1].counter_block = false;
	users[1].floor = 1;
	users[1].in = false;

	User two;
	two.ID = 63913004;
	two.PIN = 22222;
	users[2] = two;
	users[2].counter_block = false;
	users[2].floor = 1;
	users[2].in = false;

	User three;
	three.ID = 45176601;
	three.PIN = 33333;
	users[3] = three;
	users[3].counter_block = false;
	users[3].floor = 2;
	users[3].in = false;

	User four;
	four.ID = 37159500;
	four.PIN = 44444;
	users[4] = four;
	users[4].counter_block = false;
	users[4].floor = 2;
	users[4].in = false;

	User five;
	five.ID = 60612684;
	five.PIN = 55555;
	users[5] = five;
	users[5].counter_block = false;
	users[5].floor = 3;
	users[5].in = false;

	User six;
	six.ID = 80000606;
	six.PIN = 66666;
	users[6] = six;
	users[6].counter_block = false;
	users[6].floor = 3;
	users[6].in = false;



	int msg = 0;
	int last_state = INITIAL;
	uint8_t menu_data[4];
	bool ingresed_number;
	int go_back = 0;
	uint8_t disp_show[4] = { SIM_GUION,  SIM_GUION,  SIM_GUION,  SIM_GUION};
	OS_MSG_SIZE p_size;

	//INICIALIZACIÓN DE LOS DRIVERS
	ledsInit();
	DRV_Enc_Init(&queue_encoder);
	cardReaderInit();
	timerInit(0);
	Leds_Stat_Init();


	int inicio_temp = 0;
	int menu_option=0;
	uint32_t arrAux[NUM_CHARACTERS];
	uint8_t intensidad = MAX_INTENS;

	int delete_option=0;

    while (1)
    {
    	switch (fm1_state)
    		{
    		case INITIAL:
    			fm1_state = MENU;
    			ID = 0;
    			PIN = 0;
    			try = 0;
    			user = 0;
    			counter = 0;
    			go_back = 0;
    			new_user_flag = false;
    			ID_status = NO_FULL;
    			PIN_status = NO_FULL;
    			ingresed_number = false;
    			menu_option = 0;
    			delete_option = 0;
    			tarjeta_ID = false;

    			disp_show[0] = SIM_GUION;
    			disp_show[1] = SIM_GUION;
    			disp_show[2] = SIM_GUION;
    			disp_show[3] = SIM_GUION;

    			//writeNum((uint64_t) 1234);
    			break;
    		case MENU:
    			deleteBuffer();
    			//getBuffer();
    			setDigitDisp(LETRA_N,LETRA_D,LETRA_B,LETRA_I);


    			if (PIN_status == MISTAKE)
    			{
    				//show_display;
    				fm1_state  = NOT_OK;
    			}
    			else if (ID_status == MISTAKE)
    			{
    				//show display
    				fm1_state  = NOT_OK;

    			}


    			if (menu_option < 4)
    			{
    				menu_option += get_paso();

    			}
    			if (menu_option>=4)
    			{
    				menu_option = 0;
    			}
    			if (menu_option < 0)
    			{
    				menu_option = 3;
    			}

    			blinkDisp(menu_option);

    			char* press_buttonc;
    			char press_button = 0;
				press_buttonc = OSQPend((OS_Q*) &queue_encoder, 0, OS_OPT_PEND_NON_BLOCKING, &p_size, NULL, &os_err);
				press_button = *press_buttonc;
    			//int press_button =  get_boton();//get button status
    			if(press_button > 1)
    			{
    				if(menu_option == NUM)
    				{
    					if(ID_status == NO_FULL) //PENSAR BIEN ESTO
    					{
    						fm1_state = GET_ID;
    					}
    					else if(ID_status == CHECK && PIN_status == NO_FULL)
    					{
    						fm1_state = GET_PIN;
    					}
    					else
    					{
    						fm1_state = MENU;
    					}
    				}
    				else if(menu_option == DONE)
    				{
    					if(last_state == GET_ID )
    					{
    						fm1_state = CHECK_ID;
    					}
    					else if(last_state == GET_PIN)
    					{
    						fm1_state = CHECK_PIN;
    					}
    					else
    					{
    						//shows display not done
    						fm1_state = MENU;
    					}

    				}
    				else if(menu_option == DELETE)
    				{
    						fm1_state = ERASE_MENU;

    				}

    				else if(menu_option == INTENSITY)
    				{
    					fm1_state =INTENSITY_MENU;
    				}
    				menu_option = 0;
    			}
    			else
    			{
    				//writeNum(1000);
    				fm1_state = MENU;
    			}
    			break;
    		case ERASE_MENU:

    			setDigitDisp(1,LETRA_B,SIM_VOLVER,LETRA_C);
    			if (delete_option < 4)
    			{
    				delete_option += get_paso();

    			}
    			if(delete_option >= 4)
    			{
    				delete_option = 0;
    			}
    			if(delete_option < 0){
    				delete_option = 3;
    			}

    			blinkDisp(delete_option);
				press_button = 0;
    			press_buttonc = OSQPend((OS_Q*) &queue_encoder, 0, OS_OPT_PEND_NON_BLOCKING, &p_size, NULL, &os_err);
    			press_button = *press_buttonc;
    			//press_button = get_boton();//get button status
    			if(press_button != 0)
    			{
    				if(delete_option == ALL_DIGITS)
    				{
    					counter = 0;
    					uint8_t i = 0;
    					for(i=0; i < CANT_DISP-1; i++){
    						disp_show[i] = SIM_GUION;
    					}
    					disp_show[3] = SIM_VOLVER;
    					if(last_state ==  GET_ID)
    					{
    						ID = 0;
    						ID_status = NO_FULL;
    						fm1_state = MENU;

    					}
    					else if(last_state == GET_PIN)
    					{
    						PIN = 0;
    						fm1_state = MENU;
    						PIN_status = NO_FULL;
    					}
    				}
    				else if (delete_option == DIGIT && !tarjeta_ID)
    				{

    					if(last_state ==  GET_ID)
    					{
    						ID_status = NO_FULL;
    						delete_last_digit(&ID);
    						ID_to_disp(disp_show, &ID);
    						fm1_state = MENU;
    					}
    					else if(last_state == GET_PIN)
    					{
    						PIN_status = NO_FULL;
    						delete_last_digit(&PIN);
    						PIN_to_disp(disp_show, LETRA_N); //LETRA H x
    						fm1_state = MENU;
    					}
    				}
    				else if(delete_option == VOLVER_B){
    					fm1_state = MENU;
    				}
    				else if(delete_option == CANCELAR){
    					fm1_state = INITIAL;
    				}

    				delete_option = 0;
    			}
    			else
    			{
    				fm1_state =  ERASE_MENU;
    			}

    			break;

    		case INTENSITY_MENU:

    			setDigitDisp(APAGADO,APAGADO,APAGADO,intensidad);
    			blinkDisp(D3);
    			intensidad += get_paso();
    			if(intensidad > MAX_INTENS){
    				intensidad = MIN_INTENS;
    			}
    			if(intensidad < MIN_INTENS){
    				intensidad = MAX_INTENS;
    			}
    			press_button = 0;
    			press_buttonc = OSQPend((OS_Q*) &queue_encoder, 0, OS_OPT_PEND_NON_BLOCKING, &p_size, NULL, &os_err);
    			press_button = *press_buttonc;
    			//press_button = get_boton();//get button status
    			if(press_button != 0)
    			{
    				//setIntensidad(intensidad);
    				fm1_state = MENU;
    			}
    			else
    			{
    				fm1_state = INTENSITY_MENU;
    			}
    			setIntensidad(intensidad);
    			break;
    		case GET_ID:
    			last_state = GET_ID;
    			ID_status= get_ID(&ID,  disp_show,&go_back);

    			writeWord(&disp_show);
    			blinkDisp(D3);
    			if (ID_status == FULL)
    			{
    				fm1_state = MENU;
    			}
    			else if (ID_status == MISTAKE)
    			{
    				tarjeta_ID = true;
    				fm1_state = NOT_OK;
    				//mostrar error
    			}
    			else if(ID_status == NO_FULL && go_back != BOTON_BACK)
    			{
    				fm1_state = GET_ID; //CAMBIAR A MENU
    			}
    			else if(ID_status == NO_FULL  && go_back == BOTON_BACK)
    			{
    				fm1_state = MENU;

    			}


    			break;
    		case CHECK_ID:
    			last_state = CHECK_ID;
    			stopBlink();

    			user = check_ID(ID, &new_user_flag);
    			if (user == &aux_mistake && new_user_flag == false)
    			{

    				fm1_state = NOT_OK;
    			}
    			else if(user == &aux_mistake && new_user_flag == true)
    			{

    				 fm1_state = GET_ID;
    			}
    			else
    			{
    				if(inicio_temp==0){
    					setTimeAndInit(6000);
    					inicio_temp = 1;
    				}

    				if(timePassed() && inicio_temp){
    					inicio_temp = 0;
    					uint8_t i = 0;
    					for(i=0; i < CANT_DISP; i++){
    						disp_show[i] = SIM_GUION;
    					}
    					fm1_state = GET_PIN;
    					ID_status = CHECK;
    					counter = 0;

    				}
    				if(time()>3000){
    					writeNum(ID/10000);
    					get_paso();
    					get_boton();
    				}
    				else if(!timePassed()){
    					writeNum(ID);
    					get_paso();
    					get_boton();
    				}



    			}
    			break;
    		case GET_PIN:
    		{
    			last_state = GET_PIN;
    			if(!inicio_temp){
    				PIN_status = get_PIN(&PIN,  disp_show,&go_back);
    			}

    			if(go_back==BOTON_NUM && inicio_temp==0){
    				setTimeAndInit(1000);
    				inicio_temp = 1;
    			}
    			if(timePassed() && inicio_temp  ){
    				//sin tiempo
    				disp_show[D2] = LETRA_N; //letra X TODO
    				inicio_temp = 0;
    				get_paso();
    				get_boton();
    			}

    			writeWord(disp_show);

    			blinkDisp(D3);
    			if (PIN_status == FULL)
    			{
    				fm1_state = MENU;
    			}
    			else if (PIN_status == MISTAKE)
    			{
    				fm1_state = NOT_OK;
    				//mostrar error
    			}
    			else if(PIN_status == NO_FULL && go_back !=BOTON_BACK )
    			{
    				fm1_state = GET_PIN;
    			}
    			else if(PIN_status == NO_FULL  && go_back == BOTON_BACK)
    			{
    				fm1_state = MENU;
    			}

    		}
    		break;
    		case CHECK_PIN:
    			last_state = CHECK_PIN;
    			inicio_temp = 0;
    			tarjeta_ID = false;
    			ok = check_PIN(PIN, user, new_user_flag);
    			if (ok)
    			{

    				fm1_state = OK;
    			}
    			else if (try < 2 && !ok)
    			{
    				uint8_t i = 0;
    				for(i=0; i < CANT_DISP-1; i++){
    					disp_show[i] = SIM_GUION;
    				}
    				disp_show[3] = SIM_VOLVER;
    				fm1_state = MENU;
    				PIN = 0;
    				PIN_status = NO_FULL;
    				try++;
    			}
    			else if (try  == 2 && !ok)
    			{
    				fm1_state =  NOT_OK;
    				user->counter_block = true;
    			}

    		break;
    		case TRY_AGAIN:
    		break;
    		case OK:

    			Write_Led_stat(STAT_D1); //pestillo D3
    			stopBlink();
    			setDigitDisp(LETRA_P,LETRA_A,5,LETRA_E);

    			if(inicio_temp==0){
    				setTimeAndInit(1000);
    				inicio_temp = 1;
    			}

    			if(timePassed() && inicio_temp  ){
    				//sin tiempo
    				Turn_off_led_stat(STAT_D1); //D3
    				inicio_temp = 0;
    				msg = defineFloor(ID);
    				OSQPost (&queue, (void*)&msg, sizeof(void), OS_OPT_POST_FIFO, &os_err);
    				fm1_state = INITIAL;
    			}


    		break;
    		case NOT_OK:
    			if(new_user_flag)
    			{

    				//No agregamos esa funcionalidad
    			}
    			else if(!new_user_flag)
    			{

    				stopBlink();
    				if(last_state == CHECK_ID || tarjeta_ID)
    				{
    					Write_Led_stat(STAT_D3); // prendio D1
    					setDigitDisp(LETRA_E,LETRA_R,LETRA_R,0); //ErrO ID mal

    					if(inicio_temp==0){
    						setTimeAndInit(2000);
    						inicio_temp = 1;
    					}

    					if(timePassed() && inicio_temp  ){
    						//sin tiempo
    						Turn_off_led_stat(STAT_D3);
    						inicio_temp = 0;
    						fm1_state = INITIAL;
    					}


    				}
    				if(last_state == CHECK_PIN)
    				{

    					Write_Led_stat(STAT_D2); //

    					setDigitDisp(LETRA_E,LETRA_R,LETRA_R,1); //Err1 ID mal
    					if(inicio_temp==0){
    						setTimeAndInit(2000);
    						inicio_temp = 1;
    					}

    					if(timePassed() && inicio_temp  ){
    						//sin tiempo
    						Turn_off_led_stat(STAT_D2);
    						inicio_temp = 0;
    						fm1_state = INITIAL;
    					}

    				}



    			}
    		break;
    		case HOLD:
    		//wait 5 seconds
    		//encender led
    		fm1_state = MENU;
    		break;
    		default:
    			break;
    		}


    	}


}


int main(void) {
    OS_ERR err;

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    hw_Init();


    OSInit(&err);
 #if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	 /* Enable task round robin. */
	 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &err);
 #endif
	 OS_CPU_SysTickInit(SystemCoreClock / (uint32_t)OSCfg_TickRate_Hz);


    /* Create semaphore */
    OSSemCreate(&semTest, "Sem Test", 0u, &err);
    OSSemCreate(&semCom, "Sem Com", 0u, &err);

    Task_Cloud_Set_Semaphore(&semCom);
    Task_KeepAlive_Set_Semaphore(&semCom);

    //Creamos la Queue
    OSQCreate(&queue, "queue", (OS_MSG_QTY)QUEUE_SIZE, &err);

    /* Crear mutex */
    OSMutexCreate(&mutex, "mutex", &err);

    Task_Cloud_Set_Mutex(&mutex);
    Task_KeepAlive_Set_Mutex(&mutex);

    OSQCreate(&queue_encoder, "queue_encoder", (OS_MSG_QTY)QUEUE_SIZE, &err);

    OSTaskCreate(&TaskStartTCB,
                 "App Task Start",
                  TaskStart,
                  0u,
                  TASKSTART_PRIO,
                 &TaskStartStk[0u],
                 (TASKSTART_STK_SIZE / 10u),
                  TASKSTART_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 &err);

    App_OS_SetAllHooks();
    OSStart(&err);



	/* Should Never Get Here */
    while (1) {

    }
}

static int get_ID(uint64_t* ID, uint8_t* disp_show, int* go_back)
{


	 	uint32_t number_tarjeta_buff[NUM_CHARACTERS];
       uint8_t number_tarjeta_status = getBuffer(number_tarjeta_buff);//  = get_number_tarjeta(); //Ver qué tipo de dato es
       *go_back = 0;
       int press_button = 0;
       int ID_full = NO_FULL;
       static int8_t number = 0;

       if (number_tarjeta_status == CARD_ACCEPTED)
       {

       	uint64_t number_tarjeta = convertToID2(number_tarjeta_buff);
       	*ID = number_tarjeta;
           ID_full = FULL;
           ID_to_disp(disp_show,ID);
           tarjeta_ID = true;
           //next_state = NEXT;
       }
       else if( number_tarjeta_status == CARD_REJECTED)
       {
       	ID_full = MISTAKE;
       }
       else if(number_tarjeta_status ==  NO_CARD) //Se presionó el  (leo recién el número si se presionó el botón pues sino elimino los que venía guardando antes de presionar el botón)
       {
       	ID_full = get_digit(ID, &number, disp_show, go_back, 1e7, 8);
       	//    static uint8_t get_digit(uint64_t * reg ,uint8_t * number, uint8_t * disp_show, int * go_back, uint64_t max_num,uint8_t max_dig){


       }
       else
       {
           ID_full = NO_FULL;
       }
       return ID_full;
   }

   static int get_PIN(uint64_t* PIN, uint8_t * disp_show, int * go_back)
   {
       int PIN_full = NO_FULL;
       static int8_t number = 0;
       *go_back = NO_BOTON;
       PIN_full = get_digit(PIN, &number, disp_show, go_back, 1e4, 5);





       return PIN_full;
   }

   static uint8_t get_digit(uint64_t * reg ,int8_t * number, uint8_t * disp_show, int * go_back, uint64_t max_num,uint8_t max_dig){
   	uint8_t reg_full = NO_FULL;
   	//int8_t press_button;
	OS_MSG_SIZE p_size;
	char* press_buttonc;
	char press_button = 0;
	OS_ERR os_err;
   	if((*number < 11)  && *number >= -1)
		{
			*number += get_paso();
			press_buttonc = OSQPend((OS_Q*) &queue_encoder, 0, OS_OPT_PEND_NON_BLOCKING, &p_size, NULL, &os_err);
			press_button = *press_buttonc;
			//press_button = get_boton();
			if(press_button > 1)
			{

				if (*number > 0 ) //  -n -n -1231243 -2
				{
					*go_back = BOTON_NUM;
					disp_show[0] = disp_show[1];
					disp_show[1] = disp_show[2];
					disp_show[2] = disp_show[3];
					reg_full = save_number_encoder(reg, *number - 1 , max_num, max_dig);
				}
				else if(*number==0){
					*number = 0;
					*go_back = BOTON_BACK;
					reg_full = NO_FULL;
				}
			}

			else
			{
				reg_full = NO_FULL;
				if(*number==-1){
					*number=10;
				}
				disp_show[3] = ( (*number) ? (*number - 1) : SIM_VOLVER );
			}

		}
		else if ((*number == 0) || (*number > 10))
		{
			*number = 0; //LETRA MENU
		}
		else if(*number < 0){
			*number = 10;
		}
   	return reg_full;
   }

   static User* check_ID(uint64_t ID, bool* new_user_flag){
		bool exits = false;
		if (ID == ID_IDENTIFICATION && *new_user_flag == false)
		{
		  *new_user_flag = true;
		}
		else if (ID == ID_IDENTIFICATION && *new_user_flag == true)
		{
			*new_user_flag= false; //HAY QUE VER UN FORMA DE QUE SE REINICIE ESTO
		}
		else if(ID != ID_IDENTIFICATION && *new_user_flag == true) //Se quiso agregar un nuevo
		{
			create_new_user();
		}
		else
		{
			for (int i = 1; i <counter_users+1; i++)
			{
				if (users[i].ID == ID && users[i].counter_block==false) //Encuentra si hay un ID existente con el ingresado
				{

					exits = true;
					return &(users[i]);
				}
			}
		}
	return &aux_mistake ;

}



static bool check_PIN(uint64_t PIN, User* user, bool new_user_flag){
	bool correct = false;
	if(new_user_flag)
	{
		user->PIN = PIN;
		correct = true;
	}
	else if (!new_user_flag)
	{
		if (user->PIN == PIN)
		{
			correct = true;
			user->in =!(user->in);
		}
		/*
		for (int i = 1; i < counter_users+1; i++)
		{
			if (users[i].PIN == PIN) //Encuentra si hay un ID existente con el ingresado
			{

				break;
			}
		}*/
	}
	return correct;
}

//Solo si recibe un digito por vez
static int save_number_encoder(uint64_t*pointer, int number, uint32_t max, uint8_t max_dig)
{
	int num = number;

	if(*pointer >= max || number > 10)
	{
		return MISTAKE;
	}
	else
	{
		counter++;
		num += *pointer*10;
		*pointer = num;
		if (counter == max_dig ||  *pointer >= max)
		{
			return FULL;
			counter=0;
		}
		else
		{
			return NO_FULL;
		}
		return true;

	}

}

static User* create_new_user(void)
{
	last_user++;
	return last_user;

}


static void delete_last_digit(uint64_t *reg)
{
	if(*reg < 10 )
	{
		*reg = 0;
		if(!counter){
			counter = 0;
		}
		else{
			counter--;
		}

	}
	else
	{
		*reg/= 10;
		counter--;
	}
}

static void ID_to_disp(uint8_t * disp_show, uint64_t * reg ){
	int8_t i = 0;
	uint64_t num = *reg;

	if(counter>=3){
		for(i=2; i>=0; i--){
			disp_show[i] = num % 10;
			num /= 10;
		}
	}
	else{

		for(i=2; i>-1; i--){

			disp_show[i] = num % 10;
			num /= 10;
			if(i < 3-counter){
				disp_show[i] = SIM_GUION;
			}
		}
	}
	disp_show[3] = SIM_VOLVER;
}

static void PIN_to_disp(uint8_t * disp_show, uint8_t simbol){
	int8_t i = 0;

	if(counter>=3){
		for(i=2; i>=0; i--){
			disp_show[i] = simbol;
		}
	}
	else{
		for(i=2; i>-1; i--){
			disp_show[i] = simbol;
			if(i < 3-counter){
				disp_show[i] = SIM_GUION;
			}
		}
	}
	disp_show[3] = SIM_VOLVER;

}


static void setDigitDisp(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3){
	writeDisp(d0,D0);
	writeDisp(d1,D1);
	writeDisp(d2,D2);
	writeDisp(d3,D3);
}

static int defineFloor(uint64_t ID)
{
	int i = 0;
	int j = 0;
	static int floors[3] = {1,2,3};
	for ( i = 1; i <counter_users+1; i++)
	{
		if(users[i].ID == ID)
		{
			j = users[i].floor;
				 if(users[i].in)
				 {
					 if(floors[j-1] == j)
					 {
						 floors[j-1] = 3 + j ;
					 }
					 else if(floors[j-1]== 3 + j)
					 {
						 floors[j-1]+= 3;
					 }
				 }
				 else
				 {
					 if(floors[j-1] == 3 + j)
					 {
						 floors[j-1] = j;
					 }
					 else if(floors[j-1]== j + 6)
					 {
						 floors[j-1] -= 3;
					 }
				 }
		}
	}
	return floors[j-1];

}
