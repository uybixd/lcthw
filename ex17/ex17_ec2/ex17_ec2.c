#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    struct Address *rows;
    int max_data;
    int max_rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_close(struct Connection *conn)
{
    if (conn) {
        if (conn->db) {
            for (int i = 0; i < conn->db->max_rows; i++) {
                if (conn->db->rows[i].name)
                    free(conn->db->rows[i].name);
                if (conn->db->rows[i].email)
                    free(conn->db->rows[i].email);
            }
            free(conn->db->rows);
            free(conn->db);
        }

        if (conn->file)
            fclose(conn->file);
        
        free(conn);
    }
}

void die(const char *message, struct Connection *conn)
{   

    if (conn) Database_close(conn);

    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

void Database_load(struct Connection *conn)
{
    /*rewind(conn->file);
    if (fread(&conn->db->max_data, sizeof(int), 1, conn->file) != 1)
        die("Failed to load max_data.", conn);

    if (fread(&conn->db->max_rows, sizeof(int), 1, conn->file) != 1)
        die("Failed to load max_rows.", conn);*/

    //conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    //if (!conn->db->rows)
        //die("Memory error: Failed to allocate rows.", conn);
    
    for (int i = 0; i < conn->db->max_rows; i++) {
        conn->db->rows[i].name = malloc(sizeof(char) * conn->db->max_data);
        conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);
        if (!conn->db->rows[i].name || !conn->db->rows[i].email)
            die("Memory error: Failed to allocate name or email in LOAD.", conn);

        if (fread(&conn->db->rows[i].id, sizeof(int), 1, conn->file) != 1)
            die("Failed to load id.", conn);
        if (fread(&conn->db->rows[i].set, sizeof(int), 1, conn->file) != 1)
            die("Failed to load set.", conn);
        if(fread(conn->db->rows[i].name, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load name.", conn);
        if(fread(conn->db->rows[i].email, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load email.", conn);
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error", conn);
    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error", conn);
    
    if (mode == 'c') {
        conn->file = fopen(filename, "w");
        // 这里为什么要初始化 conn->db->rows, 试试没有会怎么样
        //conn->db->rows = NULL;
    } else {
        conn->file = fopen(filename, "r+");
        if (conn->file) {

            fread(&conn->db->max_data, sizeof(int), 1, conn->file);
            fread(&conn->db->max_rows, sizeof(int), 1, conn->file);

            conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
            if (!conn->db->rows)
                die("Memory error: failed to allocate rows.", conn);

            for (int i = 0; i < conn->db->max_rows; i++) {
                conn->db->rows[i].name = malloc(conn->db->max_data);
                conn->db->rows[i].email = malloc(conn->db->max_data);
                if (!conn->db->rows[i].name || !conn->db->rows[i].email)
                    die("Memory error: failed to allocate name or email.", conn);
            }

            Database_load(conn);
        }     

    }

    if (!conn->file)
        die("Failed to open the file", conn);
    
    return conn;
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
    conn->db->max_data = max_data;
    conn->db->max_rows = max_rows;

    conn->db->rows = malloc(sizeof(struct Address) * max_rows);

    if (!conn->db->rows)
        die("Memory error: failed to allocate rows.", conn);

    for (int i = 0; i < max_rows; i++) {
        conn->db->rows[i].id = i;
        conn->db->rows[i].set = 0;
        // char = 1 , so you can do the malloc with only max_data
        conn->db->rows[i].name = malloc(sizeof(char) * max_data);
        conn->db->rows[i].email = malloc(sizeof(char) * max_data);

        if (!conn->db->rows[i].name || !conn->db->rows[i].email)
            die("Memory error: failed to allocate name or email.", conn);
        
        // malloc 分配的内存是未经清理的，可能有垃圾值，memset 用中间的值从指针开始的位置填入 0
        memset(conn->db->rows[i].name, 0, max_data);
        memset(conn->db->rows[i].email, 0, max_data);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);

    for (int i = 0; i < conn->db->max_rows; i++) {
        // fwrite 第一个参数需要一个地址
        fwrite(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
        fwrite(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
        fwrite(conn->db->rows[i].name, conn->db->max_data, 1, conn->file);
        fwrite(conn->db->rows[i].email, conn->db->max_data, 1, conn->file);
    }

    fflush(conn->file);
}

void Database_set(struct Connection *conn, int id, const char *name,
        const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die("Already set, delete it first", conn);

    addr->set = 1;

    char *res = strncpy(addr->name, name, conn->db->max_data);

    if (!res)
        die("Name copy failed", conn);
    
    // now assign the return value of copy emil to the variable res
    addr->email[conn->db->max_data - 1] = '\0';

    res = strncpy(addr->email, email, conn->db->max_data - 1);
    if (!res)
        // basicall the same, if failed, die
        die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{   
    struct Address addr = {.id = id, .set = 0, .name = conn->db->rows[id].name,.email = conn->db->rows[id].email };
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;

    struct Database *db = conn->db;

    for (i = 0; i < conn->db->max_rows; i++) {

        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{   
    struct Connection *conn = NULL;

    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]", conn);

    char *filename = argv[1];

    char action = argv[2][0];

    conn = Database_open(filename, action);

    int id = 0;
    
    if (argc > 3) id = atoi(argv[3]);

    // switch

    switch (action) {
        case 'c':{
            int max_data = 512;
            int max_rows = 100;
            if (argc == 5){
                max_data = atoi(argv[3]);
                max_rows = atoi(argv[4]);

                if (max_data <= 0 || max_rows <= 0)
                    die("MAX_DATA and MAX_ROWS must be positive integers.", conn);
            }

            Database_create(conn, max_data, max_rows);
            Database_write(conn);
            break;
        }
        case 'g':
            if (argc != 4)
                die("Need an id to get", conn);
            if (id >= conn->db->max_rows) die("There's not that many records.", conn);
            Database_get(conn, id);
            break;
        
        case 's':
            if (argc != 6)
                die("Need id, name, email to set", conn);
            if (id >= conn->db->max_rows) die("There's not that many records.", conn);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        
        case 'd':
            if (argc != 4)
                die("Need id to delete", conn);

            Database_delete(conn, id);

            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list",conn);
    }
    
    Database_close(conn);

    return 0;
}