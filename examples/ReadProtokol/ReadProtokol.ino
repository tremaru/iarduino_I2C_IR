// ЧТЕНИЕ ПРОТОКОЛА ОТ ИК ПУЛЬТА ДУ:                // * Строки со звёздочкой являются необязательными.
                                                    //   Библиотека поддерживает пульты использующие протоколы: NEC, SAMSUNG, SHARP, SONY (SIRC12, SIRC15), PHILIPS (RC5, RC5X, RC6), IARDUINO.
#include <Wire.h>                                   // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_IR.h>                        //   Подключаем библиотеку для работы с ИК-приёмником/передатчиком I2C-flash.
iarduino_I2C_IR ir(0x09);                           //   Объявляем объект ir   для работы с функциями и методами библиотеки iarduino_I2C_IR, указывая адрес модуля на шине I2C.
                                                    //   Если объявить объект без указания адреса (iarduino_I2C_IR ir;), то адрес будет найден автоматически.
void setup(){                                       //
     delay(500);                                    // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                            //   Инициируем работу с шиной UART для передачи данных в монитор последовательного порта на скорости 9600 бит/сек.
     ir.begin();                                    //   Инициируем работу с ИК-приёмником/передатчиком I2C-flash.
     Serial.println("Нажмите кнопку пульта");       //   Выводим текст.
     Serial.println("не менее чем на 0,5 секунд."); //   Выводим текст.
//   Указываем модулю искать ИК-протокол:           //
     ir.seeProtocol_RX();                           //   Переводим ИК приёмник в режим определения протокола получаемых данных.
//   Ждём обнаружение ИК-протокола:                 //
     while( ir.getProtocol_RX()==IR_UNDEFINED ){;}  //   Выполняем цикл while пока ИК-протокол приёмника не станет отличным от значения IR_UNDEFINED.
//   Далее код будет выполнен если протокол найден: //
     Serial.print("Обнаружен протокол ");           //   Выводим текст.
     switch(ir.getProtocol_RX()){                   //
        case IR_NEC:      Serial.print("NEC");      break; // Обнаружен протокол NEC.
        case IR_SAMSUNG:  Serial.print("SAMSUNG");  break; // Обнаружен протокол SAMSUNG.
        case IR_SHARP:    Serial.print("SHARP");    break; // Обнаружен протокол SHARP.
        case IR_SIRC12:   Serial.print("SIRC12");   break; // Обнаружен протокол SONY.
        case IR_SIRC15:   Serial.print("SIRC15");   break; // Обнаружен протокол SONY.
        case IR_RC5:      Serial.print("RC5");      break; // Обнаружен протокол PHILIPS.
        case IR_RC5X:     Serial.print("RC5X");     break; // Обнаружен протокол PHILIPS.
        case IR_RC6:      Serial.print("RC6");      break; // Обнаружен протокол PHILIPS.
        case IR_IARDUINO: Serial.print("IARDUINO"); break; // Обнаружен протокол IARDUINO.
     }                    Serial.print(".\r\n");    //
     ir.setProtocol(ir.getProtocol_RX(),ir.modeRC6);// * Указываем использовать обнаруженный ИК протокол не только для приёма, но и для передачи данных по ИК-каналу.
}                                                   //
                                                    //
void loop(){                                        //
     if( ir.check(true) ){                          //   Если ИК приёмник получил пакет данных (нажимается кнопка ИК пульта) или пакет повтора (удерживается кнопка ИК пульта).
         Serial.print("Адрес=");                    //   Выводим текст.
         Serial.print(ir.address);                  //   Выводим принятый адрес ИК-устройства.
         Serial.print(", команда=");                //   Выводим текст.
         Serial.print(ir.command);                  //   Выводим принятую команду для ИК-устройства.
         Serial.println(".");                       //   Выводим текст.
         delay(100);                                // * Ждём 100 мс.
     }                                              //
}                                                   //