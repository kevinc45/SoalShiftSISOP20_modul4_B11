#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

#define MAX 256
char filelama[MAX][MAX];

    //DIR *dird = opendir("/home/kevinc45/fs.log");
static  const  char *dirpath = "/home/kevinc45/Documents";
char alphabet[100] = {"9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[Lm"};

const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if(!dot || dot == filename) return "";
	return dot;
}

char *strremove(char *str, const char *sub) {
	char *p, *q, *r;
	if ((q = r = strstr(str, sub)) != NULL) {
		size_t len = strlen(sub);
		while ((r = strstr(p = r + len, sub)) != NULL) {
			while (p < r)
				*q++ = *p++;
		}
		while ((*q++ = *p++) != '\0')
			continue;
	}
	return str;
}

void caesarcipher(char* dest, const char* src, char* pathsementara){
	struct dirent *de;
	strcpy(dest, "");
  //char* new = (char*)src;
  //int slash = 0;
	if (src[0] == '/') 
	{
		src++;
    //slash = 1;
	}

	const char *ekstensifile = get_filename_ext(src);
	char ekstensi[100];
	strcpy(ekstensi,ekstensifile);
	int panjangekstensi = strlen(ekstensifile);
    //int t;
    //t = strlen(src);
    //const char s[2] = "/";
	char *token;
    //int first = 1;
    //printf("%s\n",pathsementara);

	char pathsementaranew[1000];
	strcpy(pathsementaranew,strremove(pathsementara,dirpath));

	token = strtok(pathsementaranew, "/");
	if(strncmp(pathsementaranew,"encv1_",6) == 0){
		sprintf(dest,"%s/",token);
		token=strtok(NULL,"/");
		while(token!=NULL){
			int index_alphabet=0;
			int panjang=strlen(token);
			int i=0;
			while(i<panjang){
				if(token[i]==alphabet[index_alphabet]){
					token[i]=alphabet[index_alphabet+10];
					i++;
					index_alphabet=0;
				}
				else index_alphabet++;
			}
			strcat(NULL,token);
			token=strtok(dest,"/");
			if(token!=NULL) strcat(dest,"/");
		}
	}
	DIR *dir = opendir("/home/kevinc45/Documents/");
	while((de = readdir(dir))){
		if (de->d_type == 4){
			int panjangjawaban = strlen(dest);
			for(int p=panjangjawaban-panjangekstensi;p<=panjangjawaban;p++){
				dest[p]='\0';
			}
			strcat(dest,ekstensi);
		}
	}
        //printf("dest:%s\n",dest);
}

// void decrypt(char* dest, char* src){
//     struct dirent *de;
//     strcpy(dest, "");
//     //char* new = src;
//     //int slash = 0;
//     if (src[0] == '/') {
//     src++;
//     //slash = 1;
//     }

//     const char *ekstensifile = get_filename_ext(src);
//     char ekstensi[100];
//     strcpy(ekstensi,ekstensifile);
//     int panjangekstensi = strlen(ekstensifile);
//     //int t;
//     //t = strlen(src);
//     const char s[2] = "/";
//     char *token;
//     token=strtok(src, s);
//     while(token!=NULL){
//         //int index_alphabet=0;
//         int panjang=strlen(token);
//         int k=panjang;
//         int c=panjang;
//         int i=0;
//         while(i<panjang){
//             if(token[i]==alphabet[k]){
//                 token[i]=alphabet[k-10];
//                 i++;
//                 k=c;
//                 }
//                 else k--;
//                 }
//             strcat(dest,token);
//             token=strtok(NULL,s);
//             if(token!=NULL) strcat(dest,"/");
//             }
//         DIR *dir = opendir("/home/kevinc45/Documents/");
//         while((de = readdir(dir))){
//         if (de->d_type == 4){
//             int panjangjawaban = strlen(dest);
//         for(int p=panjangjawaban-panjangekstensi;p<=panjangjawaban;p++){
//             dest[p]='\0';
//         }
//         strcat(dest,ekstensi);
//         }
//         }
// }

static  int  xmp_getattr(const char *path, struct stat *stbuf){

	int res;
	char fpath[1000];
	char pathsementara[1000];
	sprintf(pathsementara,"%s%s",dirpath,path);
    //printf("%s\n",pathsementara);
	char temp[1000];
	if(strncmp(path,"encv1_",6)==0){

		caesarcipher(temp, path, pathsementara);
		sprintf(fpath,"%s/%s",dirpath,temp);
	}
	else{
		sprintf(fpath, "%s%s",dirpath,path);
	} 

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;

}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];
	char pathsementara[1000];
	sprintf(pathsementara,"%s%s",dirpath,path);
	if(strcmp(fpath,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		char temp[1000];
		if(strncmp(path,"encv1_",6)==0){
			caesarcipher(temp, path,pathsementara);
		}
		else strcpy(temp,path);
		sprintf(fpath, "%s/%s",dirpath,temp);
	}

	int res = 0;
	int fd = 0 ;

	(void) fi;

	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno; 

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	return res;

}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
	off_t offset, struct fuse_file_info *fi){
	char fpath[1000];
	char pathsementara[1000];
	sprintf(pathsementara,"%s%s",dirpath,path);
	if((strcmp(fpath,"/") == 0))
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		char temp[1000];
		char tempa[1000];
		strcpy(tempa,path);
		if(strncmp(path,"encv1_",6)==0){
			caesarcipher(temp, path,pathsementara);
			sprintf(fpath, "%s/%s",dirpath,temp);
		}
		else {
            //printf("%s\n",path);
			sprintf(fpath, "%s%s",dirpath,path);
		}
	}
    //printf("%s\n",fpath);

	int res = 0;  

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi; 

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		char temp[1000];

		if(strncmp(path,"/encv1_",7)==0){
        //printf("Cipher running...\n");
			caesarcipher(temp, de->d_name, pathsementara);
		}
		else {
			strcpy(temp,de->d_name);
		}

		res = (filler(buf, temp, &st, 0));
		if(res!=0) break;
	}

	closedir(dp);

	return 0;

}

static int xmp_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

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


int main(int  argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
