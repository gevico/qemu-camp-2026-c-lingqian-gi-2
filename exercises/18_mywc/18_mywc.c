#include "mywc.h"

int main(int argc, char *argv[]) {
    // 默认处理 text.txt，也支持命令行参数指定文件
    const char *filename = "text.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    process_file(filename);
    return 0;
}
