#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int do_getattr(const char* path,struct stat* st){
    printf("getattr called\n");
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if(strcmp(path,"/")==0){
        st->st_mode = __S_IFDIR | 0755;
        st->st_nlink = 2;
    }
    else{
        st->st_mode = __S_IFREG | 0644;
        st->st_nlink = 1; // no of hardlinks
        st->st_size = 1024;
    }
    return 0;


}

static int do_readdir(const char* path,void* buffer,fuse_fill_dir_t filler,off_t offset,struct fuse_file_info *fi){
    printf("Getting the list of files\n");
    filler(buffer,".",NULL,0);
    filler(buffer,"..",NULL,0);
    if(strcmp(path,"/")==0){
        filler(buffer,"file54",NULL,0);
        filler(buffer,"file349",NULL,0);
    }    
    return 0;
}

static int do_read(const char* path,char* buffer,size_t size,off_t offset,struct fuse_file_info *fi){
    char file54text[] = "Hello this is just some example I am trying to do.";
    char file349text[] = "Hello world from file349!";
    char* selectedtext = NULL;

    if(strcmp(path,"/file54")==0){
        selectedtext = file54text;
    }else if(strcmp(path,"/file349")){
        selectedtext = file349text;
    }else return -1;

    memcpy(buffer,offset+selectedtext,size);
    return strlen(selectedtext)-offset;
}

static struct fuse_operations  operation = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
};

int main(int argc,char* argv[]){
    return fuse_main(argc,argv,&operation,NULL);
}