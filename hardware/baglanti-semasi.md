
# Shot Timer - Elektronik Bağlantılar

Bu belgede ESP32 Shot Timer projesinin elektronik bağlantıları açıklanmaktadır.

## 1. ESP32 ve OLED Ekran

Kullanılan ekran: 2.42 inç, 128x64, SSD1309 SPI OLED.

| OLED | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SCK / CLK | GPIO18 |
| SDA / MOSI | GPIO23 |
| CS | GPIO5 |
| DC | GPIO17 |
| RES / RST | GPIO16 |

## 2. MAX4466 Mikrofon

| MAX4466 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| OUT | GPIO34 |

Mikrofonun analog çıkışı GPIO34 üzerinden okunur.

Yazılımda başlangıç algılama eşiği 120 olarak tanımlanmıştır. Bu değer seri monitör üzerinden değiştirilebilir.

## 3. START Butonu

| Bağlantı | Pin |
|---|---|
| START butonu 1. uç | GPIO27 |
| START butonu 2. uç | GND |

GPIO27 dahili pull-up direnciyle kullanılır.

Kısa basış zamanlayıcıyı yeniden başlatır.
3 saniye uzun basış Deep Sleep moduna geçirir.

## 4. Buzzer ve Transistör

Kullanılan bileşenler:
- 5V aktif buzzer
- 2N2222 NPN transistör
- 1 kΩ direnç

ESP32 GPIO25, transistörün bazını 1 kΩ direnç üzerinden kontrol eder.

Buzzer başlangıç sinyali 300 ms sürer.

Not: Transistörün bacak dizilimi üreticiye ve kılıfa göre doğrulanmalıdır. Nihai kolektör, emiter ve buzzer güç bağlantıları fiziksel devre üzerinden kontrol edilerek tamamlanacaktır.

## 5. LiPo Batarya ve FM5324

Kullanılan batarya:
- 3.7V LiPo
- 3000 mAh
- Tek hücreli (1S)

Batarya bağlantıları:

| Batarya | FM5324 |
|---|---|
| Pozitif (+) | B+ |
| Negatif (-) | B- |

FM5324'ün 5V çıkışı ESP32'nin uygun 5V besleme girişine bağlanır.

FM5324 ve ESP32 ortak GND kullanır.

FM5324'ün K kontrol girişi, START butonuyla güç modülünü uyandırmak için kullanılmaktadır.

K girişinin GPIO27 ile doğrudan birleştirilmesi önceki testlerde sorun oluşturmuştur. Nihai izolasyon bağlantısı doğrulanmadan bu iki hat doğrudan birleştirilmemelidir.

## 6. ESP32 Pin Özeti

| ESP32 GPIO | Görev |
|---|---|
| GPIO5 | OLED CS |
| GPIO16 | OLED RESET |
| GPIO17 | OLED DC |
| GPIO18 | OLED SCK |
| GPIO23 | OLED MOSI |
| GPIO25 | Buzzer kontrol |
| GPIO27 | START butonu |
| GPIO34 | Mikrofon analog giriş |

## Güvenlik Notları

- LiPo batarya doğrudan ESP32'nin 3V3 pinine bağlanmamalıdır.
- Güç bağlantıları yapılırken batarya ayrılmalıdır.
- FM5324 K girişine doğrudan 5V veya batarya pozitif gerilimi uygulanmamalıdır.
- Bağlantılar tamamlandıktan sonra kısa devre kontrolü yapılmalıdır.
