#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    //char name[MAX_DATA];
    //char email[MAX_DATA];
	char *name;
	char *email;
};

struct Database {
	//struct Address rows[MAX_ROWS];
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
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) {
			if (conn->db->rows) {
				free(conn->db->rows);
				conn->db->rows = NULL;
			}
			free(conn->db);
			conn->db = NULL;
		}
        free(conn);
		conn = NULL;
    }
}

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

	Database_close(conn);
    exit(1);
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);
	int rc;

    //rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    //if(rc != 1) die("Failed to write database.", conn);

	// 写入 max_data 和 max_rows
    fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);

    // 写入每个 Address
    for (int i = 0; i < conn->db->max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];
        fwrite(&addr->id, sizeof(int), 1, conn->file);
        fwrite(&addr->set, sizeof(int), 1, conn->file);

        // 写入 name 和 email 的长度及其内容
        int name_len = 0;
		if (addr->name) name_len = strlen(addr->name);
        rc = fwrite(&name_len, sizeof(int), 1, conn->file);
        rc = fwrite(addr->name, sizeof(char), name_len, conn->file);

        int email_len = 0;
		if (addr->email) email_len = strlen(addr->email);
        rc = fwrite(&email_len, sizeof(int), 1, conn->file);
        rc = fwrite(addr->email, sizeof(char), email_len, conn->file);
    }

	// 将文件缓冲区中的数据强制写入到磁盘中。即便 fwrite 成功，数据可能仍然停留在文件的内存缓冲区中，fflush 会确保这些数据被物理写入磁盘。
    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_load(struct Connection *conn)
{
    //int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    //if(rc != 1) die("Failed to load database.", conn);
	
	fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    fread(&conn->db->max_rows, sizeof(int), 1, conn->file);

    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    if(!conn->db->rows) die("Cannot create memory for db-rows.", conn);

    for (int i = 0; i < conn->db->max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];

        fread(&addr->id, sizeof(int), 1, conn->file);
        fread(&addr->set, sizeof(int), 1, conn->file);

        // 读取 name 和 email 的长度及其内容
        int name_len;
        fread(&name_len, sizeof(int), 1, conn->file);
        addr->name = malloc(name_len);  // 不+1 预留 null 终止符
		if (!addr->name) die("Failed at create name", conn);
        fread(addr->name, sizeof(char), name_len, conn->file);
        addr->name[name_len] = '\0';  // 添加 null 终止符

        int email_len;
        fread(&email_len, sizeof(int), 1, conn->file);
        addr->email = malloc(email_len);
		if (!addr->email) die("Failed at create email", conn);
        fread(addr->email, sizeof(char), email_len, conn->file);
        addr->email[email_len] = '\0';
    }
}

struct Connection *Database_open(const char *filename, char mode, int argc, char *argv[])
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
		if (argc == 3) {
			conn->db->max_data = MAX_DATA;
			conn->db->max_rows = MAX_ROWS;
		} else {
			int max_data = atoi(argv[3]);
			int max_rows = atoi(argv[4]);
			conn->db->max_data = max_data;
			conn->db->max_rows = max_rows;
		}
		printf("  max_data is %d, max_rows is %d \n", conn->db->max_data, conn->db->max_rows);
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_create(struct Connection *conn)
{
	int max_rows = conn->db->max_rows;
	conn->db->rows = malloc(sizeof(struct Address) *max_rows);
    if(!conn->db->rows) die("Cannot create memory for db-rows.", conn);

    for(int i = 0; i < max_rows; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0, .name=NULL, .email=NULL};
        // then just assign it
        conn->db->rows[i] = addr;
		conn->db->rows[i].id = i;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) 
		die("Already set, delete it first", conn);

    addr->set = 1;
	int max_data = conn->db->max_data;

	addr->name = malloc(sizeof(char) * max_data);
    if(!addr->name) die("Create memory for name error", conn);
	addr->email = malloc(sizeof(char) * max_data);
    if(!addr->email) die("Create memory for email error", conn);

    // WARNING: bug, read the "How To Break It" and fix this
	
    char *res = strncpy(addr->name, name, max_data);
	// 手动将最后一个字符设置为 '\0'
	addr->name[max_data - 1] = '\0';
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, max_data);
	addr->email[max_data - 1] = '\0';
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    //struct Address addr = {.id = id, .set = 0};
    //conn->db->rows[id] = addr;

    struct Address *addr = &conn->db->rows[id];
	addr->set = 0;
	if (addr->name != NULL) {
		free(addr->name);
		addr->name = NULL;
	}
	if (addr->email != NULL) {
		free(addr->email);
		addr->email = NULL;
	}
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action, argc, argv);

    int id = 0;
    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
			Database_create(conn);
			Database_write(conn);
			break;

		// find
        case 'g':
            if(argc != 4) die("Need an id to get", conn);
            Database_get(conn, id);
            break;
		case 'l':
            Database_list(conn);
            break;

		// add, update
        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);
			printf(" access record %d \n", id);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

		// delete
        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}
