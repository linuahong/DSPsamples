
/*	头文件	 */
#include "hw_types.h"				// HWREG（x）
#include "hw_syscfg0_OMAPL138.h"	// SYSCFG0_PINMUX(x)
#include "soc_OMAPL138.h"			// SOC_GPIO_0_REGS
#include "gpio.h"					// GPIO_DIR_OUTPUT

/*	函数声明	 */
void GPIOBankPinInit(void);//GPIO初始化函数声明



/*	主函数	*/
void main(void)
{
	unsigned int i;

    GPIOBankPinInit();//GPIO初始化

    /*	跑马灯控制程序	*/
    for(;;)
    {
    	for(i=0x00FFFFFF;i>0;i--);//延时
    	GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_LOW);//D10灭,GPIO0[2]
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_HIGH);//D7亮,GPIO0[0]

		for(i=0x00FFFFFF;i>0;i--);//延时
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_LOW);//D7灭,GPIO0[0]
		GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_HIGH);//D6亮,GPIO0[5]

		for(i=0x00FFFFFF;i>0;i--);//延时
		GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_LOW);//D6灭,GPIO0[5]
		GPIOPinWrite(SOC_GPIO_0_REGS, 2, GPIO_PIN_HIGH);//D9亮,GPIO0[1]

		for(i=0x00FFFFFF;i>0;i--);//延时
		GPIOPinWrite(SOC_GPIO_0_REGS,2, GPIO_PIN_LOW);//D9灭,GPIO0[1]
		GPIOPinWrite(SOC_GPIO_0_REGS,3, GPIO_PIN_HIGH);//D10亮,GPIO0[2]
    }
}


/*	GPIO初始化	*/
void GPIOBankPinInit(void)
{
	unsigned int PINMUX_1_VAL;//局部变量

	/*	读取PINMUX1寄存器的值	*/
	PINMUX_1_VAL=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1));
	/*	将底板4个用户led的对应管脚复用为GPIO，即GPIO0[0]、GPIO0[1]、GPIO0[2]、GPIO0[5]	*/
	PINMUX_1_VAL=(PINMUX_1_VAL&0x000ff0ff)|0x88800800;
	/*	将设置后的值写回PINMUX1寄存器	*/
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1))=PINMUX_1_VAL;


	/*配置底板4个用户led的对应管脚为输出,以下为各组 GPIO第一个管脚 的对应值(范围 1~144)
	 * GPIO0[0] 1	，GPIO1[0] 17
	 * GPIO2[0] 33	，GPIO3[0] 49
	 * GPIO4[0] 65	，GPIO5[0] 81
	 * GPIO6[0] 97	，GPIO7[0] 113
	 * GPIO8[0] 129	*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 1, GPIO_DIR_OUTPUT);//GPIO0[0]
    GPIODirModeSet(SOC_GPIO_0_REGS, 2, GPIO_DIR_OUTPUT);//GPIO0[1]
    GPIODirModeSet(SOC_GPIO_0_REGS, 3, GPIO_DIR_OUTPUT);//GPIO0[2]
    GPIODirModeSet(SOC_GPIO_0_REGS, 6, GPIO_DIR_OUTPUT);//GPIO0[5]


    /*	初始化底板4个用户led状态为灭	*/
    GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_LOW);//D7灭,GPIO0[0]
    GPIOPinWrite(SOC_GPIO_0_REGS, 2, GPIO_PIN_LOW);//D9灭,GPIO0[1]
    GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_LOW);//D10灭,GPIO0[2]
    GPIOPinWrite(SOC_GPIO_0_REGS, 6, GPIO_PIN_LOW);//D6灭,GPIO0[5]
}
