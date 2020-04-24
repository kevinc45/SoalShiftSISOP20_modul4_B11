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

Dengan menggunakan *fuse*, maka kita dapat melakukan hal-hal yang bisa dilakukan pada *file system* biasa dengan fungsi khusus yang kita inginkan. Dalam permasalahan ini, kita menggunakan beberapa fungsi *fuse*, antara lain:
```c
static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.write = xmp_write,
	.truncate = xmp_truncate,
	.mkdir = xmp_mkdir,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.mknod = xmp_mknod
};
```

Isi dari masing-masing ***xmp*** dapat dilihat di *repository* [ini](https://github.com/asayler/CU-CS3753-PA5/blob/master/fusexmp.c).



# References:
- [https://en.wikipedia.org/wiki/Caesar_cipher](https://en.wikipedia.org/wiki/Caesar_cipher)
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU2MDE4NTI2NywzNDU0MjIxMTgsMTQ2OT
ExODAwMSwtMTg3MTYzMTA2MSwyMTE2MTA3MTI1LDI2NTc1NTQw
XX0=
-->