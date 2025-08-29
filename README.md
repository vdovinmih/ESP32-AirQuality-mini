![Static Badge](https://img.shields.io/badge/No-AI-blue)

# ESP32-AirQuality-mini
AirQuality zigbee mini sensor ESP32

Цели и задачи устройства:
1. измерять температуру, влажность, и показатель качества воздуха (TVOС)
2. по протоколу zigbee передавать эти показания в систему умного дома на основе z2m
3. [опция] выступать роутером в zigbee сети
4. [опция] передавать измеренные значения напрямую в устройства одной zigbee сети минуя координатор и сервер умного дома
 

Основные комплектующие: 
1. ESP32- C6/H2 SuperMini 
https://aliexpress.ru/item/1005006907067134.html?spm=a2g2w.orderdetail.0.0.60bb4aa6OtokH4&sku_id=12000041762699861
https://aliexpress.ru/item/1005008148472502.html?sku_id=12000043993594567&spm=a2g2w.productlist.search_results.0.532a1736lb0FJ2
https://github.com/Zektopic/esp32-h2-supermini-kicad?ysclid=memrikceaz898473666

2. Датчик температуры и влажности AHT10 (i2c)
https://aliexpress.ru/item/1005008699139931.html?sku_id=12000046300583727&spm=a2g2w.productlist.search_results.1.28c04050xA3ERq

3. AGS10 Модуль обнаружения газа (i2c)
https://aliexpress.ru/item/1005008593136272.html?sku_id=12000048412147624&spm=a2g2w.productlist.search_results.1.490f4050ZTz7pN

4. [опция] на площадки модуля B+ B- опционально можно подключить 3.7в LiIon АКБ совмещённый с модулем защиты
https://aliexpress.ru/item/1005004304618610.html?sku_id=12000028691739753&spm=a2g2w.productlist.search_results.1.18de2d01bsl4up

на модули датчиков питание 3.3в подаётся с ESP32-H2, +2 пина i2c, я использовал (H2 модуль) 13, 14. Но 13 нога висит в паралель с диодом. Лучше использовать другую пару например 11,12
 
Питание устройства:
1. Интегрированный на ESP32 модуле разъём usb-c
2. пара клемм на 12 вольт (при наличии преобразователя 12в -> 5в)
