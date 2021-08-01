// Rafael Hadzic
// 2020010302

#include <pic18f4520.h>
#include "atraso.h"
#include "lcd.h"
#include "keypad.h"
#include "ssd.h"
#include "bits.h"
#include "serial.h"
#include "pwm.h"
#include "lcd2.h"

//void lcd_result(int beat){
//    int i = 0;
//    lcdInit();
//    lcd_cmd(L_L1);
//    for (i = 0; i < 16; i++){
//        lcdData
//    }
//    
//}

void initial() {
    unsigned char i;
    lcd_init();
    lcd_cmd(L_L1);
    lcd_str("Iniciando...");
    atraso_ms(500);
    lcd_cmd(L_L2);
    lcd_str("Aguarde");
    atraso_ms(1000);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Aperte * para");
    lcd_cmd(L_L2);
    lcd_str("simular as");
    atraso_ms(2000);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("batidas do");
    lcd_cmd(L_L2);
    lcd_str("coracao");
    atraso_ms(2000);
    lcd_cmd(L_CLR);
    //    for (i = 0; i < 4; i++) {
    //        atraso_ms(500);
    //        lcd_cmd(0x1C); //lcd desliza pra direita
    //    }
    //    for (i = 0; i < 4; i++) {
    //        atraso_ms(500);
    //        lcd_cmd(0x18); //lcd desliza pra esquerda
    //    }
}

void main() {
    char i;
    unsigned int tecla = 16;
    int tempo = 0, beat = 0, l;
    char k, j;

    //    ADCON1 = 0x06;
    TRISA = 0xC3;
    TRISB = 0x03;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;

    initial();
    lcd_str("Medindo...");

    // O ritmo cardíaco é medido em 10 segundos, e em seguida multiplicado por 6
    serial_init();
    ssdInit();
    kpInit();
    pwmInit();
    tecla = kpRead();
    while (tempo < 4000) {
        tempo++;
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(kpRead(), 0)) {
                beat++;
                pwmFrequency(100 * 8);
                for (k = 0; k < 3; k++) {
                    for (j = 1; j > 0; j = j * 2) {
                        bitSet(TRISC, 1);
                        atraso_ms(40);
                        break;
                    }
                    bitClr(TRISC, 1);
                }
                serial_tx_str("\nIncrementacao realizada");
            }
        }
        ssdDigit(beat / 10, 0);
        ssdDigit(beat % 10, 1);
        ssdUpdate();
        for (i = 0; i < 255; i++);
    }
    int result = beat * 6;

    for (l = 0; l < 500; l++) {
        ssdInit();
        ssdDigit(result / 100, 0);
        ssdDigit((result / 10) % 10, 1);
        ssdDigit(result % 10, 2);
        ssdUpdate();
        for (i = 0; i < 255; i++);
    }
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Pressione");
    lcd_cmd(L_L2);
    lcd_str("reset");
    for(;;);
}