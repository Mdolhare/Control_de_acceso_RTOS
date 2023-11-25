/***************************************************************************//**
  @file     encoder.c
  @brief    Application functions for encoder
  @author   
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "encoder_app.h"
#include "Encoder.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int read_encoder( int* reg, int number);

static int get_number_encoder(void); //AUXILIAR



int encoder_num(int press_button, int* reg)
{
	int word_status = KEEP_WRITING;
	if(*reg  <= 9 && *reg > 0) //número correcto
	{
	}
	else
	{
		word_status =  MISTAKE_WRITING;
	}
	return word_status;
 /*
    int word_status = KEEP_WRITING;
    int number_encoder;
    static int enter_pressed = 0;
    number_encoder = get_number_encoder();
        if( press_button == 1  && enter_pressed && number_encoder == 0) //Se presionó una vez el botón , pero se había presionado antes un enter
        { 
            word_status = DONE_WRITING;  //Termina la palabra
            enter_pressed = 0;  
        } 
        else if (press_button == 2 )//!!!! ver prioridad de delete o enter
        {
            if(number_encoder != 0)
            {
                word_status =  read_encoder( reg, number_encoder);
                if(word_status == KEEP_WRITING)  
                {
                    word_status = DONE_WRITING;  //Termina la palabra
                    enter_pressed = 0;
                }
            
            }
            word_status = DONE_WRITING;  //Termina la palabra
            enter_pressed = 0; 
        }
        else if (press_button == 1 && number_encoder != 0)
        {
            enter_pressed=1;
            word_status =  read_encoder( reg, number_encoder);
        }
        else
        {
            word_status = MISTAKE_WRITING;
        }
        return word_status;*/
}

static int read_encoder( int* reg, int number)
{
   
}
static int get_number_encoder(void)
{
	static int number = 1;
	return number++;
}
