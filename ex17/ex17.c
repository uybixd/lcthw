// import some head file for cpp to put in
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
// 为了使用 errno
#include <errno.h>
#include <string.h>

// Create two identifier, Define marco, it will be replaced with the token when compiling the source file
#define MAX_DATA 512
#define MAX_ROWS 100

// defile a struct named Address
// Address has for element
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

// Define a struct named Databse
// the element of Database is a Address named rows[MAX_ROWS]
struct Database {
    struct Address rows[MAX_ROWS];
}; 

// Define a struct named Connection
// The first element is a FILE pointer named file
// The second element is a Database type pointer named db
struct Connection {
    FILE *file;
    struct Database *db;
};

// define a function named die and take a const char pointer as argument
void die(const char *message)
{   
    // errno catch the error code from last function call or system call
    if (errno) {
        // print a specifical string you want + the error massage map to the value of errno
        perror(message);
        //printf(">>> errno value %d\n", errno);
    } else {
        // if errno is undefined, yes it won't be set to 0 when the function call success
        printf("ERROR: %s\n", message);
    }
    // abort the program and return a value to OS as a status value, you can see that in lldb
    exit(1);
}

// Define a function named Address_print
// it takes a struct Address pointer named addr as parameter
void Address_print(struct Address *addr)
{
    // print three format string
    // the first one is the value of id which in the addr, the rest two are the samilar
    // that is how you access the element in a point of struct
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

// Define a function named Database_load
// takes struct Connection pointer named conn as a parameter
void Database_load(struct Connection *conn)
{
    /*  declare a int variable named rc
        set rc to the return of fread()
        the fread return how many objects it read
        the "content" of read will set to the conn->db
        the "sizeof(struct Database) tell the fread how long to read"
        The "1" tell the fread how many to read
        The "conn->file" is the target file
        */
    //  here should use size_t, an unsigned int
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    // make sure you read one database a time
    if (rc != 1)
        // die and print some message
        die("Failed to load database.");
}

// Make a function named Database_open and the type of it is struct Connection pointer
// This function take two parameters, first is a pointer type const char pointer named 'filename', 
// and second is a char named 'mode'
struct Connection *Database_open(const char *filename, char mode)
{   
    // allocate a piece of memory to the conn which is a pointer of struct Connection
    // and the size fo the memory is equal to the size of "struct Connection"
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        // if the allocate is failed die and print some error massage
        die("Memory error");
    // allocate a piece of memory to the pointer inside conn
    // and the size is equal to "struct Database"
    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        // If this memory allocate failed die and print some error message
        die("Memory error");
    // If mode is c
    if (mode == 'c') {
        // use the fopen function in "w" mode which will create a file if it doesn't exist
        conn->file = fopen(filename, "w");

    } else {
        // if not the "c" mode open it with "r+"
        conn->file = fopen(filename, "r+");
        // if fopen the filename successed
        if (conn->file) {
            // Call the Database_load function on conn
            Database_load(conn);
        }
    }
    // if failed to open the file, print an error message
    if (!conn->file)
        die("Failed to open the file");
    // this function (Database_open) returns conn
    return conn;
}

// Define a function called Database_close
// It takes a struct Connection pointer as parameter named conn
void Database_close(struct Connection *conn)
{
    if (conn) {
        // if the conn exist
        if (conn->file)
            // if the conn->file exist, call the fclose on conn->file
            fclose(conn->file);
        if (conn->db)
            // if the conn->db exist, call the free on conn->db
            free(conn->db);
        // free the conn itself
        // do I have to free the element of the pointer conn, before free the conn itself
        free(conn);
    }
}
// Define a function named Database_write
// It takes a struct Connection pointer named conn as parameter
void Database_write(struct Connection *conn)
{
    //set the file position indicator for the conn->file back to the beginning of the file
    rewind(conn->file);
    // define a variable named rc and set it to the return of function fwrite
    // write the conn->db to the conn->file
    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        // if the rc not equal to 1 then die and print an error message
        die("Failed to write database.");
    // force to writing the data in buffer into the file in disk
    rc = fflush(conn->file);
    if (rc == -1)
        // if the return of function fflush is -1
        // die and print an error message
        die("Cannot flush database");
}

// Define a function name Database_create
// This function takes a struct Connection pointer as parameter
void Database_create(struct Connection *conn)
{
    int i = 0;
    // the max capacity is 100
    for (i=0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        // the way to create a addr is interesting, it just use the .id and .set
        // I got it, this will create 101 "empty" database first and all the .set is equal to 0
        struct Address addr = {.id = i, .set = 0 };
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

// Define a function named Database take four parameters
// a pointer named conn type is struct Connection, an int id, a const char pointer name
// and another const char pointer email
// const char * means mutable poiter to an immutable string
void Database_set(struct Connection *conn, int id, const char *name,
        const char *email)
{
    // create variable named addr is a struct Address pointer
    // and assigen the address of rows[id], which is in db, which is in conn
    struct Address *addr = &conn->db->rows[id];
    // if the addr->set exist
    if (addr->set)
        // Die and print an error message
        die("Already set, delete it first");
        // If the die function has been called, this sentence won't run
        // why set the addr->set to 1
    addr->set = 1;
        // WARNING: bug, read the "How To Break It" and fix this
    // copy the specifical length of characters of name to the addr->name
    // assign the return value which is the addr->name to the variable res which is a char pointer
    char *res = strncpy(addr->name, name, MAX_DATA);
    // demonstrate the strncpy bug
    // If the res doesn't exist, means strncpy failed, print an error message and die
    if (!res)
        die("Name copy failed");
    
    // now assign the return value of copy emil to the variable res
    addr->email[MAX_DATA - 1] = '\0';

    res = strncpy(addr->email, email, MAX_DATA - 1);
    if (!res)
        // basicall the same, if failed, die
        die("Email copy failed");
}

// Define a function named Database_get, this function returns nothing
// It takes two parameters, conn and id
void Database_get(struct Connection *conn, int id)
{
    // create a variable in this function named addr
    // assign the address of rows[id] to it
    // rows[id] is in db, which is in conn
    struct Address *addr = &conn->db->rows[id];

    // If the addr->set exist
    if (addr->set) {
        // call the Address_print() function with addr
        Address_print(addr);
    } else {
        // else print an error message and die
        die("ID is not set");
    }
}

// Define a function named Database_delete and this function returns nothing
void Database_delete(struct Connection *conn, int id)
{   
    // I don't understand why assign the addr to rows[id]
    // This should delete the conn->db->rows[id], but I don't how
    // And now I got it, the database in the file not "actually" been delete, but just set the "set" to 0
    struct Address addr = {.id = id, .set = 0 };
    // assign addr to rows[id], which is in db, which is in conn
    conn->db->rows[id] = addr;
}

// Define a function named Database_list
// This function takes conn as parameter
void Database_list(struct Connection *conn)
{
    int i = 0;
    // create a pointer named db which is a struct Database
    // assign the conn->db to db
    struct Database *db = conn->db;

    // for i < MAX_ROWS
    for (i = 0; i < MAX_ROWS; i++) {
        // create a pointer which is a struct Address
        // Point cur to the address of db->rows[i]
        struct Address *cur = &db->rows[i];

        // If the cur->set exist, this will only print those databse whose set value greater than 0
        if (cur->set) {
            // Call the Address_print with cur
            Address_print(cur);
        }
    }
}

// Define the main function
int main(int argc, char *argv[])
{   
    // If the arguments less than 3
    if (argc < 3)
        // call the die function with a string
        die("USAGE: ex17 <dbfile> <action> [action params]");
    // create a char pointer named filename and assign argv[1] to it
    char *filename = argv[1];
    // create a char variable and assign argv[2][0] to it
    // but this just use the first character of the third argument
    char action = argv[2][0];
    // create a struct Connection pointer and assign the return of function Database_open
    // call the Database_open with filename and action
    struct Connection *conn = Database_open(filename, action);
    // create a variable int, assign 0 to it
    int id = 0;
    
    // if the arguments greater than 3
    // assign the return of atoi of argv[3]
    if (argc > 3) id = atoi(argv[3]);
    // It should die when id greater than MAX_ROWS which equal to 100
    // But I have tried 'a', 'b', and 'c' it all set id to 0
    if (id >= MAX_ROWS) die("There's not that many records.");

    // switch
    switch (action) {
        case 'c': 
            // for action equal to c call Database_create and Databse_write
            Database_create(conn);
            Database_write(conn);
            break;
        
        case 'g':
        // In case g, if the argc not equal to 4
            if (argc != 4)
                // call the die function with a string
                die("Need an id to get");
            // if the argc equal to 4
            // call the Database_get with conn and id
            Database_get(conn, id);
            break;
        
        case 's':
            // In case 's', if argc equal not equal to 6
            if (argc != 6)
                // call the die function with a string
                die("Need id, name, email to set");
            // else call the Database_set with those parameters
            Database_set(conn, id, argv[4], argv[5]);
            // write the database into the file
            Database_write(conn);
            break;
        
        case 'd':
            // In the 'd' case
            if (argc != 4)
                // if the argc not equal to 4, call the die fucntion with a string
                die("Need id to delete");
            
            // else, call the Database_delete with conn and id
            Database_delete(conn, id);
            // Call Database write to conn
            Database_write(conn);
            break;

        case 'l':
            // In the 'l' case, call the Databsae_list with conn
            Database_list(conn);
            break;
        // a default branch to catch the unconsidered situation
        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list");
    }
    
    // call the Database_close on the conn
    Database_close(conn);

    // return 0 if the programming run well
    return 0;
}