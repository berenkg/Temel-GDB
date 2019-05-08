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

```console
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
