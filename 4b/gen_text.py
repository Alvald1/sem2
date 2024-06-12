import random
import string

def generate_random_word(length=5):
    """Функция для генерации случайного слова из заданного количества символов."""
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for _ in range(length))

def generate_text_file(file_name, num_lines, words_per_line, vocab_size):
    # Генерация списка случайных слов
    word_list = [generate_random_word() for _ in range(vocab_size)]

    # Открытие файла для записи
    with open(file_name, 'w') as file:
        for _ in range(num_lines):
            # Генерация строки с заданным количеством слов
            line = ' '.join(random.choices(word_list, k=words_per_line))
            # Запись строки в файл
            file.write(line + '\n')

# Пример использования:
generate_text_file('text', 1000, 10, 1000)

