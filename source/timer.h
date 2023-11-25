#ifndef _TIMER_H_
#define _TIMER_H_


/**
 * @brief Inicializa driver timer
 * @time recibe tiempo de primer temporizador, si es 0 no arranca en
 * 			la primera vez, y se debe usar luego setTimeAndInit
 */
void timerInit(uint32_t time);

/**
 * @brief verifica si paso el tiempo
 * @return true si paso, false si no paso
 */
bool timePassed(void);

/**
 * @brief Lee el tiempo que le falta para terminar al temporizador
 * @return tiempo en ms
 */
uint32_t time(void);

/**
 * @brief Arranca un temporizador
 * @param time: tiempo en ms del temporizador
 */
void setTimeAndInit(uint32_t time);



#endif
