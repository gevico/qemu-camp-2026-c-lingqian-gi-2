#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// URL解析结果结构体
typedef struct {
    char scheme[32];   // 协议：http/https/ftp
    char host[128];    // 域名：www.example.com
    int port;          // 端口：默认80/443
    char path[256];    // 路径：/path/to/file
    char query[256];   // 查询参数：name=test&age=18
} URL;

// 初始化URL结构体
void init_url(URL *url) {
    memset(url->scheme, 0, sizeof(url->scheme));
    memset(url->host, 0, sizeof(url->host));
    url->port = -1;
    memset(url->path, 0, sizeof(url->path));
    memset(url->query, 0, sizeof(url->query));
}

// 解析URL核心函数
int parse_url(const char *url_str, URL *url) {
    if (url_str == NULL || url == NULL) return -1;
    init_url(url);

    // 1. 提取协议（scheme）：到://为止
    const char *scheme_end = strstr(url_str, "://");
    if (scheme_end == NULL) return -1; // 无协议，非法URL
    int scheme_len = scheme_end - url_str;
    if (scheme_len >= sizeof(url->scheme)) return -1;
    strncpy(url->scheme, url_str, scheme_len);

    // 2. 跳过://，定位到域名起始
    const char *host_start = scheme_end + 3;
    const char *host_end = host_start;

    // 找域名结束位置（/ 或 ? 或 字符串结束）
    while (*host_end != '\0' && *host_end != '/' && *host_end != '?') {
        host_end++;
    }

    // 3. 拆分域名和端口
    char host_port[128];
    int host_port_len = host_end - host_start;
    if (host_port_len >= sizeof(host_port)) return -1;
    strncpy(host_port, host_start, host_port_len);
    
    // 按:分割域名和端口
    char *port_str = strchr(host_port, ':');
    if (port_str != NULL) {
        *port_str = '\0';
        strncpy(url->host, host_port, sizeof(url->host)-1);
        url->port = atoi(port_str + 1);
    } else {
        strncpy(url->host, host_port, sizeof(url->host)-1);
        // 设置默认端口
        if (strcmp(url->scheme, "http") == 0) {
            url->port = 80;
        } else if (strcmp(url->scheme, "https") == 0) {
            url->port = 443;
        }
    }

    // 4. 提取路径（path）：到?为止
    const char *path_start = host_end;
    const char *path_end = path_start;
    if (*path_start == '/') {
        while (*path_end != '\0' && *path_end != '?') {
            path_end++;
        }
        int path_len = path_end - path_start;
        if (path_len < sizeof(url->path)) {
            strncpy(url->path, path_start, path_len);
        }
    }

    // 5. 提取查询参数（query）
    const char *query_start = strchr(url_str, '?');
    if (query_start != NULL) {
        query_start++; // 跳过?
        strncpy(url->query, query_start, sizeof(url->query)-1);
    }

    return 0;
}

// 打印URL解析结果
void print_url(URL *url) {
    printf("Scheme: %s\n", url->scheme);
    printf("Host: %s\n", url->host);
    printf("Port: %d\n", url->port);
    printf("Path: %s\n", url->path);
    printf("Query: %s\n", url->query);
}

// 主函数：测试解析
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        fprintf(stderr, "Example: %s \"https://www.example.com:8080/path?name=test&age=18\"\n", argv[0]);
        return 1;
    }

    URL url;
    if (parse_url(argv[1], &url) != 0) {
        fprintf(stderr, "Parse URL failed\n");
        return 1;
    }

    print_url(&url);
    return 0;
}
