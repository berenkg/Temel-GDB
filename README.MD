# 1. Giriş
Bu seride temel anlamda debug araçlarından olan gdb'den bahsedilecektir. Uygulamalarımızı geliştirme sırasında gerek sistemden olsun gerek yazılımcı tarafından olsun gelen bir takım sinyaller veya kesmeler veya hatalar yüzünden uygulamamızın çalışmasında kesilmeler olabilir. Bu gibi durumları çoğu zaman tahmin edebiliyor olmak yeterli olmayabilir. Bu gibi durumlarda en büyük yardımcımız gdb olacaktır. Linux sistemlerde yaygın olarak GBU debugger isimli uygulama kullanılır. Bu uygulama ile uygulamanızın kodu veya core dosyası incelenebilir.

# 2. Derleme

```sh
root@kali:~/Documents/GDB Serisi/1 Genel Bakis# cat simple.c 
#include<stdio.h>
main()
{
	printf("Hello to the World of GNU Debugger!\n");
	return 0;
}
```

```sh
root@kali:~/Documents/GDB Serisi/1 Genel Bakis# gcc -g simple.c -o simple_debug_symbol_var
```
Burada gcc ile derleme esnasında -g parametresini kullanmamızın sebebi **Symbol Dosyalarını** dahil etmek istediğizi belirtmek içindir.

## 2.1 Symbol Dosyaları
Symbol dosyaları yazılmış olan kod ile birlikte derlenebilirken `gcc -g simple.c -o simple_debug_symbol_var` ayrı olarak da derlenebilir. Asıl amacı debug aşamasındayken yazılımcıya daha iyi bilgiler sunmasıdır. Örnek olarak derlenmiş bir kodun binary halinden tekrar kod haline ulaşmak normalde zor bir süreçtir ancak symbol dosyaları ile birlikte derlenirse, bu kodlara ulaşılabilir. Aşağıda buna bir örnek verilmiştir.
gdb ile programımızı başlatım list komutu ile kodlarımızı görüntülemeye çalışcağız.

```sh
root@kali:~/Documents/GDB Serisi/1 Genel Bakis# gdb simple_debug_symbol_var 
GNU gdb (Debian 8.2-1) 8.2
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from simple_debug_symbol_var...done.
(gdb) list 1
1	#include<stdio.h>
2	main()
3	{
4		printf("Hello to the World of GNU Debugger!\n");
5		return 0;
6	}
(gdb) 
```

Yukarıda görüldüğü üzere kodlar başarıyla ekrana getirilmiştir.
Eğer bunu derleme aşamasında `-g` parametresi vermeden bir binary dosya oluşturarak deneseydik hata alacaktık. 

# 3. Başlangıç
Bu bölümde temel gdb komutlarına değinilecektir. Önceden temel programlama bilgisi yanında Assembly bilginizin olması size avantaj sağlayacaktır. İlk olarak aşağıdaki kodu derleyip gdb ile çalıştırınız.

```c
#include<stdio.h>
#include<string.h>
void cikti(char *userInput)
{
    char buffer[20];
    strcpy(buffer, userInput);
    printf("%s\n", buffer);
}

int main(int argc, char **argv)
{
	cikti(argv[1]);
	return 0;
}
```
Yukarıdaki kodun çalıştırılmadan önce bir argüman alacağı açıktır. gdb ile çalıştırılma esnasında `run <argüman>` şeklinde ilerlemelisiniz. Ama bundan önce programımızı belirli bir çalışma alanında durdurup register kontrolü yapalım. Bunu `info registers` komutu ile gerçekleştireceğiz tabi ki programı belirli bir alanda durdurabilmek içinde `breakpoint` komutunu kullanmamız gerekmektedir. Şimdi buradaka aşamaları sırasıyla yapalım.

```sh
root@kali:~/Documents/GDB Serisi/1 Genel Bakis# gdb simple2
GNU gdb (Debian 8.2-1) 8.2
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from simple2...done.
```
Programımızı gdb ile başlattık. symbols yüklemesi yapıldığı için list komutu ile kodlarımıza bakabiliriz.
```sh
(gdb) list
1	#include<stdio.h>
2	#include<string.h>
3	void cikti(char *userInput)
4	{
5	    char buffer[20];
6	    strcpy(buffer, userInput);
7	    printf("%s\n", buffer);
8	}
9	
10	int main(int argc, char **argv)
(gdb) 
11	{
12		cikti(argv[1]);
13		return 0;
14	}
(gdb) 
Line number 15 out of range; simple2.c has 14 lines.
```
Çok güzel. Şimdi program çalıştırılmaya başladığında main kısmında durmasını sağlayacak breakpoint'i koyalım.

```sh
(gdb) break main
Breakpoint 1 at 0x1182: file simple2.c, line 12.
```
Şimdi programımızı başlatabiliriz.
```sh
(gdb) run KUDAY
Starting program: /root/Documents/GDB Serisi/Dosyalar/simple2 KUDAY
BFD: /usr/lib/debug/.build-id/75/5312dcb2382eb2fde78494879bb2104028ae80.debug: unable to initialize decompress status for section .debug_aranges
BFD: /usr/lib/debug/.build-id/75/5312dcb2382eb2fde78494879bb2104028ae80.debug: unable to initialize decompress status for section .debug_aranges
warning: File "/usr/lib/debug/.build-id/75/5312dcb2382eb2fde78494879bb2104028ae80.debug" has no build-id, file skipped
BFD: /usr/lib/debug/.build-id/55/e7de84df6813c82c5b12822219c17d19c9c4b0.debug: unable to initialize decompress status for section .debug_aranges
BFD: /usr/lib/debug/.build-id/55/e7de84df6813c82c5b12822219c17d19c9c4b0.debug: unable to initialize decompress status for section .debug_aranges
warning: File "/usr/lib/debug/.build-id/55/e7de84df6813c82c5b12822219c17d19c9c4b0.debug" has no build-id, file skipped

Breakpoint 1, main (argc=2, argv=0x7fffffffe168) at simple2.c:12
12		cikti(argv[1]);
```
Programımız herhangi bir çıktı vermedi bunun sebebi main kısmında şuanda beklemektedir. gdb çıktılarından program içerisinde 2 tane argüman olduğu söyleniyor. Bunlardan bir tanesi programın yolu diğer ise KUDAY'dır. Şimdi registerlarımıza bakalım.

<hr>
**Önemli NOT:**Bu yazıyı yazarken kullandığım makinem 64 bittir. Bundan dolayı kodlar derlenirken 64 bitlik adreslere göre derlenmiştir. Eğer programları direkt olarak binary halleriyle analiz yapmaya çalışırsanız. İleriki bölümlerde göreceğimiz eip, eax, ebx gibi registerler yerine rip, rax, rbx register isimleri göreceksiniz. Bunun önüne geçebilmek için programların kaynak kodlarınıda *Dosyalar* klasörü altında veriyorum. Eğer sizde 64 bit bir bilgisayar kullanıyorsanız, programları derlerken -m32 flagini ayağa kaldırmanız gerekmektedir. Yani yazacağınız komut şu şekilde olacaktır. `gcc -g -m32 kod.c -o cikti` BU şekilde kodunuz x86'ya göre derlenecektir. Hala problem yaşamaktaysanız gdb'nin analiz kısmınıda 32 bite göre ayarlamanız gerekmektedir. Bunun için ise gdb başlatıldığında `set architecture i386:x86-64:intel` komutunu vermeniz yeterli olacaktır.
<hr>

Eğer yukarıda yazdıklarımı başarılı bir şekilde gerçekleştirebildiyseniz `info registers' komutundan sonra aşağıdaki gibi bir çıktı alacaksınızdır.
```sh
(gdb) info registers
eax            0xffffd230          -11728
ecx            0xffffd230          -11728
edx            0xffffd254          -11692
ebx            0x0                 0
esp            0xffffd210          0xffffd210
ebp            0xffffd218          0xffffd218
esi            0xf7f9f000          -134615040
edi            0xf7f9f000          -134615040
eip            0x565561ff          0x565561ff <main+29>
eflags         0x216               [ PF AF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 
```