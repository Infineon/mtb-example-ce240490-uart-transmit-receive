/******************************************************************************
* File Name: main.c
*
* Description: This example demonstrates the UART transmit and receive
*              operation using PDL APIs
*
* Related Document: See Readme.md 
*
*******************************************************************************
* Copyright 2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"


/*******************************************************************************
*            Global Functions
*******************************************************************************/
void handle_error(void);

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
* Summary:
* User defined error handling function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void handle_error(void)
{
     /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.
* Reads one byte from the serial terminal and echoes back the read byte.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    uint32_t read_value;
    uint32_t send_value;
    uint32_t count;

    cy_en_scb_uart_status_t init_status;
    cy_stc_scb_uart_context_t USER_UART_context;

    /* Initialize the device and board peripherals */
    cybsp_init();

    /* Start UART operation */
    init_status = Cy_SCB_UART_Init(USER_UART_HW, &USER_UART_config, &USER_UART_context);
    if (init_status!=CY_SCB_UART_SUCCESS)
       {
            handle_error();
       }
    Cy_SCB_UART_Enable(USER_UART_HW);

    /* Transmit header to the terminal */
    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    Cy_SCB_UART_PutString(USER_UART_HW, "\x1b[2J\x1b[;H");

    Cy_SCB_UART_PutString(USER_UART_HW, "************************************************************\r\n");
    Cy_SCB_UART_PutString(USER_UART_HW, "PDL: UART Transmit and Receive\r\n");
    Cy_SCB_UART_PutString(USER_UART_HW, "************************************************************\r\n\n");
    Cy_SCB_UART_PutString(USER_UART_HW, ">> Start typing to see the echo on the screen \r\n\n");

    __enable_irq();

    for(; ;)
    {
        /* Start read operation  */
        read_value = Cy_SCB_UART_Get(USER_UART_HW);
        /* Blocking read data */
        while(read_value == CY_SCB_UART_RX_NO_DATA)
        {
          read_value = Cy_SCB_UART_Get(USER_UART_HW);
        }
        send_value = (uint8_t)read_value;

        /* Start send operation */
        count = Cy_SCB_UART_Put(USER_UART_HW, send_value);

        /* Checking send data number */
        if(count == 0ul)
        {
          handle_error();
        }
    }
}

/* [] END OF FILE */
