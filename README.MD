# 1. Giriş
Bu yazıda temel anlamda debug araçlarından olan gdb'den bahsedilecektir. Yazının sonunda temel anlamda gdb kullanabiliyor duruma geleceksinizdir. Yazı boyunca kullanılan bütün uygulamalar ve kodlar *Dosyalar* klasöründe bulunmaktadır. 
Uygulamalarımızı geliştirme sırasında gerek sistemden olsun gerek yazılımcı tarafından olsun gelen bir takım sinyaller veya kesmeler veya hatalar yüzünden uygulamamızın çalışmasında kesilmeler olabilir. Bu gibi durumları çoğu zaman tahmin edebiliyor olmak yeterli olmayabilir. Bu gibi durumlarda en büyük yardımcımız gdb olacaktır. Linux sistemlerde yaygın olarak GBU debugger isimli uygulama kullanılır. Bu uygulama ile uygulamanızın kodu veya core dosyası incelenebilir.

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
## ÖNEMLİ NOT:
Bu yazıyı yazarken kullandığım makinem 64 bittir. Bundan dolayı kodlar derlenirken 64 bitlik adreslere göre derlenmiştir. Eğer programları direkt olarak binary halleriyle analiz yapmaya çalışırsanız. İleriki bölümlerde göreceğimiz eip, eax, ebx gibi registerler yerine rip, rax, rbx register isimleri göreceksiniz. Bunun önüne geçebilmek için programların kaynak kodlarınıda *Dosyalar* klasörü altında veriyorum. Eğer sizde 64 bit bir bilgisayar kullanıyorsanız, programları derlerken -m32 flagini ayağa kaldırmanız gerekmektedir. Yani yazacağınız komut şu şekilde olacaktır. `gcc -g -m32 kod.c -o cikti` BU şekilde kodunuz x86'ya göre derlenecektir. Hala problem yaşamaktaysanız gdb'nin analiz kısmınıda 32 bite göre ayarlamanız gerekmektedir. Bunun için ise gdb başlatıldığında `set architecture i386:x86-64:intel` komutunu vermeniz yeterli olacaktır.
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

# 4. Adresleri Görüntüleme

Kod içerisinde belirli bölümlere breakpoint koyarak registerlar hakkında bilgi sahibi olmayı gördük. Şimdi adresleri nasıl görüntüleyeceğimizi göstermek istiyorum. bunu `x` komutu ile gerçekleştiriyoruz. gdb içerisindeyken `help x` demek iyi bir başlangıç olabilir.
```sh
(gdb) help x
Examine memory: x/FMT ADDRESS.
ADDRESS is an expression for the memory address to examine.
FMT is a repeat count followed by a format letter and a size letter.
Format letters are o(octal), x(hex), d(decimal), u(unsigned decimal),
  t(binary), f(float), a(address), i(instruction), c(char), s(string)
  and z(hex, zero padded on the left).
Size letters are b(byte), h(halfword), w(word), g(giant, 8 bytes).
The specified number of objects of the specified size are printed
according to the format.  If a negative number is specified, memory is
examined backward from the address.

Defaults for format and size letters are those previously used.
Default count is 1.  Default address is following last thing printed
with this command or "print".
```
*x/FMT ADDRESS* aslında bize yazım kuralını gösteriyor. FMT kısmına seçeneklerden istediğinizi belirterek bir dump işlemi gerçekleştirebilirsiniz.

## 4.1 Assembly dump
Şuanda hala kodumuzun main kısmında takılı kalmış durumdayız. main fonksiyonunun assemly halini görebilmek için `disassemble main` komutunu çalıştırmalıyız.
```sh
(gdb) disassemble main
Dump of assembler code for function main:
   0x565561e2 <+0>:	lea    0x4(%esp),%ecx
   0x565561e6 <+4>:	and    $0xfffffff0,%esp
   0x565561e9 <+7>:	pushl  -0x4(%ecx)
   0x565561ec <+10>:	push   %ebp
   0x565561ed <+11>:	mov    %esp,%ebp
   0x565561ef <+13>:	push   %ecx
   0x565561f0 <+14>:	sub    $0x4,%esp
   0x565561f3 <+17>:	call   0x56556220 <__x86.get_pc_thunk.ax>
   0x565561f8 <+22>:	add    $0x2e08,%eax
   0x565561fd <+27>:	mov    %ecx,%eax
=> 0x565561ff <+29>:	mov    0x4(%eax),%eax
   0x56556202 <+32>:	add    $0x4,%eax
   0x56556205 <+35>:	mov    (%eax),%eax
   0x56556207 <+37>:	sub    $0xc,%esp
   0x5655620a <+40>:	push   %eax
   0x5655620b <+41>:	call   0x565561a9 <cikti>
   0x56556210 <+46>:	add    $0x10,%esp
   0x56556213 <+49>:	mov    $0x0,%eax
   0x56556218 <+54>:	mov    -0x4(%ebp),%ecx
   0x5655621b <+57>:	leave  
   0x5655621c <+58>:	lea    -0x4(%ecx),%esp
   0x5655621f <+61>:	ret    
End of assembler dump.
```

## 4.2 Eşleşme
Görüldüğü üzere main fonksiyonunun içerisindeki breakpoint ile eip register'ının adresleri aynı. Bu zaten beklenen bir sonuçtu...

## 4.3 Dump
Şimdi x komutu ile bir dump gerçekleştirelim.
```sh
(gdb) x/i 0x565561ff
=> 0x565561ff <main+29>:	mov    0x4(%eax),%eax
```
Görüldüğü gibi Assembly dump ile aynı sonucu almış olduk. Farklı boyutlarda da dump işlemi gerçekleştirebiliriz.
```sh
(gdb) x/10i 0x565561ff
=> 0x565561ff <main+29>:	mov    0x4(%eax),%eax
   0x56556202 <main+32>:	add    $0x4,%eax
   0x56556205 <main+35>:	mov    (%eax),%eax
   0x56556207 <main+37>:	sub    $0xc,%esp
   0x5655620a <main+40>:	push   %eax
   0x5655620b <main+41>:	call   0x565561a9 <cikti>
   0x56556210 <main+46>:	add    $0x10,%esp
   0x56556213 <main+49>:	mov    $0x0,%eax
   0x56556218 <main+54>:	mov    -0x4(%ebp),%ecx
   0x5655621b <main+57>:	leave  
```
Registerları nasıl dump edeceğiz diye sorabilirsiniz, aşağıda gösterilmiştir.
```sh
(gdb) x/x $esp
0xffffd210:	0xf7fe4560
```
```sh
(gdb) x/10x $esp
0xffffd210:	0xf7fe4560	0xffffd230	0x00000000	0xf7ddfb41
0xffffd220:	0xf7f9f000	0xf7f9f000	0x00000000	0xf7ddfb41
0xffffd230:	0x00000002	0xffffd2c4
```
```sh
(gdb) x/20x $esp
0xffffd210:	0xf7fe4560	0xffffd230	0x00000000	0xf7ddfb41
0xffffd220:	0xf7f9f000	0xf7f9f000	0x00000000	0xf7ddfb41
0xffffd230:	0x00000002	0xffffd2c4	0xffffd2d0	0xffffd254
0xffffd240:	0x00000001	0x00000000	0xf7f9f000	0xffffffff
0xffffd250:	0xf7ffd000	0x00000000	0xf7f9f000	0xf7f9f000
```
<hr>
## ÖNEMLİ NOT
Mikrodenetleyicilerle hiç uğraştınız mı bilmiyorum ama bir devre tasarlarken kristallere ihtiyaç duyarız bu şekilde frekans hesabı yaparız. Aslında burada asıl belirlenmek istenen saniyede yapılacak işlem sayısıdır. Bu görevi bilgisayarlarımızda CPU'lar üstlenmektedir. Bu kısımda anlatacağım konunun daha net anlaşılabilmesi için senaryolaştıracağım ve yazdığım her şeyi doğruymuş gibi kabul etmeden sadece mesajı almaya çalışın.
C dili ile yazdığımız bir satırlık bir kodun assembly karşılığını gördük. Aslında gördüğümüzden de daha fazlası var örnek vermek gerekirse kullandığımız printf fonksiyonu daha öncesinden bizim için yazılmış bir fonksiyon. Bu ne anlama geliyor peki? O fonksiyonun olduğu kütüphaneyi aslında projemize dahil etmiş olduk demektir. Buraya kadar bir sıkıntı olduğunu düşünmüyorum. Şimdi size bir soru soracağım. breakpoin koyduğumuz yerden adım adım devam etmek istediğimizde printf fonksiyonunun saklandığı adrese geldiğimizde ve devam et dediğimizde gdb printf fonksiyonun kodlarının yazıldığı adreslere bir anda fırlayıp onun içinden mi devam eder yoksa o kod bloğunun komple çalıştırıp alt adresemi geçer? Daha açıklayıcı olmak için aşağıdaki çıktılara bakınız.
```sh
(gdb) disassemble cikti
Dump of assembler code for function cikti:
   0x565561a9 <+0>:	push   %ebp
   0x565561aa <+1>:	mov    %esp,%ebp
   0x565561ac <+3>:	push   %ebx
   0x565561ad <+4>:	sub    $0x24,%esp
   0x565561b0 <+7>:	call   0x565560b0 <__x86.get_pc_thunk.bx>
   0x565561b5 <+12>:	add    $0x2e4b,%ebx
   0x565561bb <+18>:	sub    $0x8,%esp
   0x565561be <+21>:	pushl  0x8(%ebp)
   0x565561c1 <+24>:	lea    -0x1c(%ebp),%eax
   0x565561c4 <+27>:	push   %eax
   0x565561c5 <+28>:	call   0x56556030 <strcpy@plt>
   0x565561ca <+33>:	add    $0x10,%esp
   0x565561cd <+36>:	sub    $0xc,%esp
   0x565561d0 <+39>:	lea    -0x1c(%ebp),%eax
   0x565561d3 <+42>:	push   %eax
   0x565561d4 <+43>:	call   0x56556040 <puts@plt>
   0x565561d9 <+48>:	add    $0x10,%esp
   0x565561dc <+51>:	nop
   0x565561dd <+52>:	mov    -0x4(%ebp),%ebx
   0x565561e0 <+55>:	leave  
   0x565561e1 <+56>:	ret    
End of assembler dump.
```

Görüldüğü üzere 0x565561d4 adresinde bir fonksiyon çağrılıyor, printf fonksiyonumuz. Şimdi sorunun cevabını verelim. Bu aslında bizim elimizde...
`step` kod kod ilerler,
`stepi` adres adres ilerler,
`continue` başka bir breakpoint görene kadar devam eder.

```sh
(gdb) disassemble main
Dump of assembler code for function main:
   0x565561e2 <+0>:	lea    0x4(%esp),%ecx
   0x565561e6 <+4>:	and    $0xfffffff0,%esp
   0x565561e9 <+7>:	pushl  -0x4(%ecx)
   0x565561ec <+10>:	push   %ebp
   0x565561ed <+11>:	mov    %esp,%ebp
   0x565561ef <+13>:	push   %ecx
   0x565561f0 <+14>:	sub    $0x4,%esp
   0x565561f3 <+17>:	call   0x56556220 <__x86.get_pc_thunk.ax>
   0x565561f8 <+22>:	add    $0x2e08,%eax
   0x565561fd <+27>:	mov    %ecx,%eax
=> 0x565561ff <+29>:	mov    0x4(%eax),%eax
   0x56556202 <+32>:	add    $0x4,%eax
   0x56556205 <+35>:	mov    (%eax),%eax
   0x56556207 <+37>:	sub    $0xc,%esp
   0x5655620a <+40>:	push   %eax
   0x5655620b <+41>:	call   0x565561a9 <cikti>
   0x56556210 <+46>:	add    $0x10,%esp
   0x56556213 <+49>:	mov    $0x0,%eax
   0x56556218 <+54>:	mov    -0x4(%ebp),%ecx
   0x5655621b <+57>:	leave  
   0x5655621c <+58>:	lea    -0x4(%ecx),%esp
   0x5655621f <+61>:	ret    
End of assembler dump.
```
```sh
(gdb) stepi
0x56556202	12		cikti(argv[1]);
```
```sh
(gdb) disassemble main 
Dump of assembler code for function main:
   0x565561e2 <+0>:	lea    0x4(%esp),%ecx
   0x565561e6 <+4>:	and    $0xfffffff0,%esp
   0x565561e9 <+7>:	pushl  -0x4(%ecx)
   0x565561ec <+10>:	push   %ebp
   0x565561ed <+11>:	mov    %esp,%ebp
   0x565561ef <+13>:	push   %ecx
   0x565561f0 <+14>:	sub    $0x4,%esp
   0x565561f3 <+17>:	call   0x56556220 <__x86.get_pc_thunk.ax>
   0x565561f8 <+22>:	add    $0x2e08,%eax
   0x565561fd <+27>:	mov    %ecx,%eax
   0x565561ff <+29>:	mov    0x4(%eax),%eax
=> 0x56556202 <+32>:	add    $0x4,%eax
   0x56556205 <+35>:	mov    (%eax),%eax
   0x56556207 <+37>:	sub    $0xc,%esp
   0x5655620a <+40>:	push   %eax
   0x5655620b <+41>:	call   0x565561a9 <cikti>
   0x56556210 <+46>:	add    $0x10,%esp
   0x56556213 <+49>:	mov    $0x0,%eax
   0x56556218 <+54>:	mov    -0x4(%ebp),%ecx
   0x5655621b <+57>:	leave  
   0x5655621c <+58>:	lea    -0x4(%ecx),%esp
   0x5655621f <+61>:	ret    
End of assembler dump.
```
Yukarıda görüldüğü gibi. Diğer komutları size bırakıyorum...
<hr>

# 5. Register Üzerine Yazma
Aşağıdaki gibi bir kodumuzun olduğunu düşünelim ve bu kodu derleyelim.

```sh
root@kali:~/Documents/GDB Serisi/Dosyalar# cat simple3.c 
#include<stdio.h>

void beniCalistirazsin()
{
	printf("Crack!");
	exit(0);
}

main()
{
	printf("Hello to the World of GNU Debugger!\n");
	return 0;
}
```
Normal şartlar altında beniCalistirazsin() fonksiyonu çalışmamalıdır. Ama eip üzerine yazabilirsek sonra çalıştırılacak olan adresi değiştirmiş oluruz. Programı derledikten sonra (`gcc -g -m32 simple3.c -o simple3`) gdb ile açalım ve set komutunu nasıl kullandığımızı görelim. Aşağıdaki adımları takip ettiğinizde ne yaptığımı anlamış olacaksınızdır. Ekstra bir açıklama yapmayacağım. Eğer anlamadığınızı düşünüyorsanız 3. konudan itibaren tekrar ediniz.

Önce programı normal başlatım çalıştıralım ve sonlandıralım.
```sh
root@kali:~/Documents/GDB Serisi/Dosyalar# gdb simple3
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
Reading symbols from simple3...done.
```
```sh
(gdb) break main 
Breakpoint 1 at 0x1200: file simple3.c, line 11.
```
```sh
(gdb) run 
Starting program: /root/Documents/GDB Serisi/Dosyalar/simple3 

Breakpoint 1, main () at simple3.c:11
11		printf("Hello to the World of GNU Debugger!\n");
```
```sh
(gdb) info registers 
eax            0x56559000          1448448000
ecx            0xffffd240          -11712
edx            0xffffd264          -11676
ebx            0x0                 0
esp            0xffffd220          0xffffd220
ebp            0xffffd228          0xffffd228
esi            0xf7f9f000          -134615040
edi            0xf7f9f000          -134615040
eip            0x56556200          0x56556200 <main+25>
eflags         0x216               [ PF AF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 0
gs             0x63                99
```
```sh
(gdb) continue 
Continuing.
Hello to the World of GNU Debugger!
[Inferior 1 (process 6691) exited normally]
```

Şimdi `eip` üzerine yazmayı deneyelim.
```sh
root@kali:~/Documents/GDB Serisi/Dosyalar# gdb simple3
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
Reading symbols from simple3...done.
```
```sh
(gdb) break main 
Breakpoint 1 at 0x1200: file simple3.c, line 11.
```
```sh
(gdb) run
Starting program: /root/Documents/GDB Serisi/Dosyalar/simple3 

Breakpoint 1, main () at simple3.c:11
11		printf("Hello to the World of GNU Debugger!\n");
```
```sh
(gdb) info registers 
eax            0x56559000          1448448000
ecx            0xffffd240          -11712
edx            0xffffd264          -11676
ebx            0x0                 0
esp            0xffffd220          0xffffd220
ebp            0xffffd228          0xffffd228
esi            0xf7f9f000          -134615040
edi            0xf7f9f000          -134615040
eip            0x56556200          0x56556200 <main+25>
eflags         0x216               [ PF AF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 0
gs             0x63                99
```
```sh
(gdb) info functions 
All defined functions:

File simple3.c:
3:	void beniCalistirazsin();
9:	int main();

Non-debugging symbols:
0x56556000  _init
0x56556030  printf@plt
0x56556040  puts@plt
0x56556050  exit@plt
0x56556060  __libc_start_main@plt
0x56556070  __cxa_finalize@plt
0x56556080  _start
0x565560c0  __x86.get_pc_thunk.bx
0x565560d0  deregister_tm_clones
0x56556110  register_tm_clones
0x56556160  __do_global_dtors_aux
0x565561b0  frame_dummy
0x565561b5  __x86.get_pc_thunk.dx
0x56556223  __x86.get_pc_thunk.ax
0x56556230  __libc_csu_init
0x56556290  __libc_csu_fini
0x56556294  _fini
0xf7fd5010  _dl_catch_exception@plt
0xf7fd5020  malloc@plt
0xf7fd5030  _dl_signal_exception@plt
0xf7fd5040  calloc@plt
0xf7fd5050  realloc@plt
0xf7fd5060  _dl_signal_error@plt
0xf7fd5070  _dl_catch_error@plt
0xf7fd5080  free@plt
0xf7fdd8d0  _dl_rtld_di_serinfo
0xf7fe4950  _dl_debug_state
0xf7fe61a0  _dl_mcount
0xf7fe6a70  _dl_get_tls_static_info
0xf7fe6b60  _dl_allocate_tls_init
0xf7fe6dc0  _dl_allocate_tls
0xf7fe6e00  _dl_deallocate_tls
0xf7fe70e0  ___tls_get_addr
0xf7fe7130  __tls_get_addr
0xf7fe7510  _dl_make_stack_executable
0xf7fe77d0  _dl_find_dso_for_object
0xf7fe9850  _dl_exception_create
0xf7fe9950  _dl_exception_create_format
0xf7fe9ba0  _dl_exception_free
0xf7feacb0  __tunable_get_val
0xf7feb5a0  __get_cpu_features
0xf7fed180  malloc
0xf7fed280  calloc
--Type <RET> for more, q to quit, c to continue without paging--print beniCalistirazsin
0xf7fed2d0  free
0xf7fed4e0  realloc
0xf7fed800  _dl_signal_exception
0xf7fed860  _dl_signal_error
0xf7fed9e0  _dl_catch_exception
0xf7fedaa0  _dl_catch_error
0xf7fd2850  __vdso_clock_gettime
0xf7fd2d80  __vdso_gettimeofday
0xf7fd3030  __vdso_time
0xf7fd3070  __kernel_vsyscall
0xf7fd3090  __kernel_sigreturn
0xf7fd30a0  __kernel_rt_sigreturn
0xf7dde010  _Unwind_Find_FDE@plt
0xf7dde020  realloc@plt
0xf7dde030  *ABS*@plt
0xf7dde040  memalign@plt
0xf7dde050  _dl_exception_create@plt
0xf7dde060  *ABS*@plt
0xf7dde070  __tunable_get_val@plt
0xf7dde080  *ABS*@plt
0xf7dde090  _dl_find_dso_for_object@plt
0xf7dde0a0  calloc@plt
0xf7dde0b0  ___tls_get_addr@plt
0xf7dde0c0  __libpthread_freeres@plt
0xf7dde0c8  malloc@plt
0xf7dde0d0  __libdl_freeres@plt
0xf7dde0d8  free@plt
0xf7dde1c6  abort
0xf7ddf8c0  __libc_init_first
0xf7ddfa50  __libc_start_main
0xf7ddfc80  gnu_get_libc_release
0xf7ddfca0  gnu_get_libc_version
0xf7de0060  __divdi3
0xf7de0100  __moddi3
0xf7de0190  __udivdi3
0xf7de01c0  __umoddi3
0xf7de01f0  __errno_location
0xf7de02f0  iconv_open
0xf7de0560  iconv
0xf7de0750  iconv_close
0xf7de0f30  __gconv_get_modules_db
0xf7de0f50  __gconv_get_alias_db
0xf7de80d0  __gconv_transliterate
0xf7de8700  __gconv_get_cache
0xf7de9360  setlocale
0xf7deb380  localeconv
0xf7deb530  nl_langinfo
0xf7deb560  __nl_langinfo_l
0xf7deb560  nl_langinfo_l
0xf7deb5c0  __ctype_get_mb_cur_max
--Type <RET> for more, q to quit, c to continue without paging--q
Quit
```
```sh
(gdb) print beniCalistirazsin
$1 = {void ()} 0x565561b9 <beniCalistirazsin>
```
```sh
(gdb) set $eip = 0x565561b9
```
```sh
(gdb) info registers 
eax            0x56559000          1448448000
ecx            0xffffd240          -11712
edx            0xffffd264          -11676
ebx            0x0                 0
esp            0xffffd220          0xffffd220
ebp            0xffffd228          0xffffd228
esi            0xf7f9f000          -134615040
edi            0xf7f9f000          -134615040
eip            0x565561b9          0x565561b9 <beniCalistirazsin>
eflags         0x216               [ PF AF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 0
gs             0x63                99
```
```sh
(gdb) continue 
Continuing.
Crack![Inferior 1 (process 6703) exited normally]
```
Görüldüğü üzere ekrana Crack! yazısı gelmiştir.
<hr>
NOT: disassembly komutu uygulanırken intel ailesine uygun syntax'ta assembly kodları göremeyebilirsiniz. Bunu düzeltmek için `set disassembly-flover intel` komutunu uygulamanız gerekmektedir.
<hr>

# 6. SON
Artık internetteki gdb ile alakalı araştırmalarınızda okuduklarınızı anlayacak genel anlamda anlayacak kadar bilgiye sahipsiniz. Temel anlamda gdb kullanabiliyorsunuz. Umarım yararlı olmuştur. *Beren Kuday GÖRÜN*

