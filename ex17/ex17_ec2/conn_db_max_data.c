/*试验一下 conn->db->max_data 与 &conn->db->max*/

#include <stdio.h>

struct Database {
    int max_data;
};

struct Connection {
    struct Database *db;
};

int main(int argc, char *argv[])
{   

    struct Database db = { .max_data = 10};
    struct Connection conn = { .db = &db};

    printf("The vale of the max_data: %d\n", conn.db->max_data);

    printf("The address of the max_data: %p\n", &conn.db->max_data);

    return 0;
}