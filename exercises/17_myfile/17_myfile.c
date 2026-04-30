#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  // 补全ELF类型的解析逻辑
  switch (e_type) {
    case ET_NONE:    type_str = "No file type"; break;
    case ET_REL:     type_str = "Relocatable file (object file)"; break;
    case ET_EXEC:    type_str = "Executable file"; break;
    case ET_DYN:     type_str = "Shared object file"; break;
    case ET_CORE:    type_str = "Core file"; break;
    default:         type_str = "Unknown type"; break;
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
  ssize_t read_bytes;

  // 遍历两个ELF文件进行解析
  for (int i = 0; i < 2; i++) {
    printf("===== Analyzing file: %s =====\n", filepath[i]);
    
    // 打开文件（只读模式）
    fd = open(filepath[i], O_RDONLY);
    if (fd == -1) {
      perror("Failed to open file");
      continue;
    }

    // 读取ELF头部数据
    read_bytes = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (read_bytes != sizeof(Elf64_Ehdr)) {
      perror("Failed to read ELF header");
      close(fd);
      continue;
    }

    // 验证ELF魔数（确保是合法的ELF文件）
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "Not a valid ELF file\n");
      close(fd);
      continue;
    }

    // 打印ELF类型
    print_elf_type(ehdr.e_type);

    // 关闭文件
    close(fd);
    printf("\n");
  }

  return 0;
}
