#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string>
#include<iostream>
using namespace std;
struct stat st;
struct Input{
    const char* in;
    const char* out;
    const char* dir;
    int flag;
    off_t block;
    off_t start;
    off_t end;

};//structure to take the arguments


void permissions(const char *file) {
    struct stat st;

    if (stat(file, &st) == -1) {
        const char* msg = "Error: cannot get file status\n";
        write(2,msg, strlen(msg));
        return;
    }
    const char* msg1="User has read permissions on newfile:";
    write(1,msg1, strlen(msg1));
    if (st.st_mode & S_IRUSR)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg2="User has write permission on newfile:";
    write(1,msg2,strlen(msg2));
    if (st.st_mode & S_IWUSR)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg3="User has execute permission on newfile:";
    write(1, msg3, strlen(msg3));
    if (st.st_mode & S_IXUSR)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg4="Group has read permissions on newfile:";
    write(1,msg4,strlen(msg4));
    if (st.st_mode & S_IRGRP)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg5="Group has write permission on newfile:";
    write(1,msg5,strlen(msg5));
    if (st.st_mode & S_IWGRP)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg6="Group has execute permission on newfile:";
    write(1,msg6,strlen(msg6));
    if (st.st_mode & S_IXGRP)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg7="Others has read permissions on newfile:";
    write(1,msg7,strlen(msg7));
    if (st.st_mode & S_IROTH)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg8="Others has write permission on newfile:";
    write(1,msg8,strlen(msg8));
    if (st.st_mode & S_IWOTH)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
    const char* msg9="Others has execute permission on newfile:";
    write(1,msg9,strlen(msg9));
    if (st.st_mode & S_IXOTH)
        write(1, "Yes\n", 4);
    else
        write(1, "No\n", 3);
}

   
    

int open_file(const char* path){
    int in=open(path,O_RDONLY);
    return in;
}
struct Input take_Input(int argc, char *argv[]){
    struct Input ip;

    ip.out=argv[1];
    ip.in=argv[2];
    ip.dir=argv[3];
    ip.flag=atoi(argv[4]);
    if(ip.flag==0){
         if(argc!=6){
            perror("Invalid number of arguments");
            exit(1);
        }

        ip.block=(off_t)atoi(argv[5]);

    }
    if(ip.flag==1){
        if(argc!=5){
            perror("Invalid number of arguments");
            exit(1);
        }
    }
    if(ip.flag==2){
        if(argc!=7){
            perror("INvalid number of arguments");
            exit(1);
        }
        ip.start=(off_t)atoi(argv[5]);
        ip.end=(off_t)atoi(argv[6]);
    }
    return ip;
    
}
bool same_size(int in1, int in2) {
if (lseek(in1, 0, SEEK_END)!= lseek(in2, 0, SEEK_END)) {
        write(1, "Incorrect file sizes\n", 21);
        close(in1);
        close(in2);
        return false;
    }
    return true;
}
bool flag0(int in1,int in2,size_t block){
     char* buffer1= new char[block];
     char* buffer2= new char[block];
     off_t nread1;
     off_t nread2;
while(((nread1=read(in1,buffer1,block))>0) && ((nread2=read(in2,buffer2,block))>0)){
        for (size_t i = 0; i < nread2 / 2; i++) {
    char tmp = buffer2[i];
    buffer2[i] = buffer2[nread2 - 1 - i];
    buffer2[nread2 - 1- i] = tmp;
}
for (size_t i = 0; i < nread1; i++) {
            if (buffer1[i] != buffer2[i]) {
        return false;}}}
return true;

}
bool flag1(int in1,int in2,off_t start,off_t end){
    off_t e1=end-start;
    size_t block;
    
    if(e1>block){
        block=8192;
    }else{
        block=e1;
    }
    char* buffer=new char[block];
    char* buffer2= new char[block];
    off_t curr1=end;//input
    off_t curr2=start;//output
    while(curr1>start && curr2<end){
        off_t size=curr1;
        off_t to_read;
        if(size>block){
            to_read=block;
        }else{
            to_read=size;
        }
        curr1=curr1-to_read;
        lseek(in1,curr1,SEEK_SET);
        size_t nread1=read(in1,buffer,to_read);
        for (size_t i = 0; i < nread1 / 2; i++) {
    char tmp = buffer[i];
    buffer[i] = buffer[nread1 - i - 1];
    buffer[nread1 - i - 1] = tmp;
}
        off_t len=end-curr2;
        off_t to_read2;
        if(len>block){
        to_read2=block;
    }else{
        to_read2=len;
    }
        lseek(in2,curr2,SEEK_SET);
        size_t nread2=read(in2,buffer2,to_read2);
        for (size_t i = 0; i < nread1; i++) {
            if (buffer[i] != buffer2[i]) {
        return false;}}
        curr2=curr2+to_read2;
            }
     
    return true;
    }
bool copy(int in,off_t start,size_t size,off_t end,int out){
    size_t block;
    if(size>block){
        block=8192;
    }else{
        block=size;
    }
    char* buffer1= new char[block];
    char* buffer2= new char[block];

    off_t curr1=start;
    off_t curr2=start;
    while(size>0){
        size_t to_read;
        if(size>block){
            to_read=block;
        }else{
            to_read=size;
        }
        lseek(in, curr1, SEEK_SET);
        size_t nread1=read(in,buffer1,to_read);
        lseek(in,curr2,SEEK_SET);
        size_t nread2=read(out,buffer2,to_read);
        for (size_t i = 0; i < nread1; i++) {
            if (buffer1[i] != buffer2[i]) {
        return false;}}
        curr1=curr1+nread1;
        curr2=curr2+nread2;
        size=size-nread1;
    }
    return true;

}
int main(int argc, char *argv[]){
    struct Input ip=take_Input(argc,argv);
    if(ip.flag==0){
        int in =open_file(ip.in);
        if(in==-1){
            perror("Error in opening input file");
            return 0;
        }
        int out=open_file(ip.out);
        if(out==-1){
            perror("Errir in opening the file");
            return 0;
        }
        
    if (stat(ip.dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        write(1, "Directory exists.\n", 18);
    } else {
        write(1, "Directory does not exist.\n", 27);
    }

        if(same_size(in,out)){
            bool check=flag0(in,out,ip.block);
            if(check){
                const char* msg="whether the contents are correctly processed :Yes\n";
            write(1,msg,strlen(msg));
                }else{
                    const char* msg1="whether the contents are correctly processed :No\n";
            write(1,msg1,strlen(msg1));
            }
        }
        if(same_size(in,out)==true){
            const char* msg="Both file sizes are same:Yes\n";
            write(1,msg,strlen(msg));
        }else{
            const char* msg1="Both file sizes are same:NO\n";
            write(1,msg1,strlen(msg1));
        }
       permissions(ip.dir);
       permissions(ip.in);
       permissions(ip.out);
    }
    if(ip.flag==1){
        int in=open_file(ip.in);
        if(in==-1){
            perror("Error in opening input file");
            return 0;
        }
        int out=open_file(ip.out);
        if(out==-1){
            perror("Errir in opening the file");
            return 0;
        }
        if (stat(ip.dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        write(1, "Directory exists.\n", 18);
    } else {
        write(1, "Directory does not exist.\n", 27);
    }
        if(same_size(in,out)){
            off_t e1=lseek(in,0,SEEK_END);
            bool check=flag1(in,out,0,e1);
            if(check){
                const char* msg="whether the contents are correctly processed :Yes\n";
            write(1,msg,strlen(msg));
                }else{
                    const char* msg1="whether the contents are correctly processed :No\n";
            write(1,msg1,strlen(msg1));
            }
        }
        if(same_size(in,out)==true){
            const char* msg="Both file sizes are same:Yes\n";
            write(1,msg,strlen(msg));
        }else{
            const char* msg1="Both file sizes are same:NO\n";
            write(1,msg1,strlen(msg1));
        }
       permissions(ip.dir);
       permissions(ip.in);
       permissions(ip.out);   

    }
    if(ip.flag==2){
        int in=open_file(ip.in);
        if(in==-1){
            perror("Error in opening input file");
            return 0;
        }
        off_t eof=lseek(in,0,SEEK_END);
        int out=open_file(ip.out);
        if(out==-1){
            perror("Errir in opening the file");
            return 0;
        }
        if (stat(ip.dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        write(1, "Directory exists.\n", 18);
    } else {
        write(1, "Directory does not exist.\n", 27);
    }
        if(same_size(in,out)){
            bool c1=flag1(in,out,0,ip.start);
            size_t size = ip.end - ip.start + 1;
            bool c2=copy(in,ip.start, size, ip.end + 1, out);
            bool c3=flag1(in,out,ip.end+1,eof);
            if(c1 && c2 && c3){
            const char* msg="whether the contents are correctly processed :Yes\n";
            write(1,msg,strlen(msg));
                }else{
                    const char* msg1="whether the contents are correctly processed :No\n";
            write(1,msg1,strlen(msg1));
            }   
        
        }
        if(same_size(in,out)==true){
            const char* msg="Both file sizes are same:Yes\n";
            write(1,msg,strlen(msg));
        }else{
            const char* msg1="Both file sizes are same:NO\n";
            write(1,msg1,strlen(msg1));
        }
        permissions(ip.dir);
       permissions(ip.in);
       permissions(ip.out); 
    }




}

























