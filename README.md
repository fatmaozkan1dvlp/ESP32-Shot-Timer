
# ESP32 Shot Timer

ESP32 tabanlı, OLED ekranlı, ses algılamalı ve taşınabilir bir atış zamanlayıcı (Shot Timer) projesidir.

Bu proje; elektronik devre, gömülü yazılım ve özel tasarlanmış 3D baskı kutusundan oluşmaktadır.

## Proje Hakkında

Shot Timer, başlangıç sesli uyarısından sonra mikrofon aracılığıyla algılanan atışların zamanlarını hesaplayan bir cihazdır.

Atış süreleri OLED ekranda saniye ve milisaniye cinsinden gösterilir. Cihaz tek START butonuyla kontrol edilir ve LiPo batarya ile çalışır.

## Özellikler

- ESP32-WROOM-32 mikrodenetleyici
- 2,42 inç, 128x64 SSD1309 OLED ekran
- MAX4466 analog mikrofon
- 3 saniyelik başlangıç geri sayımı
- 300 ms başlangıç bip sesi
- En fazla 10 atış kaydı
- Milisaniye cinsinden zaman gösterimi
- İlk atış süresi (SPLIT)
- Son atışın toplam süresi (TIME)
- 180 ms yeniden algılama bekleme süresi
- Ayarlanabilir mikrofon hassasiyeti
- Tek START butonuyla kontrol
- Deep Sleep güç tasarrufu
- 3000 mAh LiPo batarya
- Özel tasarlanmış 3D baskı kutusu
- Arduino IDE ile programlama

## Kullanılan Malzemeler

| Adet | Malzeme | Özellik |
|---|---|---|
| 1 | ESP32-WROOM-32 DevKit | 38 pin |
| 1 | OLED ekran | 2,42 inç, SSD1309, SPI, 128x64 |
| 1 | MAX4466 mikrofon modülü | Analog çıkış |
| 1 | FM5324 güç modülü | LiPo şarj ve 5V yükseltici |
| 1 | LiPo batarya | 3,7V, 3000 mAh |
| 1 | Aktif buzzer | 5V |
| 1 | 2N2222 NPN transistör | Buzzer sürücüsü |
| 1 | Direnç | 1 kΩ |
| 1 | START butonu | 2 bacaklı, anlık basmalı |
| 1 | 3D baskı kutusu | Özel tasarım |
| - | Bağlantı kabloları | Elektronik montaj |

Ayrıntılı malzeme listesi: [Malzeme Listesi](hardware/malzeme-listesi.md)

## ESP32 Pin Bağlantıları

| ESP32 pini | Bağlanan bileşen |
|---|---|
| GPIO5 | OLED CS |
| GPIO16 | OLED RESET |
| GPIO17 | OLED DC |
| GPIO18 | OLED SCK |
| GPIO23 | OLED MOSI |
| GPIO25 | Buzzer kontrolü |
| GPIO27 | START butonu |
| GPIO34 | MAX4466 mikrofon OUT |
| 3V3 | OLED ve mikrofon beslemesi |
| GND | Ortak toprak |

### OLED Ekran Bağlantıları

| OLED | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SCK / CLK | GPIO18 |
| SDA / MOSI | GPIO23 |
| CS | GPIO5 |
| DC | GPIO17 |
| RES / RST | GPIO16 |

### Mikrofon Bağlantıları

| MAX4466 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| OUT | GPIO34 |

### START Butonu

- Bir uç: GPIO27
- Diğer uç: GND

GPIO27, yazılımda dahili pull-up ile kullanılır.

### Buzzer

GPIO25, 1 kΩ direnç üzerinden 2N2222 transistörün bazını kontrol eder. Buzzer, transistör üzerinden sürülür.

Transistörün bacak dizilimi ve güç bağlantıları kullanılan fiziksel devreye göre doğrulanmalıdır.

### Batarya ve Güç Modülü

- LiPo batarya pozitif (+): FM5324 B+
- LiPo batarya negatif (-): FM5324 B-
- FM5324 5V çıkışı: ESP32 uygun 5V besleme girişi
- GND: Ortak toprak

**Önemli:** FM5324'ün K kontrol girişinin GPIO27'ye doğrudan bağlanması önceki testlerde güç modülünün yeniden açılmasını engellemiştir. Nihai güç uyandırma bağlantısı doğrulanmadan bu iki hat doğrudan birleştirilmemelidir.

Ayrıntılı bağlantılar: [Elektronik Bağlantılar](hardware/baglanti-semasi.md)

## Cihazın Çalışma Mantığı

### 1. Başlatma

START butonuna kısa basılıp bırakıldığında 3 saniyelik geri sayım başlar.

### 2. Başlangıç Sinyali

Geri sayım tamamlandığında buzzer 300 milisaniye boyunca ses verir.

Ölçüm, buzzer etkinleştirildiği anda başlatılır.

### 3. Atış Algılama

MAX4466 mikrofonundan gelen analog sinyal ESP32 tarafından okunur.

Sinyal genliği ayarlanan eşik değerini geçtiğinde atış algılanır.

Yazılım en fazla 10 atış kaydeder.

### 4. TIME ve SPLIT

**TIME:** Başlangıç sinyalinden son algılanan atışa kadar geçen toplam süredir.

**SPLIT:** Bu yazılım sürümünde başlangıç sinyalinden ilk algılanan atışa kadar geçen süredir. Sonraki atışlarda değişmez.

Süreler ekranda saniye ve milisaniye biçiminde gösterilir.

### 5. Kör Süre

Atış algılamaları arasında 180 ms bekleme uygulanır.

Bu süre, aynı sesin tekrar tekrar algılanmasını azaltmak amacıyla kullanılır.

### 6. Yeniden Başlatma

START butonuna tekrar kısa basılıp bırakıldığında önceki ölçümler sıfırlanır ve yeni geri sayım başlar.

### 7. Deep Sleep

START butonuna 3 saniye basılı tutulduğunda ESP32 Deep Sleep moduna geçer.

Cihaz IDLE durumunda veya 10 atış tamamlandıktan sonra 5 dakika işlem yapılmazsa otomatik olarak Deep Sleep moduna geçer.

ESP32 beslemesi devam ettiği sürece GPIO27 üzerinden uyandırılabilir.

**Not:** FM5324'ün düşük akım nedeniyle 5V çıkışını tamamen kapatması ayrı bir güç yönetimi durumudur. Böyle bir durumda ESP32 yalnızca GPIO27 ile uyanamaz; güç modülünün yeniden etkinleştirilmesi gerekir.

## Mikrofon Hassasiyeti

Yazılımın başlangıç eşik değeri:

\`\`\`cpp
int threshold = 120;
\`\`\`

Arduino IDE Seri Monitör üzerinden ayarlanabilir.

| Komut | İşlev |
|---|---|
| s | Zamanlayıcıyı başlatır |
| p | Mikrofon izlemeyi açar/kapatır |
| + | Eşik değerini 30 azaltır, hassasiyeti artırır |
| - | Eşik değerini 30 artırır, hassasiyeti azaltır |

Seri monitör hızı: **115200 baud**

Hassasiyet değişiklikleri çalışma sırasında geçerlidir. Kodda kalıcı olarak saklanmaz.

## Arduino IDE ile Kurulum

### 1. Arduino IDE'yi kurun

Arduino IDE uygulamasını bilgisayarınıza yükleyin.

### 2. ESP32 kart desteğini kurun

Arduino IDE üzerinden ESP32 kart paketini yükleyin.

Kart olarak **ESP32 Dev Module** seçin.

### 3. Gerekli kütüphaneyi kurun

Arduino IDE > Sketch > Include Library > Manage Libraries bölümünden **U8g2** kütüphanesini aratıp yükleyin.

SPI ve ESP32'ye ait diğer başlık dosyaları, uygun ESP32 Arduino kart paketiyle sağlanır.

### 4. Kaynak kodu indirin

[ShotTimer.ino](firmware/ShotTimer/ShotTimer.ino) dosyasını indirin.

Dosyayı \`ShotTimer\` adlı klasör içinde Arduino IDE ile açın.

### 5. ESP32'ye yükleyin

1. ESP32'yi USB kablosuyla bilgisayara bağlayın.
2. Arduino IDE'de kart olarak ESP32 Dev Module seçin.
3. Doğru COM portunu seçin.
4. Yükle (Upload) butonuna basın.
5. Yükleme tamamlandığında cihazı kontrol edin.

## 3D Baskı Kutusu

Cihazın kutusu proje için özel olarak tasarlanmıştır.

Bütün kutu parçaları tek bir 3MF dosyasında bulunmaktadır.

**[ShotTimer.3mf dosyasını görüntüle](3d-models/ShotTimer.3mf)**

3MF dosyasını Bambu Studio, OrcaSlicer veya uyumlu başka bir dilimleme programında açabilirsiniz.

Baskı öncesinde yazıcınıza uygun dilimleme ayarlarını kontrol edin.

[3D Baskı Açıklamaları](3d-models/README.md)

## Proje Dosyaları

\`\`\`text
ESP32-Shot-Timer/
├── README.md
├── firmware/
│   └── ShotTimer/
│       └── ShotTimer.ino
├── hardware/
│   ├── malzeme-listesi.md
│   └── baglanti-semasi.md
└── 3d-models/
    ├── README.md
    └── ShotTimer.3mf
\`\`\`

## Teknik Notlar

- Zaman ölçümü ESP32'nin mikrosaniye sayacı kullanılarak gerçekleştirilir.
- Ekran üç ondalık basamakla saniye gösterir.
- Gösterim çözünürlüğü 1 ms olsa da bu tek başına 1 ms ölçüm doğruluğunun doğrulandığı anlamına gelmez.
- Mikrofonun algılama performansı ortam gürültüsüne, kazanç ayarına ve eşik değerine bağlıdır.
- Güç devresinin son bağlantıları ve cihazın gerçek çalışma süresi ayrıca doğrulanmalıdır.

## Lisans

Bu proje için henüz açık kaynak lisansı seçilmemiştir.

## Geliştirme

Proje, elektronik ve gömülü yazılım geliştirme çalışması olarak hazırlanmıştır.
  
