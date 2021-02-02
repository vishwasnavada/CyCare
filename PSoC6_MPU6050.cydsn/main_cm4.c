/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "mpu6050.h"


int16_t CAX, CAY, CAZ; //current acceleration values
int16_t CGX, CGY, CGZ; //current gyroscope values
int16_t Breath_rate;
int16_t upper_threshold = 152;
int16_t lower_threshold = -58;


int main(void)
{
   
    __enable_irq(); /* Enable global interrupts. */
    
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_MPU6050_Start();        //Start the SC I2C
    MPU6050_init();             //Initialize MPU6050
	MPU6050_initialize();
	MPU6050_setDLPFMode(6);     // Setting the LPF at 5Hz both Gyro and temoperature for 9250
    UART_Start();               //Strat UART
    
    for(;;)
    {
        /* Place your application code here. */
        if (CAZ > upper_threshold ) 
        { 
            Breath_rate++;
        }
        else if(CAZ < lower_threshold) 
        { 
            Breath_rate++;
        }
        Breath_rate = Breath_rate/2;
        /* Breath rate incremeted for crossing upper and lower limits  
         And then divided by 2 to get the real breath rate
         Same can be calculated over a period of time to know the anomaly in the breathing*/
        
       
        /* Get data from the MPU */
		MPU6050_getMotion6(&CAY, &CAX, &CAZ, &CGX, &CGY, &CGZ);
        
        /*Send the data over UART to the BCP */
        Cy_SCB_UART_Put(UART_HW,0xAA);//header
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CAX));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CAX));
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CAY));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CAY));
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CAZ));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CAZ));
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CGX));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CGX));
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CGY));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CGY));
		Cy_SCB_UART_Put(UART_HW,CY_HI8(CGZ));
		Cy_SCB_UART_Put(UART_HW,CY_LO8(CGZ));
	   	Cy_SCB_UART_Put(UART_HW,0x55);//tail
        
       // Cy_SCB_UART_Put(UART_HW,Breath_rate);
        
    }
}

/* [] END OF FILE */
