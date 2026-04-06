import re

with open('Source.cpp', 'r', encoding='utf-8-sig') as f:
    text = f.read()

# Replace Imports
text = re.sub(r'#import \"C:\\\\Program Files.*?dao360.dll\".*?\n', '', text)
text = re.sub(r'#import \"C:\\\\Program Files.*?msado60.tlb\".*?\n', '', text)
text = re.sub(r'#include <odbcinst.h>\n', '#include \"sqlite3.h\"\n', text)

# Replace DB File Extensions
text = text.replace('L\"words.mdb\"', 'L\"words.db\"')
text = text.replace('L\"keys.mdb\"', 'L\"keys.db\"')
text = text.replace('L\"ranking.mdb\"', 'L\"ranking.db\"')

# SQLExecute replacement
sql_execute_repl = '''int sqlite3_exec_w(sqlite3* db, LPCWSTR sql, int (*callback)(void*,int,char**,char**), void* arg, char** errmsg)
{
\tint len = WideCharToMultiByte(CP_UTF8, 0, sql, -1, NULL, 0, NULL, NULL);
\tif (len == 0) return SQLITE_ERROR;
\tchar* utf8 = new char[len];
\tWideCharToMultiByte(CP_UTF8, 0, sql, -1, utf8, len, NULL, NULL);
\tint rc = sqlite3_exec(db, utf8, callback, arg, errmsg);
\tdelete[] utf8;
\treturn rc;
}

sqlite3_stmt* sqlite3_prepare_w(sqlite3* db, LPCWSTR sql)
{
\tint len = WideCharToMultiByte(CP_UTF8, 0, sql, -1, NULL, 0, NULL, NULL);
\tif (len == 0) return nullptr;
\tchar* utf8 = new char[len];
\tWideCharToMultiByte(CP_UTF8, 0, sql, -1, utf8, len, NULL, NULL);
\tsqlite3_stmt* stmt = nullptr;
\tsqlite3_prepare_v2(db, utf8, -1, &stmt, nullptr);
\tdelete[] utf8;
\treturn stmt;
}

BOOL SQLExecute(HWND hWnd, LPCWSTR lpszDBFilePath, LPCWSTR lpszSQL)
{
\tsqlite3* db = nullptr;
\tif (sqlite3_open16(lpszDBFilePath, &db) == SQLITE_OK)
\t{
\t\tchar* errmsg = nullptr;
\t\tint rc = sqlite3_exec_w(db, lpszSQL, NULL, NULL, &errmsg);
\t\tif (rc != SQLITE_OK && errmsg)
\t\t{
\t\t\tWCHAR szErr[1024];
\t\t\tMultiByteToWideChar(CP_UTF8, 0, errmsg, -1, szErr, 1024);
\t\t\tMessageBox(hWnd, szErr, 0, 0);
\t\t\tsqlite3_free(errmsg);
\t\t}
\t\tsqlite3_close(db);
\t\treturn rc == SQLITE_OK;
\t}
\treturn FALSE;
}'''

text = re.sub(r'BOOL SQLExecute\(HWND hWnd\, LPCWSTR lpszMDBFilePath\, LPCWSTR lpszSQL\).*?(?=\nvoid CreateKeysDatabaseFile)', sql_execute_repl, text, flags=re.DOTALL)

# CreateKeysDatabaseFile
create_keys_repl = '''void CreateKeysDatabaseFile(HWND hWnd)
{
\tLPCWSTR lpszDataBaseFilePath = GetKeysDataBaseFilePath();
\tif (!PathFileExists(lpszDataBaseFilePath))
\t{
\t\tif (!SQLExecute(hWnd, lpszDataBaseFilePath, L\"CREATE TABLE item(rome TEXT NOT NULL PRIMARY KEY, kana TEXT NOT NULL, priority REAL NOT NULL);\"))
\t\t{
\t\t\tDeleteFile(lpszDataBaseFilePath);
\t\t\treturn;
\t\t}

\t\tsqlite3* db = nullptr;
\t\tif (sqlite3_open16(lpszDataBaseFilePath, &db) == SQLITE_OK)
\t\t{
\t\t\tsqlite3_exec(db, \"BEGIN TRANSACTION;\", NULL, NULL, NULL);
\t\t\tfor (auto i : g_rome)
\t\t\t{
\t\t\t\tWCHAR szSQL[1024];
\t\t\t\tswprintf_s(szSQL, L\"INSERT INTO item(rome, kana, priority) VALUES ('%s', '%s', %lf);\", i.first.c_str(), i.second.kana1.c_str(), (double)i.second.priority);
\t\t\t\tsqlite3_exec_w(db, szSQL, NULL, NULL, NULL);
\t\t\t}
\t\t\tsqlite3_exec(db, \"COMMIT;\", NULL, NULL, NULL);
\t\t\tsqlite3_close(db);
\t\t}
\t}
}'''
text = re.sub(r'void CreateKeysDatabaseFile\(HWND hWnd\).*?(?=\nvoid InsertRanking)', create_keys_repl, text, flags=re.DOTALL)

# InsertRanking
insert_ranking_repl = '''void InsertRanking(HWND hWnd, DWORD category, LPSYSTEMTIME date, double score, double typecount, double misscount)
{
\tLPCWSTR lpszDataBaseFilePath = GetRankingDataBaseFilePath();
\tif (PathFileExists(lpszDataBaseFilePath))
\t{
\t\tWCHAR szSQL[1024];
\t\tdouble date1;
\t\tSystemTimeToVariantTime(date, &date1);
\t\tswprintf_s(szSQL, L\"INSERT INTO item(category, date1, score1, typecount1, misscount1) VALUES (%d, %lf, %lf, %lf, %lf);\",
\t\t\tcategory, date1, score, typecount, misscount);
\t\tSQLExecute(hWnd, lpszDataBaseFilePath, szSQL);
\t}
}'''
text = re.sub(r'void InsertRanking\(HWND hWnd.*?SQLExecute\(hWnd\, lpszDataBaseFilePath\, szSQL\);\n\t\}\n\}', insert_ranking_repl, text, flags=re.DOTALL)

# CreateRankingDatabaseFile
create_ranking_repl = '''void CreateRankingDatabaseFile(HWND hWnd)
{
\tLPCWSTR lpszDataBaseFilePath = GetRankingDataBaseFilePath();
\tif (!PathFileExists(lpszDataBaseFilePath))
\t{
\t\tif (!SQLExecute(hWnd, lpszDataBaseFilePath, L\"CREATE TABLE item(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, category INTEGER NOT NULL, date1 REAL NOT NULL, score1 REAL NOT NULL, typecount1 REAL NOT NULL, misscount1 REAL NOT NULL);\"))
\t\t{
\t\t\tDeleteFile(lpszDataBaseFilePath);
\t\t\treturn;
\t\t}
\t}
}'''
text = re.sub(r'void CreateRankingDatabaseFile\(HWND hWnd\).*?(?=\nBOOL IsAlphabet)', create_ranking_repl, text, flags=re.DOTALL)

# LoadWordsFromDatabase
load_words_repl = '''BOOL game::LoadWordsFromDatabase()
{
\tBOOL bRet = FALSE;
\tsqlite3* db = nullptr;
\tif (sqlite3_open16(GetWordsDataBaseFilePath(), &db) == SQLITE_OK)
\t{
\t\tWCHAR szSQL[256];
\t\twsprintf(szSQL, L\"SELECT * FROM item WHERE category = %d ORDER BY RANDOM() LIMIT %d;\", nCategory, nMaxQuestionCount);
\t\tsqlite3_stmt* stmt = sqlite3_prepare_w(db, szSQL);
\t\tif (stmt)
\t\t{
\t\t\twhile (sqlite3_step(stmt) == SQLITE_ROW)
\t\t\t{
\t\t\t\tsentence data;
\t\t\t\tconst WCHAR* words = (const WCHAR*)sqlite3_column_text16(stmt, 2);
\t\t\t\tif (words) data.words = words;
\t\t\t\t
\t\t\t\tconst WCHAR* kana = (const WCHAR*)sqlite3_column_text16(stmt, 3);
\t\t\t\tif (kana) data.kana = kana;

\t\t\t\tlist.push_back(data);
\t\t\t\tbRet = TRUE;
\t\t\t}
\t\t\tsqlite3_finalize(stmt);
\t\t}
\t\tsqlite3_close(db);
\t}
\treturn bRet;
}'''

text = re.sub(r'BOOL game::LoadWordsFromDatabase\(\).*?(?=\nBOOL GetRankingFromDatabase)', load_words_repl, text, flags=re.DOTALL)

# GetRankingFromDatabase
get_ranking_repl = '''BOOL GetRankingFromDatabase(std::wstring& ranking, int nCategory)
{
\tranking = L\"[ランキング]\\r\\n\";
\tBOOL bRet = FALSE;
\tsqlite3* db = nullptr;
\tif (sqlite3_open16(GetRankingDataBaseFilePath(), &db) == SQLITE_OK)
\t{
\t\tWCHAR szSQL[256];
\t\tint id = 0;
\t\twsprintf(szSQL, L\"SELECT * FROM item WHERE category = %d ORDER BY date1 DESC LIMIT 1;\", nCategory);
\t\tsqlite3_stmt* stmt = sqlite3_prepare_w(db, szSQL);
\t\tif (stmt)
\t\t{
\t\t\tif (sqlite3_step(stmt) == SQLITE_ROW)
\t\t\t{
\t\t\t\tid = sqlite3_column_int(stmt, 0);
\t\t\t}
\t\t\tsqlite3_finalize(stmt);
\t\t}

\t\tif (id > 0)
\t\t{
\t\t\twsprintf(szSQL, L\"SELECT * FROM item WHERE category = %d ORDER BY score1 DESC LIMIT 10;\", nCategory);
\t\t\tstmt = sqlite3_prepare_w(db, szSQL);
\t\t\tif (stmt)
\t\t\t{
\t\t\t\tint i = 0;
\t\t\t\twhile (sqlite3_step(stmt) == SQLITE_ROW)
\t\t\t\t{
\t\t\t\t\tconst int id_ = sqlite3_column_int(stmt, 0);
\t\t\t\t\tdouble date = sqlite3_column_double(stmt, 2);
\t\t\t\t\tdouble score = sqlite3_column_double(stmt, 3);
\t\t\t\t\tdouble typecount = sqlite3_column_double(stmt, 4);
\t\t\t\t\tdouble misscount = sqlite3_column_double(stmt, 5);
\t\t\t\t\t
\t\t\t\t\tWCHAR szText[1024];
\t\t\t\t\tSYSTEMTIME st;
\t\t\t\t\tVariantTimeToSystemTime(date, &st);
\t\t\t\t\twsprintf(szText, L\"%d位: %d (%04d/%02d/%02d %02d:%02d:%02d)\", i + 1, (int)score, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
\t\t\t\t\tif (id == id_) {
\t\t\t\t\t\tlstrcat(szText, L\" ☚ 直近\");
\t\t\t\t\t}
\t\t\t\t\tlstrcat(szText, L\"\\r\\n\");
\t\t\t\t\tranking += szText;
\t\t\t\t\ti++;
\t\t\t\t}
\t\t\t\tbRet = TRUE;
\t\t\t\tsqlite3_finalize(stmt);
\t\t\t}
\t\t}
\t\tsqlite3_close(db);
\t}
\tranking += L\"\\r\\n[SPACE]キーでタイトルへ\";
\treturn bRet;
}'''

text = re.sub(r'BOOL GetRankingFromDatabase\(std::wstring& ranking\, int nCategory\).*?(?=\nBOOL GetScaling)', get_ranking_repl, text, flags=re.DOTALL)

# g_kana population
g_kana_repl = '''\t{
\t\tg_kana.clear();

\t\tsqlite3* db = nullptr;
\t\tif (sqlite3_open16(GetKeysDataBaseFilePath(), &db) == SQLITE_OK)
\t\t{
\t\t\tsqlite3_stmt* stmt = sqlite3_prepare_w(db, L\"SELECT * FROM item ORDER BY priority;\");
\t\t\tif (stmt)
\t\t\t{
\t\t\t\twhile (sqlite3_step(stmt) == SQLITE_ROW)
\t\t\t\t{
\t\t\t\t\tconst WCHAR* rome = (const WCHAR*)sqlite3_column_text16(stmt, 0);
\t\t\t\t\tconst WCHAR* kana = (const WCHAR*)sqlite3_column_text16(stmt, 1);
\t\t\t\t\tif (rome && kana)
\t\t\t\t\t{
\t\t\t\t\t\tg_kana[kana] = rome;
\t\t\t\t\t}
\t\t\t\t}
\t\t\t\tsqlite3_finalize(stmt);
\t\t\t}
\t\t\tsqlite3_close(db);
\t\t}
\t}'''

text = re.sub(r'\t\{\n\t\tg_kana\.clear\(\);\n\n\t\t_ConnectionPtr pCon.*?pCon = NULL;\n\t\t\}\n\t\}', g_kana_repl, text, flags=re.DOTALL)

with open('Source.cpp', 'w', encoding='utf-8-sig') as f:
    f.write(text)

print('Done')
