//
/*	ͷ�ļ�	 */

#include <stdio.h>
#include "hw_types.h"				// HWREG��x��
#include "hw_syscfg0_OMAPL138.h"	// SYSCFG0_PINMUX(x)
#include "soc_OMAPL138.h"			// SOC_GPIO_0_REGS
#include "gpio.h"					// GPIO_DIR_INTPUT
#include "interrupt.h"


/*	��������	 */

void GPIOBankPinInit(void);			//GPIO��ʼ��
void SetupInt(void);				//�����жϿ�����
void ConfigureIntGPIO(void);		//���ð����ж�
void GPIO0_6_Isr(void);				//SW5�����жϷ���
void GPIO6_1_Isr(void);				//SW6�����жϷ���


/*	������	*/

void main(void) {

	GPIOBankPinInit();
	SetupInt();
	ConfigureIntGPIO();
	while(1);			//�ȴ������ж�ִ��

}


/*	GPIO��ʼ��	*/

void GPIOBankPinInit(void){

	unsigned int PINMUX_1_VAL;
	unsigned int PINMUX_19_VAL;

	PINMUX_1_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1));	//��ȡPINMUX1�Ĵ�����ֵ
	PINMUX_19_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(19));//��ȡPINMUX19�Ĵ�����ֵ

	PINMUX_1_VAL=(PINMUX_1_VAL&0xffffff0f)|0x00000080;			//���װ尴��SW5�Ķ�Ӧ�ܽŸ���ΪGPIO����GPIO0[6]
	PINMUX_19_VAL=(PINMUX_19_VAL&0xff0fffff)|0x00800000;		//���װ尴��SW6�Ķ�Ӧ�ܽŸ���ΪGPIO����GPIO6[1]

	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1))=PINMUX_1_VAL;	//0x01C14124,PINMUX1[7-4]=0x8h
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(19))=PINMUX_19_VAL;//0x01C1416C,PINMUX19[23-20]=0x8h

	/*���õװ�2���û������Ķ�Ӧ�ܽ�Ϊ���룬����Ϊ���� GPIO��һ���ܽ� �Ķ�Ӧֵ(��Χ 1~144)
	 * GPIO0[0] 1	��GPIO1[0] 17
	 * GPIO2[0] 33	��GPIO3[0] 49
	 * GPIO4[0] 65	��GPIO5[0] 81
	 * GPIO6[0] 97	��GPIO7[0] 113
	 * GPIO8[0] 129	*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_INPUT);	//����GPIO0[6]Ϊ����ģʽ����DIR01[6]=1,DIR01��ַΪ0x01E26010
    GPIODirModeSet(SOC_GPIO_0_REGS, 98, GPIO_DIR_INPUT);//����GPIO6[1]Ϊ����ģʽ����DIR67[1]=1��DIR67��ַΪ0x01E26088

    GPIOIntTypeSet(SOC_GPIO_0_REGS, 7, GPIO_INT_TYPE_FALLEDGE);	//����GPIO0[6]Ϊ�½��ش����ж�ģʽ
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 98, GPIO_INT_TYPE_FALLEDGE);//����GPIO6[1]Ϊ�½��ش����ж�ģʽ

    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);//��������GPIO0[15��0]�����жϣ���BINTEN[0]=1��BINTEN��ַΪ0x01E26008
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);//��������GPIO6[15��0]�����жϣ���BINTEN[6]=1��BINTEN��ַΪ0x01E26008

}


/*	����DSP�жϿ�����	*/

void SetupInt(void){

	// DSP�жϿ�������ʼ��
	IntDSPINTCInit();

	// DSPȫ���ж�ʹ��
	IntGlobalEnable();

}


/*	���ð����ж�	*/

void ConfigureIntGPIO(void){

	// ע�ᰴ���жϷ�����
	IntRegister(C674X_MASK_INT4, GPIO0_6_Isr);//ע��SW5�����жϷ���INT4�ж�
	IntRegister(C674X_MASK_INT5, GPIO6_1_Isr);//ע��SW6�����жϷ���INT5�ж�

	// ӳ�䰴���ж϶�ӦCPU�ж�
	IntEventMap(C674X_MASK_INT4, SYS_INT_GPIO_B0INT);//ӳ��SW5������ӦINT4�ж�
	IntEventMap(C674X_MASK_INT5, SYS_INT_GPIO_B6INT);//ӳ��SW6������ӦINT5�ж�

	// ʹ�ܰ�����Ӧ��CPU�ж�
	IntEnable(C674X_MASK_INT4);//ʹ��INT4�ж�
	IntEnable(C674X_MASK_INT5);//ʹ��INT5�ж�

}


/*	SW5�����жϷ���	*/

void GPIO0_6_Isr(void){

	printf("SW5 Down\n");

}


/*	SW6�����жϷ���	*/

void GPIO6_1_Isr(void){

	printf("SW6 Down\n");

}
