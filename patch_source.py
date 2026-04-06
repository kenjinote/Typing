with open('Source.cpp', 'r', encoding='utf-8-sig') as f:
    text = f.read()

import re

# 1. Include
if '#include "words_data.h"' not in text:
    text = text.replace('#include "sqlite3.h"', '#include "sqlite3.h"\n#include "words_data.h"')

# 2. Add CreateWordsDatabaseFile
new_func = """
void CreateWordsDatabaseFile(HWND hWnd)
{
	LPCWSTR lpszDataBaseFilePath = GetWordsDataBaseFilePath();
	if (!PathFileExists(lpszDataBaseFilePath))
	{
		sqlite3* db = nullptr;
		if (sqlite3_open16(lpszDataBaseFilePath, &db) == SQLITE_OK)
		{
			sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS item(id INTEGER PRIMARY KEY AUTOINCREMENT, category INTEGER, words TEXT, kana TEXT);", NULL, NULL, NULL);
			
			// Empty string or 0 records handling
			sqlite3_stmt* stmt = nullptr;
			sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM item", -1, &stmt, NULL);
			int count = 0;
			if (sqlite3_step(stmt) == SQLITE_ROW) {
				count = sqlite3_column_int(stmt, 0);
			}
			sqlite3_finalize(stmt);
			
			if (count == 0) {
				sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
				
				sqlite3_stmt* ins_stmt = nullptr;
				sqlite3_prepare_v2(db, "INSERT INTO item(category, words, kana) VALUES (?, ?, ?)", -1, &ins_stmt, NULL);
				for (int i = 0; i < INITIAL_WORDS_COUNT; i++) {
					sqlite3_bind_int(ins_stmt, 1, 1);
					sqlite3_bind_text16(ins_stmt, 2, INITIAL_WORDS[i].words, -1, SQLITE_STATIC);
					sqlite3_bind_text16(ins_stmt, 3, INITIAL_WORDS[i].kana, -1, SQLITE_STATIC);
					sqlite3_step(ins_stmt);
					sqlite3_reset(ins_stmt);
				}
				sqlite3_finalize(ins_stmt);
				
				sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
			}
			sqlite3_close(db);
		}
	}
}
"""

if 'CreateWordsDatabaseFile' not in text:
    text = text.replace('void CreateRankingDatabaseFile(HWND hWnd)\n{', new_func + '\nvoid CreateRankingDatabaseFile(HWND hWnd)\n{')

# 3. WM_CREATE
if 'CreateWordsDatabaseFile(hWnd);' not in text:
    text = text.replace('CreateRankingDatabaseFile(hWnd);', 'CreateRankingDatabaseFile(hWnd);\n\t\t\tCreateWordsDatabaseFile(hWnd);')

with open('Source.cpp', 'w', encoding='utf-8-sig') as f:
    f.write(text)
print("Done")
