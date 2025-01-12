#include <system.h>

// number of ticks the system has been up for
int timer_ticks = 0;

// increment timer_ticks every time the timer fires
void timer_handler(struct registers* r) {
    // the timer ticks ~18 times per second
    timer_ticks++;
    // if (timer_ticks % 18 == 0) {
    //     put_str("tick\n");
    // }
}

// isntalls timer_handler into IRQ0
void timer_install() {
    irq_install_handler(0, timer_handler);
}