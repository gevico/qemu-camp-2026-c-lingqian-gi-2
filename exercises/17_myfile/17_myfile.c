#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_NONE:
        type_str = "Unknown";
        break;
    case ET_REL:
        type_str = "Relocatable";
        break;
    case ET_EXEC:
        type_str = "Executable";
        break;
    case ET_DYN:
        type_str = "Shared Object/PIE";
        break;
    default:
        type_str = "Unknown";
        break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  // 遍历两个ELF文件进行解析
  for (int i = 0; i < 2; i++) {
    // 打开文件
    fd = open(filepath[i], O_RDONLY);
    if (fd == -1) {
      perror("Failed to open file");
      continue;
    }

    // 读取ELF文件头部
    ssize_t read_size = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (read_size != sizeof(Elf64_Ehdr)) {
      perror("Failed to read ELF header");
      close(fd);
      continue;
    }

    // 验证是否为合法ELF文件（检查魔数）
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      printf("File %s is not a valid ELF file\n", filepath[i]);
      close(fd);
      continue;
    }

    // 打印文件名和对应的ELF类型
    printf("=== %s ===\n", filepath[i]);
    print_elf_type(ehdr.e_type);
    printf("\n");

    // 关闭文件
    close(fd);
  }
  
  return 0;
}
