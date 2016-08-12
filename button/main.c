//
/*	头文件	 */

#include <stdio.h>
#include "hw_types.h"				// HWREG（x）
#include "hw_syscfg0_OMAPL138.h"	// SYSCFG0_PINMUX(x)
#include "soc_OMAPL138.h"			// SOC_GPIO_0_REGS
#include "gpio.h"					// GPIO_DIR_INTPUT
#include "interrupt.h"


/*	函数声明	 */

void GPIOBankPinInit(void);			//GPIO初始化
void SetupInt(void);				//设置中断控制器
void ConfigureIntGPIO(void);		//配置按键中断
void GPIO0_6_Isr(void);				//SW5按键中断服务
void GPIO6_1_Isr(void);				//SW6按键中断服务


/*	主函数	*/

void main(void) {

	GPIOBankPinInit();
	SetupInt();
	ConfigureIntGPIO();
	while(1);			//等待按键中断执行

}


/*	GPIO初始化	*/

void GPIOBankPinInit(void){

	unsigned int PINMUX_1_VAL;
	unsigned int PINMUX_19_VAL;

	PINMUX_1_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1));	//读取PINMUX1寄存器的值
	PINMUX_19_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(19));//读取PINMUX19寄存器的值

	PINMUX_1_VAL=(PINMUX_1_VAL&0xffffff0f)|0x00000080;			//将底板按键SW5的对应管脚复用为GPIO，即GPIO0[6]
	PINMUX_19_VAL=(PINMUX_19_VAL&0xff0fffff)|0x00800000;		//将底板按键SW6的对应管脚复用为GPIO，即GPIO6[1]

	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1))=PINMUX_1_VAL;	//0x01C14124,PINMUX1[7-4]=0x8h
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(19))=PINMUX_19_VAL;//0x01C1416C,PINMUX19[23-20]=0x8h

	/*配置底板2个用户按键的对应管脚为输入，以下为各组 GPIO第一个管脚 的对应值(范围 1~144)
	 * GPIO0[0] 1	，GPIO1[0] 17
	 * GPIO2[0] 33	，GPIO3[0] 49
	 * GPIO4[0] 65	，GPIO5[0] 81
	 * GPIO6[0] 97	，GPIO7[0] 113
	 * GPIO8[0] 129	*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_INPUT);	//设置GPIO0[6]为输入模式，即DIR01[6]=1,DIR01地址为0x01E26010
    GPIODirModeSet(SOC_GPIO_0_REGS, 98, GPIO_DIR_INPUT);//设置GPIO6[1]为输入模式，即DIR67[1]=1，DIR67地址为0x01E26088

    GPIOIntTypeSet(SOC_GPIO_0_REGS, 7, GPIO_INT_TYPE_FALLEDGE);	//设置GPIO0[6]为下降沿触发中断模式
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 98, GPIO_INT_TYPE_FALLEDGE);//设置GPIO6[1]为下降沿触发中断模式

    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);//设置允许GPIO0[15：0]产生中断，即BINTEN[0]=1，BINTEN地址为0x01E26008
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);//设置允许GPIO6[15：0]产生中断，即BINTEN[6]=1，BINTEN地址为0x01E26008

}


/*	设置DSP中断控制器	*/

void SetupInt(void){

	// DSP中断控制器初始化
	IntDSPINTCInit();

	// DSP全局中断使能
	IntGlobalEnable();

}


/*	配置按键中断	*/

void ConfigureIntGPIO(void){

	// 注册按键中断服务函数
	IntRegister(C674X_MASK_INT4, GPIO0_6_Isr);//注册SW5按键中断服务到INT4中断
	IntRegister(C674X_MASK_INT5, GPIO6_1_Isr);//注册SW6按键中断服务到INT5中断

	// 映射按键中断对应CPU中断
	IntEventMap(C674X_MASK_INT4, SYS_INT_GPIO_B0INT);//映射SW5按键对应INT4中断
	IntEventMap(C674X_MASK_INT5, SYS_INT_GPIO_B6INT);//映射SW6按键对应INT5中断

	// 使能按键对应的CPU中断
	IntEnable(C674X_MASK_INT4);//使能INT4中断
	IntEnable(C674X_MASK_INT5);//使能INT5中断

}


/*	SW5按键中断服务	*/

void GPIO0_6_Isr(void){

	printf("SW5 Down\n");

}


/*	SW6按键中断服务	*/

void GPIO6_1_Isr(void){

	printf("SW6 Down\n");

}
