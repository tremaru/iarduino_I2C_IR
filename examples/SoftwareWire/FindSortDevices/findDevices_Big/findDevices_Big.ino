// ДАННЫЙ ПРИМЕР ИЩЕТ ВСЕ МОДУЛИ НА ШИНЕ I2C И ВЫВОДИТ ИНФОРМАЦИЮ О НИХ.    // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>                //
// Требуется установить библиотеку <iarduino_I2C_Address.h>                 //
//                                                                          //
// - Пример findDevices_Small  выводит короткую информацию о модулях.       //
// - Пример findDevices_Medium выводит количество и информацию о модулях.   //
// - Пример findDevices_Big    работает с модулями не выводящими название.  //
                                                                            //
const uint8_t sum = 10;                                                     //   Определяем количество модулей на шине I2C. Допускается указывать значение превышающее реальное количество модулей.
                                                                            //
#include <iarduino_I2C_Software.h>                                          //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Address.
SoftTwoWire sWire(3,4);                                                     //   Объявляем  объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                            //
#include <iarduino_I2C_Address.h>                                           //   Подключаем библиотеку для работы с адресами модулей линейки I2C-flash.
iarduino_I2C_Address obj[sum];                                              //   Объявляем  массив объектов (obj), указав количество (sum) модулей I2C-flash на шине I2C. Адреса модулей будут определены автоматически.
                                                                            //   Если вам известны адреса модулей, то массив (obj) можно определить адресами, например: iarduino_I2C_Address obj[5]={10,20,12,54,67}.
String fncName(uint8_t);                                                    //   Объявляем функцию подбора названия модуля по его идентификатору, на случай если модуль не поддерживает вывод своего названия.
                                                                            //
void setup(){                                                               //
     delay(500);                                                            // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                                                    //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
     while(!Serial){;}                                                      // * Ждём завершения инициализации шины UART.
                                                                            //
     Serial.println( F("-----------------") );                              // *
     Serial.print  ( F("Чтение... ") );                                     // *
                                                                            //
//   Находим модули:                                                        //
     for(uint8_t i=0; i<sum; i++){ obj[i].begin(&sWire); } // &Wire...      //   Функция begin() принимает ссылку на объект для работы с шиной I2C и проверяет существование на этой шине очередного модуля.
                                                                            //   После begin(), результат обнаружения модуля остаётся доступным для чтения: if( obj[i] ){/*модуль был обнаружен*/;}else{/*нет*/;}
//   Подсчитываем количество найденных модулей:                             //
     uint8_t num        = 0;                                                //
     uint8_t sumFlash   = 0;                                                // * Количество найденных модулей    из линейки flash-I2C.
     uint8_t sumUnknown = 0;                                                // * Количество найденных модулей НЕ из линейки flash-I2C.
     uint8_t sumDouble  = 0;                                                // * Количество адресов занятых несколькими модулями flash-I2C.
     while( obj[num] ){                                                     // *
         if( obj[num].getDevice()==DEVICE_I2C_FLASH     ){ sumFlash++;   }  // * Подсчитываем количество модулей    из линейки flash-I2C.
         if( obj[num].getDevice()==DEVICE_I2C_FLASH_OLD ){ sumFlash++;   }  // * Подсчитываем количество модулей    из линейки flash-I2C без поддержки вывода названия и сортировки адреса.
         if( obj[num].getDevice()==DEVICE_I2C_UNKNOWN   ){ sumUnknown++; }  // * Подсчитываем количество модулей НЕ из линейки flash-I2C.
         if( obj[num].getDevice()==DEVICE_I2C_DOUBLE    ){ sumDouble++;  }  // * Подсчитываем количество адресов занятых несколькими модулями flash-I2C.
         num++; if( num>=sum ){ break; }                                    // *
     }                                                                      // *
                                                                            //
//   Выводим количество найденных модулей:                                  //
     Serial.println( F("выполнено!")                       );               // *
     Serial.print  ( F("Найдено модулей: ")                );               // *
     Serial.print  ( num                                   );               // *
     Serial.println( '.'                                   );               // *
     Serial.print  ( F("Из них: Flash-I2C=")               );               // *
     Serial.print  ( sumFlash                              );               // *
     Serial.print  ( F(", неизвестные=")                   );               // *
     Serial.print  ( sumUnknown                            );               // *
     Serial.print  ( F(", дублированные адреса=")          );               // *
     Serial.print  ( sumDouble                             );               // *
     Serial.println( '.'                                   );               // *
                                                                            //
//   Выводим информацию о найденных модулях:                                //
     num = 0;                                                               //
     while( obj[num] ){                                                     //   Если модуль не существует, то obj[num] вернёт false.
     //  Выводим порядковый номер модуля:                                   //
         Serial.print( num+1                               );               // *
         Serial.print( F(".) ")                            );               // *
     //  Если модуль принадлежит линейке Flash I2C:                         //
         if( obj[num].getDevice()==DEVICE_I2C_FLASH                         //
         ||  obj[num].getDevice()==DEVICE_I2C_FLASH_OLD ){                  //
         //  Выводим название модуля:                                       //
             String str=obj[num].getName();                                 //   Читаем название из модуля в строку str.
             if( str.length()==0 ){ str=fncName( obj[num].getModel() ); }   //   Если строка str пустая, то подбираем название функцией fncName().
             Serial.print( (String) "Модуль «"+str+"»"     );               // *
         //  Выводим адрес модуля на шине I2C:                              //
             Serial.print( F(", адрес на шине I2C = 0x")   );               // 
             Serial.print( obj[num].getAddress(), HEX      );               // 
         //  Выводим номер модели:                                          //
             Serial.print( F(", идентификатор = 0x")       );               // *
             Serial.print( obj[num].getModel(), HEX        );               // *
         //  Выводим версию прошивки модуля:                                //
             Serial.print( F(", версия прошивки ")         );               // *
             Serial.print( obj[num].getVersion()           );               // *
         //  Выводим состояние внутрисхемной подтяжки модуля:               //
             Serial.print( F(", подтяжка линий шины I2C ") );               // *
             Serial.print( obj[num].getPullI2C()? "в":"от" );               // *
             Serial.print( F("ключена")                    );               // *
         }else                                                              //
     //  Если модуль не принадлежит линейке Flash I2C:                      //
         if( obj[num].getDevice()==DEVICE_I2C_UNKNOWN ){                    //
         //  Выводим адрес модуля не принадлежащего к линейе Flash I2C:     //
             Serial.print( F("Неизвестный модуль")         );               // 
             Serial.print( F(", адрес на шине I2C = 0x")   );               // 
             Serial.print( obj[num].getAddress(), HEX      );               // 
         }else                                                              //
     //  Если на адресе обнаружено несколько модулей Flash I2C:             //
         if( obj[num].getDevice()==DEVICE_I2C_DOUBLE ){                     //
         //  Выводим адрес занятый несколькими модулями Flash I2C:          //
             Serial.print( F("Адрес на шине I2C = 0x")     );               // 
             Serial.print( obj[num].getAddress(), HEX      );               // 
             Serial.print( F(" занят несколькими модулями"));               // 
         }                                                                  //
     //  Переходим к следующему модулю:                                     //
         Serial.println('.');                                               //
         num++; if( num>=sum ){ break; }                                    //
     }                                                                      //
}                                                                           //
                                                                            //
void loop(){                                                                //
}                                                                           //
                                                                            //
//     Функция подбора названия модуля по его идентификатору:               //
String fncName(uint8_t mod){                                                //
       String result="";                                                    //
       switch( mod ){                                                       //   Сравниваем номер модели модуля ...
       case 0x01: result = F("Кнопка"                             ); break; //   Если номер модели модуля равен 0x01, значит это кнопка.
       case 0x02: result = F("Светодиод"                          ); break; //   Если номер модели модуля равен 0x02, значит это RGB светодиод.
       case 0x03: result = F("Потенциометр"                       ); break; //   Если номер модели модуля равен 0x03, значит это потенциометр.
       case 0x04: result = F("Звукоизлучатель"                    ); break; //   Если номер модели модуля равен 0x04, значит это звукоизлучатель.
       case 0x05: result = F("Датчик температуры и влажности"     ); break; //   Если номер модели модуля равен 0x05, значит это датчик влажности и температуры.
       case 0x06: result = F("Датчик освещённости"                ); break; //   Если номер модели модуля равен 0x06, значит это датчик освещённости.
       case 0x07: result = F("Расширитель выводов"                ); break; //   Если номер модели модуля равен 0x07, значит это расширитель выводов.
       case 0x08: result = F("Светодиодная матрица 8x8"           ); break; //   Если номер модели модуля равен 0x08, значит это светодиодная матрица 8x8.
       case 0x09: result = F("Энкодер"                            ); break; //   Если номер модели модуля равен 0x09, значит это энкодер с поддержкой функции потенциометра.
       case 0x0A: result = F("Реле электромеханическое 2 канала"  ); break; //   Если номер модели модуля равен 0x0A, значит это электромеханическое реле на 2 канала.
       case 0x0B: result = F("Реле твердотельное на 4 канала"     ); break; //   Если номер модели модуля равен 0x0B, значит это твердотельное реле на 4 канала.
       case 0x0C: result = F("Силовые ключи 4N-канала, с измерен" ); break; //   Если номер модели модуля равен 0x0C, значит это силовой ключ на 4 N-канала.
       case 0x0D: result = F("Силовые ключи 4P-канала, с измерен" ); break; //   Если номер модели модуля равен 0x0D, значит это силовой ключ на 4 P-канала.
       case 0x0E: result = F("Силовые ключи 4N-канала"            ); break; //   Если номер модели модуля равен 0x0E, значит это силовой ключ на 4 N-канала, с измерением тока.
       case 0x0F: result = F("Силовые ключи 4P-канала"            ); break; //   Если номер модели модуля равен 0x0F, значит это силовой ключ на 4 P-канала, с измерением тока.
       case 0x10: result = F("Бампер с датчиками линий"           ); break; //   Если номер модели модуля равен 0x10, значит это бампер с датчиками линий.
       case 0x11: result = F("Джойстик"                           ); break; //   Если номер модели модуля равен 0x11, значит это джойстик.
       case 0x12: result = F("LCD дисплей"                        ); break; //   Если номер модели модуля равен 0x12, значит это адаптер для LCD дисплеев 1602/2004.
       case 0x13: result = F("Клавиатура"                         ); break; //   Если номер модели модуля равен 0x13, значит это клавиатура.
       case 0x14: result = F("Мотор-редуктор с драйвером"         ); break; //   Если номер модели модуля равен 0x14, значит это мотор.
       case 0x15: result = F("Реле электромеханическое на 1 канал"); break; //   Если номер модели модуля равен 0x15, значит это электромеханическое реле на 1 канал.
       case 0x16: result = F("LED индикатор 4 разрядный"          ); break; //   Если номер модели модуля равен 0x16, значит это 4-разрядный LED индикатор.
       case 0x17: result = F("ИК-приёмопередатчик"                ); break; //   Если номер модели модуля равен 0x17, значит это ИК-приёмопередатчик.
       case 0x18: result = F("ИК стойка светофора и знаков трассы"); break; //   Если номер модели модуля равен 0x18, значит это ИК-модуль трассы.
       case 0x19: result = F("TDS/EC-метр (солемер)"              ); break; //   Если номер модели модуля равен 0x19, значит это TDS/EC-метр.
       case 0x1A: result = F("pH-метр (датчик кислотности)"       ); break; //   Если номер модели модуля равен 0x1A, значит это pH-метр.
       case 0x1B: result = F("ORP-метр (датчик ОВП)"              ); break; //   Если номер модели модуля равен 0x1B, значит это ORP-метр.
       } return   result;                                                   //
}                                                                           //