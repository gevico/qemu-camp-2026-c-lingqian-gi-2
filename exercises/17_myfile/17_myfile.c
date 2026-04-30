#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_NONE:    // 无类型
        type_str = "Unknown";
        break;
    case ET_REL:     // 可重定位文件（.o）
        type_str = "Relocatable";
        break;
    case ET_EXEC:    // 可执行文件
        type_str = "Executable";
        break;
    case ET_DYN:     // 共享对象/PIE
        type_str = "Shared Object/PIE";
        break;
    default:         // 未知类型
        type_str = "Unknown";
        break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[3][256] = {
    "./17_myfile.o",    // 可重定位文件
    "./17_myfile",      // 可执行文件
    "/lib64/ld-linux-x86-64.so.2"  // 共享对象（补充测试）
  };

  int fd;
  Elf64_Ehdr ehdr;  // 64位ELF头部（兼容主流系统）

  // 遍历所有测试文件
  for (int i = 0; i < 3; i++) {
    printf("\n=== 检测文件: %s ===\n", filepath[i]);
    
    // 打开文件
    fd = open(filepath[i], O_RDONLY);
    if (fd < 0) {
      perror("open() 失败");
      continue;
    }

    // 读取ELF头部
    ssize_t read_size = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (read_size != sizeof(Elf64_Ehdr)) {
      perror("read() 失败（文件过小或非ELF）");
      close(fd);
      continue;
    }

    // 验证ELF魔数（确保是ELF文件）
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      printf("不是有效的ELF文件\n");
      close(fd);
      continue;
    }

    // 打印ELF类型
    print_elf_type(ehdr.e_type);

    // 关闭文件
    close(fd);
  }
  
  return 0;
}
