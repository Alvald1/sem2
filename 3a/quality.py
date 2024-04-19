import subprocess
import json
import sys, os

def write_to_file(file_path, text):
    output = f'/home/farguss/.dev/bin/multimetric --maintindex=microsoft {text} > res'
    try:
        with open(file_path, 'w') as file:
            file.write(output)
    except IOError:
        print("Ошибка при записи в файл.")
        

def run_bash_script(script_path):
    try:
        subprocess.run(["bash", script_path], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Ошибка: {e}")


def read_json_from_file(file_path):
    try:
        with open(file_path, 'r') as file:
            data = json.load(file)
        return data
    except FileNotFoundError:
        print("Файл не найден.")
    except json.JSONDecodeError:
        print("Ошибка при декодировании JSON.")


if __name__ == "__main__":
    arr=['cyclomatic_complexity','halstead_difficulty','loc','maintainability_index','operands_uniq','operators_uniq']
    script_path = "multimetric.sh"  # Укажите путь к вашему bash-скрипту
    text = " ".join(sys.argv[1:])  # Объединяем все параметры после пути к файлу в одну строку
    write_to_file(script_path, text)
    run_bash_script(script_path)
    file_path = "quality_res"  # Укажите путь к вашему текстовому файлу с JSON
    json_data = read_json_from_file(file_path)
    if json_data:
        for file_name, file_data in json_data['files'].items():            
            print(os.path.basename(file_name))
            for key, value in file_data.items():
                if key in arr:
                    print(f'{key} - {value}')
            print()
        
