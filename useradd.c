#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define passwd_file "/passwd"
#define max_length 100

int readLine(int fd, char *buf, int size)
{
    int i = 0, n = 0;
    char c;

    while (read( fd, &c, 1 ) > 0) 
    {
        n++;
        if (c != '\n'){
            buf[i] = c;
            i++;
            if (i == size - 1){
                break;
            }
        }
        else {
            break;
        }
    }
    buf[i] = '\0';
    return n;
}

int user_exist(char *str, char *user)
{
    int i = 0;

    while (str[i] != ':') 
    {
        if (str[i] != user[i]){
            return 0;
        }
        i++; 
    }
    return 1;
}

int get_current_id(char *str)
{
    int i = 0, j = 0;
    int id = 0;
    char str_id[2];
    while (str[i] != ':') 
    {
        i++; 
    }
    i++;
    while (str[i] != ':') 
    {
        str_id[j] = str[i];
        j++;
        i++; 
    }
    id = atoi(str_id);
    return id;
}

// Code retrieved from https://stackoverflow.com/questions/3982320/convert-integer-to-string-without-access-to-libraries
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char * itoa(int n, char s[])
{
    int i = 0;

    do 
    {      
        s[i++] = n % 10 + '0';  
    } while ((n /= 10) > 0);   

    s[i] = '\0';
    reverse(s);

    return s;
} 

int create_user(char *user, int id, char *pass)
{
    int fd;
    int w;
    char readbuf[1024];

    char strId[20];
    itoa(id, strId);

    char new_user[30];
    strcpy( new_user, user);
    strcpy( new_user + strlen(new_user), ":");
    strcpy( new_user + strlen(new_user), strId);
    strcpy( new_user + strlen(new_user), ":");
    strcpy( new_user + strlen(new_user), pass);
    strcpy( new_user + strlen(new_user), ":");
    strcpy( new_user + strlen(new_user), "\n");

    if ((fd = open(passwd_file, O_RDWR)) < 0)
    {
		printf(1, "No se puede crear el usuario, por favor intente de nuevo\n");
		return 0;
    }
    else
    {
		read(fd, readbuf, sizeof(readbuf));
		if ((w = write(fd, new_user, strlen(new_user))) > 0 )
        {
			printf(1,"Nuevo usuario agregado\n");
		};
		
		close(fd);
		return 1;
    }
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        printf(2, "Favor de escribir: useradd usuario contrasena\n");
        exit();
    }

    int fd_rd, n = 0;
    char record[max_length];
    int current_id = 0;

    fd_rd = open(passwd_file, O_RDONLY);

    while ((n = readLine(fd_rd, record, sizeof(record))) > 0)
    {
        if (user_exist(record, argv[1]))
        {
            printf(2, "El usuario ya existe, favor de escoger otro\n");
            exit();
        }
        current_id = get_current_id(record);
    }
    close(fd_rd);
    create_user(argv[1], current_id + 1, argv[2]);
    exit();
}