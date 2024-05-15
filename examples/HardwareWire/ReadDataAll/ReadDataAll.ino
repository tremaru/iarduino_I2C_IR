// ПОДУЧЕНИЕ ДАННЫХ С ИК ПУЛЬТА ДУ NEC:             // * Строки со звёздочкой являются необязательными.
// (реагируем на нажатия и удержание кнопок)        //
                                                    //
// ИК-приемник/передатчик, FLASH-I2C (Роботраффик): //   https://iarduino.ru/shop/Expansion-payments/ik-priemnik-peredatchik-flash-i2c.html
// Информация о подключении модулей к шине I2C:     //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:       //   https://wiki.iarduino.ru/page/ir-i2c/
                                                    //
#include <Wire.h>                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_IR.
#include <iarduino_I2C_IR.h>                        //   Подключаем библиотеку для работы с ИК-приёмником/передатчиком I2C-flash.
iarduino_I2C_IR ir(0x09);                           //   Объявляем объект ir   для работы с функциями и методами библиотеки iarduino_I2C_IR, указывая адрес модуля на шине I2C.
                                                    //   Если объявить объект без указания адреса (iarduino_I2C_IR ir;), то адрес будет найден автоматически.
void setup(){                                       //
     delay(500);                                    // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                            //   Инициируем работу с шиной UART для передачи данных в монитор последовательного порта на скорости 9600 бит/сек.
     ir.begin(&Wire); // &Wire1, &Wire2 ...         //   Инициируем работу с ИК-приёмником/передатчиком, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
     ir.setProtocol(IR_NEC);                        //   Указываем протокол для приёма/передачи данных по ИК-каналу. Доступные протоколы: IR_NEC, IR_SAMSUNG, IR_SHARP, IR_SIRC12, IR_SIRC15, IR_RC5, IR_RC5X, IR_RC6, IR_IARDUINO.
     ir.setInterval(0);                             // * Указываем использовать интервал между пакетами повторов в соответствии с указанным ИК протоколом. Модуль по умолчанию использует интервалы ИК протоколов.
}                                                   //
                                                    //
void loop(){                                        //
     if( ir.check(true) ){                          //   Если ИК приёмник получил пакет данных (нажимается кнопка ИК пульта) или пакет повтора (удерживается кнопка ИК пульта).
         Serial.print("Адрес=");                    //   Выводим текст.
         Serial.print(ir.address);                  //   Выводим принятый адрес ИК-устройства.
         Serial.print(", команда=");                //   Выводим текст.
         Serial.print(ir.command);                  //   Выводим принятую команду для ИК-устройства.
         Serial.print(", кнопка ");                 //   Выводим текст.
         if( ir.key_press ){                        //   Если функция check() среагировала на первый пакет данных, а не на пакет повтора (кнопка пульта нажимается а не удерживается).
             Serial.print("нажимается");            //   Выводим текст.
         }else{                                     //   Если функция check() среагировала на пакет повтора, а не на первый пакет данных (кнопка пульта удерживается а не нажимается).
             Serial.print("удерживается");          //   Выводим текст.
         }                                          //
         Serial.println(".");                       //   Выводим текст.
         delay(100);                                // * Ждём 100 мс.
     }                                              //
}                                                   //