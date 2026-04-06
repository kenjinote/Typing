import os
import json

scripts = [
    'gen_data1.py', 'gen_data2.py', 'gen_data3.py', 'gen_data4.py', 'gen_data5.py',
    'gen_data6.py', 'gen_data7.py', 'gen_data8.py', 'gen_data9.py', 'gen_data10.py',
    'gen_data11.py', 'gen_data12.py', 'gen_data13.py', 'gen_data14.py', 'gen_data15.py',
    'gen_data16.py', 'gen_data17.py', 'gen_data18.py'
]

for s in scripts:
    os.system(f'python {s}')

all_data = []
for i in range(1, 19):
    file = f'data{i}.json'
    if os.path.exists(file):
        with open(file, 'r', encoding='utf-8') as f:
            all_data.extend(json.load(f))

# unique
unique_results = []
seen_words = set()

for w, k in all_data:
    if w not in seen_words:
        seen_words.add(w)
        unique_results.append((w, k))

with open('words_data.h', 'w', encoding='utf-8-sig') as f:
    f.write('#pragma once\n\n')
    f.write('#include <windows.h>\n\n')
    f.write('struct InitialWord {\n')
    f.write('    LPCWSTR words;\n')
    f.write('    LPCWSTR kana;\n')
    f.write('};\n\n')
    f.write(f'// Total generated items: {len(unique_results)}\n')
    f.write(f'const int INITIAL_WORDS_COUNT = {len(unique_results)};\n')
    f.write('const InitialWord INITIAL_WORDS[] = {\n')
    for w, k in unique_results:
        f.write(f'    {{L\"{w}\", L\"{k}\"}},\n')
    f.write('};\n')

print(f"Total entries: {len(unique_results)}")
