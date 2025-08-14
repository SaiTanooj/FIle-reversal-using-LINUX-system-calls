#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string>
using namespace std;
off_t p=0;
void display_progress(off_t processed, off_t total_size) {
    double percent = (processed * 100.0) / total_size;
//    if (percent > 100.0) percent = 100.0;
    printf("\rprogress: %.2f%%", percent);
    fflush(stdout);
}
struct Input{
    const char* path;
    int flag;
    off_t block;
    off_t start;
    off_t end;

};//structure to take the arguments
struct Input take_Input(int argc, char *argv[]){
    struct Input ip;

    ip.path=argv[1];

    ip.flag=atoi(argv[2]);
    if(ip.flag==0){
        if(argc!=4){
            perror("Invalid number of argiments");
        }
        ip.block=(off_t)atoi(argv[3]);

    }
    if(ip.flag==1){
        if(argc!=3){
            perror("Invalid number of arguments");
        }
    }
    if(ip.flag==2){
        if(argc!=5){
            perror("INvalid number of arguments");
        }
        ip.start=(off_t)atoi(argv[3]);
        ip.end=(off_t)atoi(argv[4]);
    }
    return ip;
    
}
//==============Directory created===========================
void create_Directory(){
    int mkd=mkdir("Assignment1",S_IRWXU);
    if(mkd==0){
        write(1,"Directory created succesfully\n",31);
    }
    else{
        if(errno==EEXIST){
            perror("Directory already exists");
            return;
        }
        write(2,"Eroor while creating the directory\n",31);
    }
}
//============open_input======================================
int open_input(const char* path){
    int in=open(path,O_RDONLY);
    return in;
}
//===========open_output=======================================
int open_output(const char* path ,int flag){
    int last=-1;
    string name;
    for(int i=0;path[i]!='\0';i++){
        if(path[i]=='/'){
            last=i;
        }
    }
    for(int i=last+1;path[i]!='\0';i++){
        name=name+path[i];
    }

    string output_path="Assignment1/"+to_string(flag)+"_"+name;
    int out =open(output_path.c_str(),O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    return out;
}
//=======Flag_0 Logic============================================
void reverse_blockwise(const char* path,off_t block,off_t end){
    char *buffer= new char[block];
    if(block==0){
        perror("invalid block size block size cannot be emoty ");
        return ;
    }
    int in,out;
    size_t nread;
    in =open_input(path);
    if(in==-1){
        perror("error while opening the file");
        return ;
        
    }
    out=open_output(path,0); 
    if(out==-1){
        perror("error while creating  the output file");
        return ;
    }

    while((nread=read(in,buffer,block))>0){
        for(size_t i=0;i<nread/2;i++){
            char temp = buffer[i];
            buffer[i] = buffer[nread - 1 - i];
            buffer[nread - 1 - i] = temp;
        }
        if(write(out,buffer,nread)==-1){
            perror("Error while writing into file");
            return ;
        }
        p+= nread;
        display_progress(p,end);
        

    }
    close(in);
    close(out);
    delete[] buffer;

}
//===========Reverse full Logic(Common to flag 1 and flag 2)=======================================
void reverse_full(int in,off_t start,off_t end,int out){
    if(in==-1){
        perror("error while opening the file");
        return;
    }
    
    if(out==-1){
        perror("error while creating  the output file");
        return ;
    }
    size_t block;
    off_t len=end-start;
    if(len>8192){
        block=8192;
    }else{
        block=len;
    }//to decide the buffer size 
    char * buffer= new char[block];
    off_t current_pos=end;

    while(current_pos>start){
        off_t size = current_pos - start;
        off_t to_read;
        if(size>block){
            to_read=block;
        }else{
            to_read=size;
        }//bytes to read
        current_pos=current_pos-to_read;  
        lseek(in,current_pos,SEEK_SET);
        size_t nread=read(in,buffer,to_read);
         for (size_t i = 0; i < nread / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[nread - 1 - i];
            buffer[nread - 1 - i] = tmp;
        }
        write(out,buffer,nread);
        p+= nread;
        display_progress(p,end);
        
    }
    

}
void copy(int in,off_t s1,size_t size,off_t end,int out){
   if(in==-1){
    perror("Error while opening the i/p file");
   }
   if(out==-1){
    perror("Error while opening the o/p file");
   }
   size_t block;
   if(size>8192){
    block=8192;
   }else{
    block=size;
   }
   char* buffer = new char[block];
   off_t current_pos=s1;
   while(current_pos<end){
    off_t len=end-current_pos;
    off_t to_read;
    if(len>block){
        to_read=block;
    }else{
        to_read=len;
    }
    lseek(in,current_pos,SEEK_SET);
    size_t nread=read(in,buffer,to_read);
    write(out,buffer,nread);
    current_pos=current_pos+to_read;
    p=p+nread;
    display_progress(p,end);
   }    

}
int main(int argc, char *argv[]){
    create_Directory();//create a directory with the name Assignemnt1
    struct Input ip=take_Input(argc,argv);//Taking input form command line 
//=================================FLAG 0=================================
    if(ip.flag==0){
       int in=open_input(ip.path);
       off_t end=lseek(in,0,SEEK_END);
       if (end == (off_t)-1) {
        perror("Error seeking to end of file");
        close(in);
    
    }

       reverse_blockwise(ip.path,ip.block,end);
    }
//================================Flag 1=====================================

    if (ip.flag == 1) {
    int out =open_output(ip.path,1);
    int fd = open_input(ip.path);
    if (fd == -1) {
        perror("Error opening input file");
        close(fd);
    }

    off_t end = lseek(fd, 0, SEEK_END);
    if (end == (off_t)-1) {
        perror("Error seeking to end of file");
        close(fd);
    
    } 

    reverse_full(fd, 0, end,out);
    close(fd);
    close(out);
}

//=========================Flag-2======================================
       if (ip.flag == 2) {
    int fd = open_input(ip.path);
    if (fd == -1) {
        perror("Error opening input file");
        return 0;
    }

    off_t size1 = lseek(fd, 0, SEEK_END);
    if (size1 == 0) {
        fprintf(stderr, "Empty file error\n");
        close(fd);
        return 0;
    }

    if (ip.start < 0 || ip.end < 0 || ip.start > ip.end || ip.end >= size1) {
        perror("Error: invalid start/end arguments");
        close(fd);
        return 0;
    }

    int out = open_output(ip.path, 2);
    if (out == -1) {
        perror("Error creating output file");
        close(fd);
        return 0;
    }

    // Reverse first part
    reverse_full(fd, 0, ip.start, out);

    // Copy middle part
    size_t size = ip.end - ip.start + 1;
    copy(fd, ip.start, size, ip.end + 1, out);

    // Reverse last part
    reverse_full(fd, ip.end + 1, size1, out);

    close(fd);
    close(out);
}

    
    
}
