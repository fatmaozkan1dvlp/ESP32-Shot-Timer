
# Shot Timer - Malzeme Listesi

Bu belgede ESP32 Shot Timer projesinde kullanılan elektronik bileşenler ve görevleri açıklanmaktadır.

## Ana Bileşenler

| Adet | Malzeme | Özellik | Görevi |
|---|---|---|---|
| 1 | ESP32-WROOM-32 DevKit | 38 pin | Ana mikrodenetleyici |
| 1 | OLED ekran | 2.42 inç, 128x64, SSD1309, SPI | Zaman ve atış bilgilerini gösterir |
| 1 | MAX4466 mikrofon modülü | Analog çıkışlı | Atış seslerini algılar |
| 1 | FM5324 güç modülü | LiPo şarj ve 5V yükseltici | Güç yönetimi |
| 1 | LiPo batarya | 3.7V, 3000 mAh | Taşınabilir güç kaynağı |
| 1 | Aktif buzzer | 5V | Başlangıç sesli uyarısı |
| 1 | 2N2222 transistör | NPN | Buzzer sürücüsü |
| 1 | Direnç | 1 kΩ | Transistör baz akımını sınırlar |
| 1 | START butonu | 2 bacaklı, anlık basmalı | Cihaz kontrolü |
| 1 | 3D baskı kutu | Özel tasarım | Elektronik muhafazası |
| 1 | 3D baskı kapak | Özel tasarım | Kutunun kapağı |

## Yardımcı Malzemeler

- Elektronik bağlantı kabloları
- Lehim ve lehimleme ekipmanları
- USB kablosu
- Montaj için gerekli bağlantı elemanları

## Notlar

- OLED ekran SPI haberleşmesi kullanır.
- Mikrofonun analog çıkışı ESP32 tarafından okunur.
- Buzzer, transistör üzerinden kontrol edilir.
- Batarya FM5324 güç modülüne bağlanır.
- START butonu zamanlayıcıyı başlatmak ve Deep Sleep modunu kontrol etmek için kullanılır.
