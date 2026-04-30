#include "mywc.h"

// 创建哈希表
WordCount **create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
    // 空指针/空单词校验
    if (hash_table == NULL || word == NULL || strlen(word) == 0) {
        return;
    }

    // 计算哈希索引
    unsigned int index = hash(word);
    WordCount *current = hash_table[index];

    // 查找链表中是否已存在该单词
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->count++; // 存在则计数+1
            return;
        }
        current = current->next;
    }

    // 不存在则创建新节点
    WordCount *new_node = malloc(sizeof(WordCount));
    if (new_node == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // 初始化新节点
    strncpy(new_node->word, word, MAX_WORD_LEN - 1);
    new_node->word[MAX_WORD_LEN - 1] = '\0'; // 确保字符串终止
    new_node->count = 1;
    new_node->next = hash_table[index]; // 插入到链表头部

    // 更新哈希表桶的头指针
    hash_table[index] = new_node;
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
  printf("Word Count Statistics:\n");
  printf("======================\n");

  if (hash_table == NULL) {
      return;
  }

  // 遍历哈希表所有桶
  for (int i = 0; i < HASH_SIZE; i++) {
      WordCount *current = hash_table[i];
      // 遍历链表并按格式打印
      while (current != NULL) {
          // %-20s 左对齐占20字符，匹配测试要求的格式（单词+若干空格+次数）
          printf("%-20s %d\n", current->word, current->count);
          current = current->next;
      }
  }
}

// 释放哈希表内存
void free_hash_table(WordCount **hash_table) {
    if (hash_table == NULL) {
        return;
    }

    // 遍历所有桶，释放链表节点
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *current = hash_table[i];
        while (current != NULL) {
            WordCount *temp = current;
            current = current->next;
            free(temp);
        }
        hash_table[i] = NULL;
    }

    // 释放哈希表本身
    free(hash_table);
}

// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  WordCount **hash_table = create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  free_hash_table(hash_table);
}
