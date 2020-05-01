#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 256
char filelama[MAX][MAX];

    //DIR *dird = opendir("/home/kevinc45/fs.log");
static  const  char *dirpath = "/home/kevinc45/Documents";
char alphabet[100] = {"9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[Lm"};

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

static  int  xmp_getattr(const char *path, struct stat *stbuf){

	int res;
	char fpath[1000];
  //char pathsementara[1000];
    //printf("%s\n",pathsementara);
	char temp[1000];
	strcpy(temp, path);
	if(strncmp(path,"/encv1_",7)==0) caesarDecrypt(temp);
	sprintf(fpath, "%s%s",dirpath,temp);
  //printf("fpath getattr: %s\n",fpath);
	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		char temp[1000];
		if(strncmp(path,"/encv1_",7)==0) caesarDecrypt(temp);
		sprintf(fpath, "%s%s",dirpath,temp);
	}
  //printf("fpath read: %s\n",fpath);

	int res = 0;
	int fd = 0;

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
  //sprintf(pathsementara,"%s%s",dirpath,path);
	if((strcmp(fpath,"/") == 0))
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(pathsementara,path);
		if(strncmp(path,"/encv1_",7)==0) caesarDecrypt(pathsementara);
		sprintf(fpath, "%s/%s",dirpath,pathsementara);
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

		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;

		char temp[1000];
		strcpy(temp,de->d_name);
		if(strncmp(path,"/encv1_",7)==0) caesarcipher(temp);

		res = (filler(buf, temp, &st, 0));
		if(res!=0) break;
	}

	closedir(dp);

	return 0;
}

static int xmp_rename(const char *from, const char *to){
	char awal[1000], akhir[1000], temp[1000];
	if (strcmp(from, "/") == 0){
		from = dirpath;
		sprintf(awal, "%s", from);
	}
	else {
		strcpy(temp, from);
		if(strncmp(from, "/encv1_", 7) == 0) caesarDecrypt(temp);
		sprintf(awal, "%s%s", dirpath, temp);
	}

	if (strcmp(to, "/") == 0){
		to = dirpath;
		sprintf(akhir, "%s", to);
	}
	else {
		char temp2[1000];
		strcpy(temp2, to);
		if(strncmp(to, "/encv1_", 7) == 0)caesarDecrypt(temp2);

		sprintf(akhir, "%s%s", dirpath, temp2);
	}
	
	int res;

	res = rename(awal, akhir);
	if (res == -1)
		return -errno;

	logwriter(0,"RENAME",awal,akhir);
	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	logwriter(0,"WRITE",fpath,"");

	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		char temp[1000];
		strcpy(temp, path);
		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);
		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = mkdir(fpath, 0750);
	if (res == -1)
		return -errno;

	logwriter(0,"MKDIR",fpath,"");

	return 0;
}

static int xmp_unlink(const char *path)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	logwriter(1,"UNLINK",fpath,"");
	return 0;
}

static int xmp_rmdir(const char *path)
{
	char fpath[1000], temp[1000];
	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		strcpy(temp, path);
		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	logwriter(1,"RMDIR",fpath,"");

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char fpath[1000], temp[1000];
	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
	res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	logwriter(0,"CREATE",fpath,"");

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0)caesarDecrypt(temp);

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	
  //logwriter(0, "OPEN", fpath, "");

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
	.mknod = xmp_mknod,
	.open = xmp_open,
	.rename = xmp_rename
};

int main(int  argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
