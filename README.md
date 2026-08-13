# Resident Evil 2 - Open Source Infinite Ammo (C++) 🧟‍♂️🔫

Bu proje, Resident Evil 2 (RE Engine) için tamamen sıfırdan C++ ve Windows API kullanılarak geliştirilmiş bir bellek yamalama (Memory Patching) ve DLL Enjeksiyon (DLL Injection) aracıdır.

**🔒 Güvenlik ve Şeffaflık Notu:** 
Güvenlik prensipleri gereği, bu repoda bilerek **hiçbir derlenmiş `.exe` veya `.dll` dosyası paylaşılmamıştır.** Çalıştırılabilir dosyalar (binary) kötü amaçlı yazılım şüphesi doğurabileceği için, bu projenin kaynak kodlarını kendi bilgisayarınızda derleyerek şeffaf bir şekilde kullanmanız hedeflenmiştir.

## 🚀 Özellikler
*   **AOB Pattern Scanning:** Oyunun güncellenmesi durumunda bellek adresleri değişse bile, dinamik bayt taraması sayesinde mermi adresini otonom olarak bulur.
*   **Custom Injector:** Standart `LoadLibraryA` yöntemini kullanarak hileyi hedef sürece (`re2.exe`) dışarıdan entegre eder.
*   **Tamamen Açık Kaynak:** Arka planda çalışan hiçbir gizli işlem yoktur, kodu satır satır inceleyebilirsiniz. Kendi DLL'inizi ve exe dosyanızı kendiniz oluşturacağınızdan içiniz rahat olabilir

## 🛠️ Gereksinimler
Kendi hilenizi derleyebilmek için bilgisayarınızda bir C++ derleyicisi (Compiler) kurulu olmalıdır. 
*   **MinGW-w64** (Kesinlikle 64-bit destekli bir sürüm olmalıdır, çünkü RE2 64-bit bir oyundur).
*   Derleyicinizin Windows `PATH` ortam değişkenlerine eklenmiş olduğundan emin olun (Terminalde `g++ -v` yazarak test edebilirsiniz).

## ⚙️ Nasıl Derlenir ve Kullanılır?

Kodu bilgisayarınıza indirdikten sonra, proje klasörünün içinde bir terminal (CMD veya PowerShell) açın ve sırasıyla şu adımları izleyin:

### 1. Adım: Hile Kütüphanesini (DLL) Derleme
Aşağıdaki komutu çalıştırarak `dllmain.cpp` dosyasından kendi paylaşımlı kütüphanenizi oluşturun:
```bash
g++ -shared -o RE2_InfiniteAmmo.dll dllmain.cpp -Wl,--out-implib,libtstdll.a
```
### 2. Adım: Enjektörü EXE olarak derleme
Aşağıdaki komutu çalıştırarak `Enjektor.exe` dosyanızı oluşturun:
```bash
g++ injector.cpp -o Enjektor.exe -static-libgcc -static-libstdc++ -static
```
### 3. Adım: Enjektörü çalıştırma
* Resident Evil 2 oyununu başlatın ve ana hikaye kaydınıza girin. (Oyun motoru kilitlerini aşmak için envanterinizde en az 1 mermi bulunmalıdır).
* Oluşturduğunuz `Enjektor.exe` dosyasına sağ tıklayıp Yönetici Olarak Çalıştırın (Windows API'nin belleğe yazma izni alabilmesi için bu şarttır).
* Konsol ekranında "Başarılı!" mesajını gördükten sonra oyuna dönün. Merminiz bir daha asla azalmayacaktır!
