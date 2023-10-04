// Server.cpp : create a console application, and include the sources in the project
//
// 1. open the *.c in the Visual C++, then "rebuild all".
// 2. click "yes" to create a project workspace.
// 3. You need to -add the library 'ws2_32.lib' to your project
//    (Project -> Properties -> Linker -> Input -> Additional Dependencies)
// 4. recompile the source.

// 2030026036 Auberon
// 2030026117 Joshua
// 2030026116 Vicky

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mysql.h"
#include <windows.h>
#include <process.h>

#define DEFAULT_PORT	5019
struct thread{
    SOCKET sock;
    int t_id;//thread id
    char ip[20];//ip address for client
    int count;
};
typedef thread Value;

int actionType;
Value threadNeed[10];

char **receiveMessage;
void split(char *src,const char *separator,char **dest,int *num);
void convertReceive(char message[], int num);
void fetchDB(void*);


int main(int argc, char **argv){

    char szBuff[100];
    int msg_len;
    int addr_len;
    struct sockaddr_in local, client_addr;

    SOCKET sock, msg_sock;
    WSADATA wsaData;

    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR){
        // stderr: standard error are printed to the screen.
        fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
        //WSACleanup function terminates use of the Windows Sockets DLL.
        WSACleanup();
        return -1;
    }
    // Fill in the address structure
    local.sin_family		= AF_INET;
    local.sin_addr.s_addr	= INADDR_ANY;
    local.sin_port		= htons(DEFAULT_PORT);

    sock = socket(AF_INET,SOCK_STREAM, 0);	//TCp socket


    if (sock == INVALID_SOCKET){
        fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR){
        fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Waiting for connections ........\n");
    //waiting for the connections
    if (listen(sock, 5) == SOCKET_ERROR){
        fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    //create threads for 3 connections
    for(int i=0; i<4; i++){

        addr_len = sizeof(client_addr);
        msg_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
        if (msg_sock == INVALID_SOCKET){
            fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        char client_address[20];
        _snprintf(client_address, 20, "%s", inet_ntoa(client_addr.sin_addr));
        printf("Client %d accepted connection from %s, port %d\n", i+1, client_address, htons(client_addr.sin_port));

        threadNeed[i].sock = msg_sock;
        threadNeed[i].t_id = i+1;
        threadNeed[i].count = 0;

        strcpy(threadNeed[i].ip, client_address);
        //create thread for each connection and go to database to fetch data.
        _beginthread(fetchDB, 0, &threadNeed[i]);
    }
    _endthread();
    if (msg_sock != INVALID_SOCKET){
        closesocket(msg_sock);
        msg_sock = INVALID_SOCKET;
    }
    WSACleanup();
    return 0;
}

void fetchDB(void* dummy){
    Value * currThread = (Value *) dummy;
    int msg_len;
    //connect to database
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL *con = mysql_init(NULL);
    char szBuff[100];
    if(con == NULL)
    {
        fprintf(stderr, "Client %d: %s\n", currThread->t_id, mysql_error(con));
        exit(EXIT_FAILURE);
    }
    if(!mysql_real_connect(con, "localhost", "root", "", "course_info", 0, NULL,0))// "localhost" is ip address,"root" is user name, "" is password,"library" is database name,
    {
        fprintf(stderr, "Client %d: %s\n", currThread->t_id, mysql_error(con));
        mysql_close(con);
        exit(EXIT_FAILURE);
    }



    int check = 0;
    while(check==0){
        //Sleep(10000);
        //receive username from client.
        memset(szBuff,'\0',sizeof(szBuff));

        msg_len = recv(currThread->sock, szBuff, sizeof(szBuff), 0);
        if (msg_len == SOCKET_ERROR){
            fprintf(stderr, "Client %d recv() failed with error %d\n", currThread->t_id, WSAGetLastError());
            return;
        }
        if (msg_len == 0){
            printf("Client %d closed connection\n", currThread->t_id);
            //if (currThread->sock != INVALID_SOCKET){
            //	closesocket(currThread->sock);
            //	currThread->sock = INVALID_SOCKET;
            //}
            return;
        }

        //make sql string
        char SQLfindPriority[100];
        strcpy(SQLfindPriority, "select priority from user where user_name='");
        strcat(SQLfindPriority, szBuff);
        strcat(SQLfindPriority, "'");

        //send username to database
        if(mysql_query(con, SQLfindPriority))
        {
            fprintf(stderr, "Client %d: Faild %s\n",currThread->t_id, mysql_error(con));
        }
        res=mysql_store_result(con);
        //num_fields= mysql_num_fields(res);
        //field=mysql_fetch_fields(res);

        printf("this is sql login search pri:%s",SQLfindPriority);

        row = mysql_fetch_row(res);
        check = res->row_count;
        if(check==0)
            msg_len = send(currThread->sock, "0", sizeof("0"), 0);
        else{
            if ( atoi(row[0])==1)
                msg_len = send(currThread->sock, "100", sizeof("100"), 0);
            else
                msg_len = send(currThread->sock, "200", sizeof("200"), 0);
        }
        printf("check %d", check);



    }

    while(1)
    {
        memset(szBuff, '\0',sizeof(szBuff));

        msg_len = recv(currThread->sock, szBuff, sizeof(szBuff), 0);

        //if actionType is 0, szBuff has two strings, so we should split them in two sub strings.

        if (szBuff[0]-48 == 0)
            convertReceive(szBuff, 2);
        //if actionType is 1, szbuff have two strings.
        if (szBuff[0]-48 == 1){
            convertReceive(szBuff, 2);
            // need to change to int, later sql add 1
        }
        //action type is 2
        if (szBuff[0]-48 == 2)
            convertReceive(szBuff, 3);
        //action tupe is 3
        if (szBuff[0]-48 == 3)
            convertReceive(szBuff, 2);
        //action type is 4
        if (szBuff[0]-48 == 4)
            convertReceive(szBuff, 4);
        if (szBuff[0]-48 == 5)
            convertReceive(szBuff, 2);
        //action type is 9
        if (szBuff[0]-48 == 9){
            printf("test1\n");
            convertReceive(szBuff, 4);
            printf("test2\n");
        }


        if (msg_len == SOCKET_ERROR){
            fprintf(stderr, "Client %d recv() failed with error %d\n", currThread->t_id, WSAGetLastError());
            WSACleanup();
            return;
        }

        if (msg_len == 0){
            printf("Client %d closed connection\n", currThread->t_id);
            //closesocket(currThread->sock);
            return;
        }
        else if (msg_len<0){
            printf("recv message is error");
        }

//		printf("Test: %d, ", actionType);
//		printf("para1:%s \n", receiveMessage[0]);

        switch(actionType){
        //case 0 is for student searching the course
        case 0:
            //make sql string
            char SQLfindCourses[100];
            strcpy(SQLfindCourses, "select * from course where CourseTitle like '%");
            strcat(SQLfindCourses, receiveMessage[0]);
            strcat(SQLfindCourses, "%'");
            //send course title to database
            if(mysql_query(con, SQLfindCourses))
            {
                fprintf(stderr, "Client %d: Faild %s\n", currThread->t_id, mysql_error(con));
                exit(EXIT_FAILURE);
            }
            res=mysql_store_result(con);
            //num_fields= mysql_num_fields(res);
            field=mysql_fetch_fields(res);
            row = mysql_fetch_row(res);
            int count;
            count = res->row_count;
//            char countString[100];
//            memset(countString, '\0', sizeof(countString));
//            itoa(count, countString, 10);
//            Sleep(100);
//            msg_len = send(currThread->sock, countString, sizeof(countString), 0);
//            memset(countString, '\0', sizeof(countString));
            if(count!=0){
                char sendCourse[500];
                memset(sendCourse, '\0', sizeof(sendCourse));
                while (row = mysql_fetch_row(res)){

                    sprintf(sendCourse, "%s, %s, %s, %s, %s, %s, %s", row[0], row[1], row[2], row[3], row[4], row[5],row[6]);//the 0 row and the 1 row
                    Sleep(100);
                    printf("%s\n", sendCourse);
                    msg_len = send(currThread->sock, sendCourse, sizeof(sendCourse), 0);
                    memset(sendCourse, '\0', sizeof(sendCourse));
                }
            }
            if (count==0){
                for(int i=0; i<8; i++){
                    Sleep(200);
                    msg_len = send(currThread->sock, "can not find this course", sizeof("can not find this course"), 0);
                }
            }
            mysql_free_result(res);
            break;
        case 1://add a student
            char send_tf[50];
            char SQLaddStudent[80];
            strcpy(SQLaddStudent, "INSERT INTO user (user_name, priority) VALUES ('" );
            strcat(SQLaddStudent, receiveMessage[0]);
            strcat(SQLaddStudent, "', 1)");
            //add student to database
            if(mysql_query(con, SQLaddStudent))
            {
                //test here Ziao FENG
                printf("this is fail to add");
                strcpy(send_tf, "Fail to add this student (may already exist)");

                fprintf(stderr, "Client: %d Faild %s\n", currThread->t_id, mysql_error(con));
//                msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
                for (int it = 0;it<8;it++){
                    Sleep(200);
                    msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
                }
            }
            else{
                //test here Ziao FENG
                printf("this is success to add");
                res=mysql_store_result(con);
                strcpy(send_tf, "Add student successfully." );

//                msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
                for (int it1 = 0;it1<8;it1++){
                    Sleep(200);
                    msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
                }
//                Sleep(1000);
//                msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
//                Sleep(1000);
//                msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
//                Sleep(1000);
//                msg_len = send(currThread->sock, send_tf, sizeof(send_tf), 0);
                mysql_free_result(res);
            }

            break;
        //This is the case for updating the student
        case 2:
            char SQLupdateStudent[80];
            strcpy(SQLupdateStudent, "UPDATE user SET priority =" );
            strcat(SQLupdateStudent, receiveMessage[1]);
            strcat(SQLupdateStudent, " WHERE user_name ='");
            strcat(SQLupdateStudent, receiveMessage[0]);
            strcat(SQLupdateStudent, "'");
            //update student to database
            if(mysql_query(con, SQLupdateStudent))
            {
                fprintf(stderr, "Faild %s\n", mysql_error(con));
                msg_len = send(currThread->sock, "Fail to update this student.", sizeof("Fail to update this student."), 0);
            }
            else{
                char SQLcheckUpdate[100];
                strcpy(SQLcheckUpdate,"SELECT user_name, priority FROM user WHERE `user_name`='");
                strcat(SQLcheckUpdate, receiveMessage[0]);
                strcat(SQLcheckUpdate, "' and `priority`=");
                strcat(SQLcheckUpdate, receiveMessage[1]);

                mysql_query(con, SQLcheckUpdate);
                res=mysql_store_result(con);
                row = mysql_fetch_row(res);
                for(int j=0; j<8;j++){
                    if (row>0){
                        msg_len = send(currThread->sock, "Update student successfully.", sizeof("Update student successfully."), 0);
                    }else{
                        msg_len = send(currThread->sock, "Fail to update this student.", sizeof("Fail to update this student."), 0);
                    }
                }
                mysql_free_result(res);
            }
            break;
        //This is for deleting the student
        case 3:
            char SQLcheckDelete[100];
            strcpy(SQLcheckDelete,"SELECT user_name FROM user WHERE `user_name`='");
            strcat(SQLcheckDelete, receiveMessage[0]);
            strcat(SQLcheckDelete, "'");
            mysql_query(con, SQLcheckDelete);
            res=mysql_store_result(con);
            row = mysql_fetch_row(res);
            if (res->row_count<=0){
                for (int iter=0;iter<8;iter++){
                    Sleep(200);
                    msg_len = send(currThread->sock, "This student doesn't exist.", sizeof("This student doesn't exist."), 0);
                }
            }else{
                char SQLdeleteStudent[80];
                strcpy(SQLdeleteStudent, "DELETE FROM `user` WHERE `user_name` ='" );
                strcat(SQLdeleteStudent, receiveMessage[0]);
                strcat(SQLdeleteStudent, "'");
                //delete student to database
                if(mysql_query(con, SQLdeleteStudent))
                {
                    fprintf(stderr, "Client %d: Faild %s\n", currThread->t_id, mysql_error(con));
                    for (int it1 = 0;it1<8;it1++){
                        Sleep(200);
                        msg_len = send(currThread->sock, "Fail to delete this student.", sizeof("Fail to delete this student."), 0);
                    }

                    mysql_free_result(res);
                }
                else{
                    for (int it1 = 0;it1<8;it1++){
                        Sleep(200);
                        msg_len = send(currThread->sock, "Delete student successfully.", sizeof("Delete student successfully."), 0);
                    }
                    Sleep(200);

                    mysql_free_result(res);
                }
            }
            break;
        //This is the case for updating the course.
        case 4:
            char SQLcheckCourse[100];
            strcpy(SQLcheckCourse,"SELECT * FROM course WHERE `MID`=");
            strcat(SQLcheckCourse, receiveMessage[0]);
            mysql_query(con, SQLcheckCourse);
            res=mysql_store_result(con);
            row = mysql_fetch_row(res);
            //This case is for checking the course is exit or not
            if (res->row_count<=0)
                msg_len = send(currThread->sock, "This course doesn't exist.", sizeof("This course doesn't exist."), 0);
            else{
                char SQLcheckInfo[100];
                strcpy(SQLcheckInfo,"SELECT * FROM course WHERE `Classroom`='");
                strcat(SQLcheckInfo, receiveMessage[1]);
                strcat(SQLcheckInfo, "' AND `Time` LIKE '%");
                strcat(SQLcheckInfo, receiveMessage[2]);
                strcat(SQLcheckInfo, "%'");
                printf("%s", SQLcheckInfo);

                mysql_query(con, SQLcheckInfo);
                res = mysql_store_result(con);
                row = mysql_fetch_row(res);
                if ((res->row_count)>0){
                    for(int iamhappy=0; iamhappy<8; iamhappy++){
                        Sleep(200);
                        msg_len = send(currThread->sock, "This time or classroom crash.", sizeof("This time or classroom crash."), 0);
                    }
                }else{
                    char SQLupdateCourse[80];
                    strcpy(SQLupdateCourse, "UPDATE `course` SET `Classroom` ='" );
                    strcat(SQLupdateCourse, receiveMessage[1]);
                    strcat(SQLupdateCourse, "', `Time`='");
                    strcat(SQLupdateCourse, receiveMessage[2]);
                    strcat(SQLupdateCourse, "' WHERE `MID`= ");
                    strcat(SQLupdateCourse, receiveMessage[0]);
                    //delete student to SQLupdateCourse
                    if(mysql_query(con, SQLupdateCourse))
                    {
                        fprintf(stderr, "Client %d: Faild %s\n", currThread->t_id, mysql_error(con));
                        for(int iamhappy=0; iamhappy<8; iamhappy++){
                            Sleep(200);
                            msg_len = send(currThread->sock, "Fail to update this course.", sizeof("Fail to update this course."), 0);

                        }
                    }
                    else{
                        for(int iamhappy=0; iamhappy<8; iamhappy++){
                            Sleep(200);
                            msg_len = send(currThread->sock, "Update course successfully.", sizeof("Update course successfully."), 0);
                        }
                        mysql_free_result(res);
                    }
                }

            }
            break;//Remember to break!
        case 5:
            //make sql string
            char SQLfindUsers[100];
            strcpy(SQLfindUsers, "select * from user where user_name like '%");
            strcat(SQLfindUsers, receiveMessage[0]);
            strcat(SQLfindUsers, "%'");
            //send name to database
            if(mysql_query(con, SQLfindUsers))
            {
                fprintf(stderr, "Client %d: Faild %s\n", currThread->t_id, mysql_error(con));
                exit(EXIT_FAILURE);
            }
            res=mysql_store_result(con);
            //num_fields= mysql_num_fields(res);
            field=mysql_fetch_fields(res);
            //row = mysql_fetch_row(res);
            int count2;
            count2 = res->row_count;
            printf("count %d\n", count2);
            char countString2[10];
            itoa(count2, countString2, 10);
            msg_len = send(currThread->sock, countString2, sizeof(countString2), 0);
            char sendUsers[20];
            if(count2!=0){

                while (row = mysql_fetch_row(res)){
                    sprintf(sendUsers, "%s, %s", row[0], row[1]);//the 0 row and the 1 row
                    Sleep(100);
                    printf("%s", sendUsers);
                    msg_len = send(currThread->sock, sendUsers, sizeof(sendUsers), 0);
                }
            }
            if(count2==0){
                msg_len = send(currThread->sock, "can not find this user", sizeof("can not find this user"), 0);
            }
            mysql_free_result(res);
            break;
        //case 9 is for exiting the connection and upload the user data to database
        case 9:
            if(currThread->count==0){
                printf("Client %d closed connection\n", currThread->t_id);
                char SQLexit[1000];
                strcpy(SQLexit, "insert into history (USER_NAME, IP_ADDR, CONNECTION_TIME, RUN_TIME) values ('" );
                strcat(SQLexit, receiveMessage[0]);
                strcat(SQLexit, "', '");
                strcat(SQLexit, currThread->ip);
                strcat(SQLexit, "', '");
                strcat(SQLexit, receiveMessage[1]);
                strcat(SQLexit, "', '");
                strcat(SQLexit, receiveMessage[2]);
                strcat(SQLexit, "')");

                //send course title to database
                if(mysql_query(con, SQLexit))
                {
                    fprintf(stderr, "Client %d Faild %s\n", currThread->t_id, mysql_error(con));
                    mysql_close(con);
                    exit(EXIT_FAILURE);
                }
                //printf("0hhahahahahahahahaha\n");
                res=mysql_store_result(con);
                //printf("1hhahahahahahahahaha\n");
                //field=mysql_fetch_fields(res);
                //printf("2hhahahahahahahahaha\n");
                mysql_free_result(res);
                //if (currThread->sock != INVALID_SOCKET){
                    //closesocket(currThread->sock);
                    //currThread->sock = INVALID_SOCKET;
                //}
                //printf("3hhahahahahahahahaha\n");
                //printf("4hhahahahahahahahaha\n");
                (currThread->count)++;
            }
            break;
        }//end of switch
        Sleep(1000);
        //printf("this is loop: %d",currThread->t_id);
    }//end of while loop
}

void convertReceive(char message[], int num){

    char *revbuf[8] = {0};
    split(message, ";", revbuf, &num);
    char **temp = (char **)malloc(num*sizeof(char *));
    actionType = atoi(revbuf[0]);

    for(int i = 1;i < num; i ++) {
        //printf("revbuf: %s", revbuf[i]);
        temp[i-1] = (char *)malloc(strlen(revbuf[i])*sizeof(char));
        strcpy(temp[i-1], revbuf[i]);
        printf("Convert %s \n", temp[i-1]);
    }
    receiveMessage = temp;
}

void split(char *src,const char *separator,char **dest,int *num) {
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
       return;
    if (separator == NULL || strlen(separator) == 0)
       return;
    pNext = strtok(src,separator);
    while(pNext != NULL) {
         *dest++ = pNext;
          ++count;
          pNext = strtok(NULL,separator);
    }
    *num = count;
}
