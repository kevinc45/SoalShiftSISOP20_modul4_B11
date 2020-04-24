# SoalShiftSISOP20_modul4_B11
## Kelompok
 - Achmad Sofyan Pratama (05111840000061)
 - Kevin Christian Hadinata (05111840000066)

# ENCV_1
Dengan menggunakan enkripsi berbasis [*Caesar Cipher*](https://en.wikipedia.org/wiki/Caesar_cipher) dengan rangkaian alphabet yang disediakan. Karena kita akan mengenkripsi nama *file* dan *folder* dengan perbedaan jarak 10 dari alfabet asalnya, maka kita perlu menambah 10 alfabet tambahan (yang merupakan 10 alfabet awal) di bagian belakang *dictionary* yang akan kita buat.

```c
char alphabet[100] = {"9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[Lm"};
```

Karena kita ingin menghindari ekstensi dari file-file di dalam folder ENCV1 ikut terenkripsi, maka kita perlu mengambil dulu ekstensi *file* yang kita enkripsi (dengan menggunakan `strchr`, lalu kita tambahkan lagi pada akhir *file* enkripsi.



# References:
- [https://en.wikipedia.org/wiki/Caesar_cipher](https://en.wikipedia.org/wiki/Caesar_cipher)
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE4NzE2MzEwNjEsMjExNjEwNzEyNSwyNj
U3NTU0MF19
-->