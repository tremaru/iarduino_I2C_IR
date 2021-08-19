// ОПРЕДЕЛЕНИЕ ЗНАКОВ И СВЕТОФОРА ТРАССЫ:      // * Строки со звёздочкой являются необязательными.
                                               //
#include <Wire.h>                              // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_IR.h>                   //   Подключаем библиотеку для работы с ИК-приёмником/передатчиком I2C-flash.
iarduino_I2C_IR ir(0x09);                      //   Объявляем объект ir   для работы с функциями и методами библиотеки iarduino_I2C_IR, указывая адрес модуля на шине I2C.
                                               //   Если объявить объект без указания адреса (iarduino_I2C_IR ir;), то адрес будет найден автоматически.
void setup(){                                  //
     delay(500);                               // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                       //   Инициируем работу с шиной UART для передачи данных в монитор последовательного порта на скорости 9600 бит/сек.
     ir.begin();                               //   Инициируем работу с ИК-приёмником/передатчиком I2C-flash.
     ir.setProtocol(IR_IARDUINO);              //   Указываем протокол для приёма/передачи данных по ИК-каналу.
//   Отправляем данные другим автомобилям:     //
//   ir.setInterval_TX(200, true);             //   Указываем модулю использовать интервал между пакетами передаваемых данных в 200 мс, разрешая менять его при передаче в пределах ±50%.
//   ir.autoSend(MODUL_CAR, 10);               //   Указываем модулю постоянно отправлять данные с адресом MODUL_CAR и командой 10, пока не будет вызван метод ir.autoStop().
}                                              //   В качестве команды автомобиля (вместо 10) можно указывать любое число от 0 до 255.
                                               //
void loop(){                                   //
//   Если приняты данные:                      //
     if( ir.check(true) ){                     //   Если ИК приёмник получил пакет данных или пакет повтора.
     //  Проверяем тип модуля трассы:          //
         switch( ir.device ){                  //   Тип модуля трассы хранится в переменной ir.device
             case MODUL_TLIGHT:                //   Если в переменной ir.device хранится значение MODUL_TLIGHT, значит данные получены от светофора.
             //  ДАННЫЕ ПОЛУЧЕНЫ ОТ СВЕТОФОРА: //
                 Serial.print("На светофоре ");//
                 Serial.print("есть дорога "); //
                 if( ir.track_L ){ Serial.print("налево, "  ); }
                 if( ir.track_R ){ Serial.print("направо, " ); }
                 if( ir.track_F ){ Serial.print("прямо, "   ); }
                 Serial.print("сигналы указывают");
                 Serial.print(": прямо ");     //
                 switch(ir.forvard){           case 0: Serial.print("нельзя"              ); break;
                                               case 1: Serial.print("можно"               ); break;
                                               case 2: Serial.print("можно если нет помех"); break;
                 }                             //
                 Serial.print(", направо ");   //
                 switch(ir.right){             case 0: Serial.print("нельзя"              ); break;
                                               case 1: Serial.print("можно"               ); break;
                                               case 2: Serial.print("можно если нет помех"); break;
                 }                             //
                 Serial.print(", налево ");    //
                 switch(ir.left){              case 0: Serial.print("нельзя"              ); break;
                                               case 1: Serial.print("можно"               ); break;
                                               case 2: Serial.print("можно если нет помех"); break;
                 }                             //
                 Serial.println(".");          //
             break;                            //
             case MODUL_SIGN:                  //   Если в переменной ir.device хранится значение MODUL_SIGN, значит данные получены от знака дорожного движения.
             //  ДАННЫЕ ПОЛУЧЕНЫ ОТ ЗНАКА:     //
                 Serial.print("Знак ПДД ");    //
                 Serial.print(ir.sign_str);    //   Выводим номер знака строкой, например, "2.1".
                 Serial.print(" \"");          //   Выводим название дорожного знака текстом:
                      if( ir.sign_int == 12200 ){   Serial.print(F("Пешеходный переход")                       ); } // Знак ПДД 1.22  (1.22.00).
                 else if( ir.sign_int == 12300 ){   Serial.print(F("Дети")                                     ); } // Знак ПДД 1.23  (1.23.00).
                 else if( ir.sign_int == 20100 ){   Serial.print(F("Главная дорога")                           ); } // Знак ПДД 2.1   (2.01.00).
                 else if( ir.sign_int == 20200 ){   Serial.print(F("Конец главной дороги")                     ); } // Знак ПДД 2.2   (2.02.00).
                 else if( ir.sign_int == 20301 ){   Serial.print(F("Пересечение со второстепенной дорогой")    ); } // Знак ПДД 2.3.1 (2.03.01).
                 else if( ir.sign_int == 20400 ){   Serial.print(F("Уступите дорогу")                          ); } // Знак ПДД 2.4   (2.04.00).
                 else if( ir.sign_int == 32405 ){   Serial.print(F("Ограничение максимальной скорости  50км/ч")); } // Знак ПДД 3.24  (3.24.05) последнее число 05 используется для указания скорости.
                 else if( ir.sign_int == 32406 ){   Serial.print(F("Ограничение максимальной скорости  60км/ч")); } // Знак ПДД 3.24  (3.24.06) последнее число 06 используется для указания скорости.
                 else if( ir.sign_int == 32409 ){   Serial.print(F("Ограничение максимальной скорости  90км/ч")); } // Знак ПДД 3.24  (3.24.09) последнее число 09 используется для указания скорости.
                 else if( ir.sign_int == 32412 ){   Serial.print(F("Ограничение максимальной скорости 120км/ч")); } // Знак ПДД 3.24  (3.24.12) последнее число 12 используется для указания скорости.
                 else if( ir.sign_int == 32700 ){   Serial.print(F("Остановка запрещена")                      ); } // Знак ПДД 3.27  (3.27.00).
                 else if( ir.sign_int == 32800 ){   Serial.print(F("Стоянка запрещена")                        ); } // Знак ПДД 3.28  (3.28.00).
                 else if( ir.sign_int == 40201 ){   Serial.print(F("Объезд препятствия справа")                ); } // Знак ПДД 4.2.1 (4.02.01).
                 else if( ir.sign_int == 40202 ){   Serial.print(F("Объезд препятствия слева")                 ); } // Знак ПДД 4.2.2 (4.02.02).
                 else                           {   Serial.print(F("Неизвестный знак")                         ); } // Знак ПДД от 1.1 до 7.63.15, текст которого не определён выше.
                 Serial.print("\"");           //
             //  Сравнивать номер знака можно и так: if( ir.sign[0]==4 && ir.sign[1]==2 && ir.sign[2]==1 ){ ... }   // Знак ПДД 4.2.1
             //                                      if( ir.sign[0]==3 && ir.sign[1]==24                 ){ ... }   // Знак ПДД 3.24, скорость = ir.sign[2]*10
             //  Проверяем наличие таблички под знаком:
                 if( ir.sign_tab == SIGN_CAR_TRUCK    ){ Serial.print(F(" действие знака распространяется только на грузовые ам.")); }
                 if( ir.sign_tab == SIGN_CAR_LIGHT    ){ Serial.print(F(" действие знака распространяется только на легковые ам.")); }
                 if( ir.sign_tab == SIGN_DISTANCE_050 ){ Serial.print(F(" расстояние до объекта или зона действия знака = 50 м." )); }
                 if( ir.sign_tab == SIGN_DISTANCE_100 ){ Serial.print(F(" расстояние до объекта или зона действия знака = 100 м.")); }
                 if( ir.sign_tab == SIGN_DISTANCE_150 ){ Serial.print(F(" расстояние до объекта или зона действия знака = 150 м.")); }
                 if( ir.sign_tab == SIGN_DISTANCE_200 ){ Serial.print(F(" расстояние до объекта или зона действия знака = 200 м.")); }
                 if( ir.sign_tab == SIGN_DISTANCE_250 ){ Serial.print(F(" расстояние до объекта или зона действия знака = 250 м.")); }
                 Serial.println(".");          //
             break;                            //
             case MODUL_CAR:                   //   Если в переменной ir.device хранится значение MODUL_CAR, значит данные получены от другой машины.
             //  ДАННЫЕ ПОЛУЧЕНЫ ОТ МАШИНЫ:    //
                 Serial.print("Машина ");      //
                 Serial.print("передаёт ");    //   Вне зависимости ит номера передаваемой команды,
                 Serial.print("команду ");     //   наличие машины нужно расценивать как возможная помеха.
                 Serial.print(ir.command);     //   Например, при повороте налево по светофору.
                 Serial.println(".");          //
             break;                            //
             default:                          //   Если в переменной ir.device хранится значение отличное от описанных выше,
             //  НЕИЗВЕСТНЫЙ ИСТОЧНИК ДАННЫХ:  //   значит это сторонний модуль, например, пульт ДУ, использующий тот же протокол что и модули трассы.
                 Serial.print("Неизвестный "); //
                 Serial.print("источник ");    //
                 Serial.print("данных");       //
                 Serial.println(".");          //
             break;                            //
         }                                     //
     }                                         //
}                                              //