#ifndef _PIC_SYNTHCORE
#define _PIC_SYNTHCORE

#include <utils/byte_io.h>
#include <drivers/io/base_io.h>

void pic_remap() {
    outB(0x20, 0x11);  // ICW1: инициализация
    outB(0xA0, 0x11);

    outB(0x21, 0x20);  // ICW2: Master PIC -> INT 0x20-0x27
    outB(0xA1, 0x28);  // ICW2: Slave PIC -> INT 0x28-0x2F

    outB(0x21, 0x04);  // ICW3: Master PIC имеет Slave на IRQ2
    outB(0xA1, 0x02);  // ICW3: Slave PIC подключён к IRQ2 Master

    outB(0x21, 0x01);  // ICW4: 8086 mode
    outB(0xA1, 0x01);

    outB(0x21, 0x0);   // Размаскируем IRQ0 (таймер) и IRQ1 (клавиатура)
    outB(0xA1, 0x0);
}
#endif // _PIC_SYNTHCORE
