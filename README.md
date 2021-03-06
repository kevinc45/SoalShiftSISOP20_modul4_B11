# SoalShiftSISOP20_modul4_B11
## Kelompok
 - Achmad Sofyan Pratama (05111840000061)
 - Kevin Christian Hadinata (05111840000066)

# ENCV1
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
	.mknod = xmp_mknod,
	.open = xmp_open,
	.rename = xmp_rename
};
```

Isi dari tiap ***xmp*** dapat dilihat di *repository* [ini](https://github.com/asayler/CU-CS3753-PA5/blob/master/fusexmp.c). Untuk mengenkripsi (dan mendekripsi) nama *file*, kita akan melakukan modifikasi pada fungsi-fungsi tersebut sehingga program *file system* dapat mengenkripsi saat mengubah nama folder dengan format "encv1_" di depan dan men-*decrypt* saat perlu melakukan akses pengolahan *file* yang lain.

**Encrypting:**
```c
void caesarcipher(char *filename){
	int count;
	int start=0;
	int length=strlen(filename);
	int alphabetidx;
	char *p;
	for(count=length;count>=0;count--){
		if(filename[count]=='/') break;
		if(filename[count]=='.') length=count-1;
	}
	for(count=1;count<length;count++){
		if(filename[count]=='/') start = count;
	}
	for(count=start;count<length;count++){
		if(filename[count]=='/')continue;
		p=strchr(alphabet,filename[count]);
		if(p){
			alphabetidx=p-alphabet;
			filename[count]=alphabet[(alphabetidx+10)%strlen(alphabet)];
		}
	}
  // printf("Encrypt? %s\n",filename);
}
```

**Decrypting:**
```c
void caesarDecrypt(char *filename){

	int length = strlen(filename);
	int start = 0;
	int count,alphabetidx;
	char *p;
	
	for (count = 1;count < length; count++){
		if(filename[count] == '/' || filename[count+1] == '\0'){
			start = count + 1;
			break;
		}
	}

	for(count = strlen(filename); count >= 0; count--) {
		if(filename[count] == '/') break;
		else if(filename[count] == '.'){
			if(count!=(strlen(filename)-1)){
				length=count-1;
				break;
			}
			else {
				length=strlen(filename);
				break;
			}
		}
	}

	for(count = start;count < length;count++) {
		if(filename[count] == '/') continue;
		p = strchr(alphabet, filename[count]);
		if(p){
			alphabetidx = p-alphabet-10;
			if (alphabetidx < 0) alphabetidx = alphabetidx + strlen(alphabet);
			filename[count] = alphabet[alphabetidx];
		}
	}
}
```

# ENCV2

Belum dikerjakan karena tidak sempat.

# Automatic Directory Synchronization

Belum dikerjakan karena tidak sempat.

# Log System
Penulisan *log* dilakukan menggunakan fungsi dibawah ini untuk setiap proses yang dilakukan oleh *user*:
```c
void logwriter(int info, char* cmd, char* fileawal, char* temp){
	char x[1000];
	char infox[1000];
	char temp2[1000];

	FILE* fp;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(infox, info?"WARNING":"INFO");
	if(strcmp(temp,"")) sprintf(temp2, "::%s", temp);

	fp = fopen("/home/kevinc45/fs.log","a");
	sprintf(x, "%s::%d%d%d-%d:%d:%d::%s::%s%s\n", infox, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cmd, fileawal, temp2);

	if(fp==NULL){
		printf("File gagal dibuat\n");
		exit(EXIT_FAILURE);
	}
	fputs(x,fp);
	fclose(fp);
}
```

Melalui
```c
if(strcmp(temp,"")) sprintf(temp2, "::%s", temp);
```
log yang ditulis dibedakan antara ```xmp_rename``` dengan yang lain, dimana fungsi *rename* akan memiliki 4 parameter (normalnya hanya 3 parameter). 

Setiap fungsi ```xmp``` yang dibutuhkan untuk dicatat memanggil fungsi
```c
void logwriter()
 ``` 
 di akhir proses kerjanya sehingga setiap kali *user* melakukan sesuatu pada *file system*, proses tersebut tercatat pada ***fs.log*** yang terdapat di ``/home/user/``.

# References:
- [https://en.wikipedia.org/wiki/Caesar_cipher](https://en.wikipedia.org/wiki/Caesar_cipher)
-  [https://github.com/Armunz/sisop-modul-4](https://github.com/Armunz/sisop-modul-4)
- [https://github.com/asayler/CU-CS3753-PA5](https://github.com/asayler/CU-CS3753-PA5)
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTc4MzQ4MjA1MSw4NDQwNjE0NzcsMTc1Mj
A3MzYwMiwzNDU0MjIxMTgsMTQ2OTExODAwMSwtMTg3MTYzMTA2
MSwyMTE2MTA3MTI1LDI2NTc1NTQwXX0=
-->