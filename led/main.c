
/*	ͷ�ļ�	 */
#include "hw_types.h"				// HWREG��x��
#include "hw_syscfg0_OMAPL138.h"	// SYSCFG0_PINMUX(x)
#include "soc_OMAPL138.h"			// SOC_GPIO_0_REGS
#include "gpio.h"					// GPIO_DIR_OUTPUT

/*	��������	 */
void GPIOBankPinInit(void);//GPIO��ʼ����������



/*	������	*/
void main(void)
{
	unsigned int i;

    GPIOBankPinInit();//GPIO��ʼ��

    /*	����ƿ��Ƴ���	*/
    for(;;)
    {
    	for(i=0x00FFFFFF;i>0;i--);//��ʱ
    	GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_LOW);//D10��,GPIO0[2]
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_HIGH);//D7��,GPIO0[0]

		for(i=0x00FFFFFF;i>0;i--);//��ʱ
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_LOW);//D7��,GPIO0[0]
		GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_HIGH);//D6��,GPIO0[5]

		for(i=0x00FFFFFF;i>0;i--);//��ʱ
		GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_LOW);//D6��,GPIO0[5]
		GPIOPinWrite(SOC_GPIO_0_REGS, 2, GPIO_PIN_HIGH);//D9��,GPIO0[1]

		for(i=0x00FFFFFF;i>0;i--);//��ʱ
		GPIOPinWrite(SOC_GPIO_0_REGS,2, GPIO_PIN_LOW);//D9��,GPIO0[1]
		GPIOPinWrite(SOC_GPIO_0_REGS,3, GPIO_PIN_HIGH);//D10��,GPIO0[2]
    }
}


/*	GPIO��ʼ��	*/
void GPIOBankPinInit(void)
{
	unsigned int PINMUX_1_VAL;//�ֲ�����

	/*	��ȡPINMUX1�Ĵ�����ֵ	*/
	PINMUX_1_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1));
	/*	���װ�4���û�led�Ķ�Ӧ�ܽŸ���ΪGPIO����GPIO0[0]��GPIO0[1]��GPIO0[2]��GPIO0[5]	*/
	PINMUX_1_VAL=(PINMUX_1_VAL&0x000ff0ff)|0x88800800;
	/*	�����ú��ֵд��PINMUX1�Ĵ���	*/
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1))=PINMUX_1_VAL;


	/*���õװ�4���û�led�Ķ�Ӧ�ܽ�Ϊ���,����Ϊ���� GPIO��һ���ܽ� �Ķ�Ӧֵ(��Χ 1~144)
	 * GPIO0[0] 1	��GPIO1[0] 17
	 * GPIO2[0] 33	��GPIO3[0] 49
	 * GPIO4[0] 65	��GPIO5[0] 81
	 * GPIO6[0] 97	��GPIO7[0] 113
	 * GPIO8[0] 129	*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 1, GPIO_DIR_OUTPUT);//GPIO0[0]
    GPIODirModeSet(SOC_GPIO_0_REGS, 2, GPIO_DIR_OUTPUT);//GPIO0[1]
    GPIODirModeSet(SOC_GPIO_0_REGS, 3, GPIO_DIR_OUTPUT);//GPIO0[2]
    GPIODirModeSet(SOC_GPIO_0_REGS, 6, GPIO_DIR_OUTPUT);//GPIO0[5]


    /*	��ʼ���װ�4���û�led״̬Ϊ��	*/
    GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_LOW);//D7��,GPIO0[0]
    GPIOPinWrite(SOC_GPIO_0_REGS, 2, GPIO_PIN_LOW);//D9��,GPIO0[1]
    GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_LOW);//D10��,GPIO0[2]
    GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_LOW);//D6��,GPIO0[5]
}
