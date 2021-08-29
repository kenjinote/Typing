﻿#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dsound")
#pragma comment(lib, "legacy_stdio_definitions")

#import "C:\Program Files (x86)\Common Files\Microsoft Shared\DAO\dao360.dll" rename_namespace("DAO") rename("EOF", "adoEOF")
#import "C:\Program Files (x86)\Common Files\System\ado\msado60.tlb" no_namespace rename("EOF", "EndOfFile")

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <odbcinst.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dsound.h>
#include <wincodec.h>
#include <map>
#include <vector>
#include <string>
#include "resource.h"

#define MAX_SOUND_COUNT 2
#define DEFAULT_DPI 96
#define SCALEX(X) MulDiv(X, uDpiX, DEFAULT_DPI)
#define SCALEY(Y) MulDiv(Y, uDpiY, DEFAULT_DPI)
#define POINT2PIXEL(PT) MulDiv(PT, uDpiY, DEFAULT_DPI)
#define MAX_LOADSTRING 100

#ifdef _DEBUG
#   define debuglog( str, ... ) \
      { \
        WCHAR c[1024]; \
        wsprintf(c, str, __VA_ARGS__); \
        OutputDebugString(c); \
      }
#else
#    define debuglog( str, ... ) // 空実装
#endif

// グローバル変数:
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
IDirectSoundBuffer* dsb1[MAX_SOUND_COUNT];
int nSoundResourceID[MAX_SOUND_COUNT] = { IDR_WAVE1,IDR_WAVE2 };

struct kana
{
	kana(LPCWSTR s, DATE d) :kana1(s), priority(d) {};
	std::wstring kana1;
	DATE priority;
};

std::map<std::wstring, kana> rome{
	{L"A",{L"ア",0.0}},
{L"BA",{L"バ",0.0}},
{L"BBA",{L"ッバ",0.0}},
{L"BBE",{L"ッベ",0.0}},
{L"BBI",{L"ッビ",0.0}},
{L"BBO",{L"ッボ",0.0}},
{L"BBU",{L"ッブ",0.0}},
{L"BBYA",{L"ッビャ",0.0}},
{L"BBYE",{L"ッビェ",0.0}},
{L"BBYI",{L"ッビィ",0.0}},
{L"BBYO",{L"ッビョ",0.0}},
{L"BBYU",{L"ッビュ",0.0}},
{L"BE",{L"ベ",0.0}},
{L"BI",{L"ビ",0.0}},
{L"BO",{L"ボ",0.0}},
{L"BU",{L"ブ",0.0}},
{L"BYA",{L"ビャ",0.0}},
{L"BYE",{L"ビェ",0.0}},
{L"BYI",{L"ビィ",0.0}},
{L"BYO",{L"ビョ",0.0}},
{L"BYU",{L"ビュ",0.0}},
{L"CA",{L"カ",0.0}},
{L"CE",{L"セ",0.0}},
{L"CHA",{L"チャ",0.0}},
{L"CHE",{L"チェ",0.0}},
{L"CHI",{L"チ",0.0}},
{L"CHO",{L"チョ",0.0}},
{L"CHU",{L"チュ",0.0}},
{L"CI",{L"シ",0.0}},
{L"CO",{L"コ",0.0}},
{L"CU",{L"ク",0.0}},
{L"DA",{L"ダ",0.0}},
{L"DDA",{L"ッダ",0.0}},
{L"DDE",{L"ッデ",0.0}},
{L"DDI",{L"ッヂ",0.0}},
{L"DDO",{L"ッド",0.0}},
{L"DDU",{L"ッヅ",0.0}},
{L"DE",{L"デ",0.0}},
{L"DHA",{L"デャ",0.0}},
{L"DHE",{L"デェ",0.0}},
{L"DHI",{L"ディ",0.0}},
{L"DHO",{L"デョ",0.0}},
{L"DHU",{L"デュ",0.0}},
{L"DI",{L"ディ",0.0}},
{L"DJI",{L"ヂ",0.0}},
{L"DO",{L"ド",0.0}},
{L"DU",{L"ヅ",0.0}},
{L"DWA",{L"ドァ",0.0}},
{L"DWE",{L"ドェ",0.0}},
{L"DWI",{L"ドィ",0.0}},
{L"DWO",{L"ドォ",0.0}},
{L"DWU",{L"ドゥ",0.0}},
{L"DYA",{L"ヂャ",0.0}},
{L"DYE",{L"ヂェ",0.0}},
{L"DYI",{L"ヂィ",0.0}},
{L"DYO",{L"ヂョ",0.0}},
{L"DYU",{L"ヂュ",0.0}},
{L"DZU",{L"ヅ",0.0}},
{L"E",{L"エ",0.0}},
{L"FA",{L"ファ",0.0}},
{L"FE",{L"フェ",0.0}},
{L"FFA",{L"ッファ",0.0}},
{L"FFE",{L"ッフェ",0.0}},
{L"FFI",{L"ッフィ",0.0}},
{L"FFO",{L"ッフォ",0.0}},
{L"FFU",{L"ッフ",0.0}},
{L"FI",{L"フィ",0.0}},
{L"FO",{L"フォ",0.0}},
{L"FU",{L"フ",0.0}},
{L"FWA",{L"ファ",0.0}},
{L"FWE",{L"フェ",0.0}},
{L"FWI",{L"フィ",0.0}},
{L"FWO",{L"フォ",0.0}},
{L"FWU",{L"フゥ",0.0}},
{L"FYA",{L"フャ",0.0}},
{L"FYE",{L"フェ",0.0}},
{L"FYI",{L"フィ",0.0}},
{L"FYO",{L"フョ",0.0}},
{L"FYU",{L"フュ",0.0}},
{L"GA",{L"ガ",0.0}},
{L"GE",{L"ゲ",0.0}},
{L"GGA",{L"ッガ",0.0}},
{L"GGE",{L"ッゲ",0.0}},
{L"GGI",{L"ッギ",0.0}},
{L"GGO",{L"ッゴ",0.0}},
{L"GGU",{L"ッグ",0.0}},
{L"GI",{L"ギ",0.0}},
{L"GO",{L"ゴ",0.0}},
{L"GU",{L"グ",0.0}},
{L"GWA",{L"グァ",0.0}},
{L"GWE",{L"グェ",0.0}},
{L"GWI",{L"グィ",0.0}},
{L"GWO",{L"グォ",0.0}},
{L"GWU",{L"グゥ",0.0}},
{L"GYA",{L"ギャ",0.0}},
{L"GYE",{L"ギェ",0.0}},
{L"GYI",{L"ギィ",0.0}},
{L"GYO",{L"ギョ",0.0}},
{L"GYU",{L"ギュ",0.0}},
{L"HA",{L"ハ",0.0}},
{L"HE",{L"ヘ",0.0}},
{L"HHA",{L"ッハ",0.0}},
{L"HHE",{L"ッヘ",0.0}},
{L"HHI",{L"ッヒ",0.0}},
{L"HHO",{L"ッポ",0.0}},
{L"HHU",{L"ッフ",0.0}},
{L"HI",{L"ヒ",0.0}},
{L"HO",{L"ホ",0.0}},
{L"HU",{L"フ",0.0}},
{L"HYA",{L"ヒャ",0.0}},
{L"HYO",{L"ヒョ",0.0}},
{L"HYU",{L"ヒュ",0.0}},
{L"I",{L"イ",0.0}},
{L"JA",{L"ジャ",0.0}},
{L"JE",{L"ジェ",0.0}},
{L"JI",{L"ジ",0.0}},
{L"JI",{L"ヂ",0.0}},
{L"JJA",{L"ッジャ",0.0}},
{L"JJE",{L"ッジェ",0.0}},
{L"JJI",{L"ッジ",0.0}},
{L"JJO",{L"ッジォ",0.0}},
{L"JJU",{L"ッジュ",0.0}},
{L"JJYA",{L"ッジャ",0.0}},
{L"JJYE",{L"ッジェ",0.0}},
{L"JJYI",{L"ッジィ",0.0}},
{L"JJYO",{L"ッジョ",0.0}},
{L"JJYU",{L"ッジュ",0.0}},
{L"JO",{L"ジョ",0.0}},
{L"JU",{L"ジュ",0.0}},
{L"JYA",{L"ジャ",0.0}},
{L"JYE",{L"ジェ",0.0}},
{L"JYI",{L"ジィ",0.0}},
{L"JYO",{L"ジョ",0.0}},
{L"JYU",{L"ジュ",0.0}},
{L"KA",{L"カ",0.0}},
{L"KE",{L"ケ",0.0}},
{L"KI",{L"キ",0.0}},
{L"KKA",{L"ッカ",0.0}},
{L"KKE",{L"ッケ",0.0}},
{L"KKI",{L"ッキ",0.0}},
{L"KKO",{L"ッコ",0.0}},
{L"KKU",{L"ック",0.0}},
{L"KO",{L"コ",0.0}},
{L"KU",{L"ク",0.0}},
{L"KYA",{L"キャ",0.0}},
{L"KYO",{L"キョ",0.0}},
{L"KYU",{L"キュ",0.0}},
{L"LA",{L"ァ",0.0}},
{L"LE",{L"ェ",0.0}},
{L"LI",{L"ィ",0.0}},
{L"LLA",{L"ッァ",0.0}},
{L"LLE",{L"ッェ",0.0}},
{L"LLI",{L"ッィ",0.0}},
{L"LLO",{L"ッォ",0.0}},
{L"LLU",{L"ッゥ",0.0}},
{L"LO",{L"ォ",0.0}},
{L"LTU",{L"ッ",0.0}},
{L"LU",{L"ゥ",0.0}},
{L"LYA",{L"ャ",0.0}},
{L"LYE",{L"ェ",0.0}},
{L"LYI",{L"ィ",0.0}},
{L"LYO",{L"ョ",0.0}},
{L"LYU",{L"ュ",0.0}},
{L"MA",{L"マ",0.0}},
{L"ME",{L"メ",0.0}},
{L"MI",{L"ミ",0.0}},
{L"MMA",{L"ッマ",0.0}},
{L"MME",{L"ッメ",0.0}},
{L"MMI",{L"ッミ",0.0}},
{L"MMO",{L"ッモ",0.0}},
{L"MMU",{L"ッム",0.0}},
{L"MO",{L"モ",0.0}},
{L"MU",{L"ム",0.0}},
{L"MYA",{L"ミャ",0.0}},
{L"MYO",{L"ミョ",0.0}},
{L"MYU",{L"ミュ",0.0}},
{L"NA",{L"ナ",0.0}},
{L"NE",{L"ネ",0.0}},
{L"NI",{L"ニ",0.0}},
{L"NN",{L"ン",0.0}},
{L"NO",{L"ノ",0.0}},
{L"NU",{L"ヌ",0.0}},
{L"NYA",{L"ニャ",0.0}},
{L"NYO",{L"ニョ",0.0}},
{L"NYU",{L"ニュ",0.0}},
{L"O",{L"オ",0.0}},
{L"PA",{L"パ",0.0}},
{L"PE",{L"ペ",0.0}},
{L"PI",{L"ピ",0.0}},
{L"PO",{L"ポ",0.0}},
{L"PPA",{L"ッバ",0.0}},
{L"PPE",{L"ッベ",0.0}},
{L"PPI",{L"ッピ",0.0}},
{L"PPO",{L"ッポ",0.0}},
{L"PPU",{L"ップ",0.0}},
{L"PU",{L"プ",0.0}},
{L"PYA",{L"ピャ",0.0}},
{L"PYE",{L"ピェ",0.0}},
{L"PYI",{L"ピィ",0.0}},
{L"PYO",{L"ピョ",0.0}},
{L"PYU",{L"ピュ",0.0}},
{L"QA",{L"クァ",0.0}},
{L"QE",{L"クェ",0.0}},
{L"QI",{L"クィ",0.0}},
{L"QO",{L"クォ",0.0}},
{L"QQA",{L"ックァ",0.0}},
{L"QQE",{L"ックェ",0.0}},
{L"QQI",{L"ックィ",0.0}},
{L"QQO",{L"ックォ",0.0}},
{L"QQU",{L"ック",0.0}},
{L"QQYA",{L"ックャ",0.0}},
{L"QQYE",{L"ックェ",0.0}},
{L"QQYI",{L"ックィ",0.0}},
{L"QQYO",{L"ックョ",0.0}},
{L"QQYU",{L"ックュ",0.0}},
{L"QU",{L"クゥ",0.0}},
{L"QWA",{L"クァ",0.0}},
{L"QWE",{L"クェ",0.0}},
{L"QWI",{L"クィ",0.0}},
{L"QWO",{L"クォ",0.0}},
{L"QWU",{L"クゥ",0.0}},
{L"QYA",{L"クャ",0.0}},
{L"QYE",{L"クェ",0.0}},
{L"QYI",{L"クィ",0.0}},
{L"QYO",{L"クョ",0.0}},
{L"QYU",{L"クュ",0.0}},
{L"RA",{L"ラ",0.0}},
{L"RE",{L"レ",0.0}},
{L"RI",{L"リ",0.0}},
{L"RO",{L"ロ",0.0}},
{L"RRA",{L"ッラ",0.0}},
{L"RRE",{L"ッレ",0.0}},
{L"RRI",{L"ッリ",0.0}},
{L"RRO",{L"ッロ",0.0}},
{L"RRU",{L"ッル",0.0}},
{L"RU",{L"ル",0.0}},
{L"RYA",{L"リャ",0.0}},
{L"RYO",{L"リョ",0.0}},
{L"RYU",{L"リュ",0.0}},
{L"SA",{L"サ",0.0}},
{L"SE",{L"セ",0.0}},
{L"SHA",{L"シャ",0.0}},
{L"SHE",{L"シェ",0.0}},
{L"SHI",{L"シ",0.0}},
{L"SHO",{L"ショ",0.0}},
{L"SHU",{L"シュ",0.0}},
{L"SI",{L"シ",0.0}},
{L"SO",{L"ソ",0.0}},
{L"SSA",{L"ッサ",0.0}},
{L"SSE",{L"ッセ",0.0}},
{L"SSI",{L"ッシ",0.0}},
{L"SSO",{L"ッソ",0.0}},
{L"SSU",{L"ッス",0.0}},
{L"SU",{L"ス",0.0}},
{L"SWA",{L"スァ",0.0}},
{L"SWE",{L"スェ",0.0}},
{L"SWI",{L"スィ",0.0}},
{L"SWO",{L"スォ",0.0}},
{L"SWU",{L"スゥ",0.0}},
{L"SYA",{L"シャ",0.0}},
{L"SYE",{L"シェ",0.0}},
{L"SYI",{L"シィ",0.0}},
{L"SYO",{L"ショ",0.0}},
{L"SYU",{L"シュ",0.0}},
{L"TA",{L"タ",0.0}},
{L"TE",{L"テ",0.0}},
{L"THA",{L"テャ",0.0}},
{L"THE",{L"テェ",0.0}},
{L"THI",{L"ティ",0.0}},
{L"THO",{L"テョ",0.0}},
{L"THU",{L"テュ",0.0}},
{L"TI",{L"チ",0.0}},
{L"TO",{L"ト",0.0}},
{L"TSA",{L"ツァ",0.0}},
{L"TSE",{L"ツェ",0.0}},
{L"TSI",{L"ツィ",0.0}},
{L"TSO",{L"ツォ",0.0}},
{L"TSU",{L"ツ",0.0}},
{L"TTA",{L"ッタ",0.0}},
{L"TTE",{L"ッテ",0.0}},
{L"TTI",{L"ッチ",0.0}},
{L"TTO",{L"ット",0.0}},
{L"TTU",{L"ッツ",0.0}},
{L"TU",{L"ツ",0.0}},
{L"TWA",{L"トァ",0.0}},
{L"TWE",{L"トェ",0.0}},
{L"TWI",{L"トィ",0.0}},
{L"TWO",{L"トォ",0.0}},
{L"TWU",{L"トゥ",0.0}},
{L"TYA",{L"チャ",0.0}},
{L"TYE",{L"チェ",0.0}},
{L"TYI",{L"チィ",0.0}},
{L"TYO",{L"チョ",0.0}},
{L"TYU",{L"チュ",0.0}},
{L"U",{L"ウ",0.0}},
{L"VU",{L"ヴ",0.0}},
{L"WA",{L"ワ",0.0}},
{L"WE",{L"ウェ",0.0}},
{L"WHA",{L"ウァ",0.0}},
{L"WHE",{L"ウェ",0.0}},
{L"WHI",{L"ウィ",0.0}},
{L"WHO",{L"ウォ",0.0}},
{L"WHU",{L"ウ",0.0}},
{L"WI",{L"ウィ",0.0}},
{L"WO",{L"ヲ",0.0}},
{L"WWA",{L"ッワ",0.0}},
{L"WWE",{L"ッウェ",0.0}},
{L"WWI",{L"ッウィ",0.0}},
{L"WWO",{L"ッヲ",0.0}},
{L"WWU",{L"ッウ",0.0}},
{L"XA",{L"ァ",0.0}},
{L"XE",{L"ェ",0.0}},
{L"XI",{L"ィ",0.0}},
{L"XN",{L"ン",0.0}},
{L"XO",{L"ォ",0.0}},
{L"XTU",{L"ッ",0.0}},
{L"XU",{L"ゥ",0.0}},
{L"XXA",{L"ッァ",0.0}},
{L"XXE",{L"ッェ",0.0}},
{L"XXI",{L"ッィ",0.0}},
{L"XXO",{L"ッォ",0.0}},
{L"XXU",{L"ッゥ",0.0}},
{L"XXYA",{L"ッャ",0.0}},
{L"XXYE",{L"ッェ",0.0}},
{L"XXYI",{L"ッィ",0.0}},
{L"XXYO",{L"ッョ",0.0}},
{L"XXYU",{L"ッュ",0.0}},
{L"XYA",{L"ャ",0.0}},
{L"XYE",{L"ェ",0.0}},
{L"XYI",{L"ィ",0.0}},
{L"XYO",{L"ョ",0.0}},
{L"XYU",{L"ュ",0.0}},
{L"YA",{L"ヤ",0.0}},
{L"YE",{L"イェ",0.0}},
{L"YO",{L"ヨ",0.0}},
{L"YU",{L"ユ",0.0}},
{L"YYA",{L"ッヤ",0.0}},
{L"YYE",{L"ッイェ",0.0}},
{L"YYI",{L"ッイ",0.0}},
{L"YYO",{L"ッヨ",0.0}},
{L"YYU",{L"ッユ",0.0}},
{L"ZA",{L"ザ",0.0}},
{L"ZE",{L"ゼ",0.0}},
{L"ZO",{L"ゾ",0.0}},
{L"ZU",{L"ズ",0.0}},
{L"ZYA",{L"ジャ",0.0}},
{L"ZYE",{L"ジェ",0.0}},
{L"ZYI",{L"ジィ",0.0}},
{L"ZYO",{L"ジョ",0.0}},
{L"ZYU",{L"ジュ",0.0}},
{L"ZZA",{L"ッザ",0.0}},
{L"ZZE",{L"ッゼ",0.0}},
{L"ZZI",{L"ッジ",0.0}},
{L"ZZO",{L"ッゾ",0.0}},
{L"ZZU",{L"ッズ",0.0}},
{L"ZZYA",{L"ッジャ",0.0}},
{L"ZZYE",{L"ッジェ",0.0}},
{L"ZZYI",{L"ッジィ",0.0}},
{L"ZZYO",{L"ッジョ",0.0}},
{L"ZZYU",{L"ッジュ",0.0}},
};

std::map<std::wstring, std::wstring> words{
	{L"この度、弊社では過去に資料請求をしていただいた方に対し、メールマガジンでフォローをさせていただくことになりました。",L"コノタビ,ヘイシャデハカコニシリョウセイキュウヲシテイタダイタカタニタイシ,メールマガジンデフォローサセテイタダクコトニナリマシタ."},
	{L"最新の研修動向、メールの上手な書き方、ビジネスメール実態調査の最新データなど、プロとしての切り口をお楽しみいただけたらと思います。",L"サイシンノケンシュウドウコウ,メールノジョウズナカキカタ,ビジネスメールノジッタイチョウサノサイシンデータナド,プロトシテノキリクチヲオタノシミイタダケタラトオモイマス."},
	{L"配信は、毎週月曜日の11時頃を予定しております。",L"ハイシンハ,マイシュウゲツヨウビノジュウイチジゴロヲヨテイシテオリマス."},
	{L"引き続きよろしくお願いいたします。",L"ヒキツヅキヨロシクオネガイイタシマス."},
	{L"大変お手数ですが、不要な方は以下のURLから解除をお願い致します。",L"タイヘンオテスウデスガ,フヨウナカタハカキノURLカラカイジョヲオネガイイタシマス."},
	{L"引き続きよろしくお願いいたします。",L"ヒキツヅキヨロシクオネガイイタシマス."},
	{L"本日、お電話で以下の講座について、ご予約のキャンセルを承りました。",L"ホンジツ,オデンワデカキノコウザニツイテ,ゴヨヤクノキャンセルヲウケタマワリマシタ."},
	{L"お振り込みはまだのため、返金等はございません。",L"オフリコミハマダノタメ,ヘンキントウハゴザイマセン."},
	{L"また機会がございましたら、お申し込みいただけますと幸いです。",L"マタノキカイガゴザイマシタラ,オモウシコミイタダケマストサイワイデス."},
	{L"ご不明な点やご質問がございましたら、お気軽にご連絡ください。",L"ゴフメイナテンヤゴシツモンガゴザイマシタラ,オキガルニゴレンラククダサイ."},
	{L"それでは、今後ともよろしくお願いいたします。",L"ソレデハ,コンゴトモよろしくオネガイイタシマス."},
	{L"現在、為替変動などの諸事情により、原材料の価格が高騰しております。",L"ゲンザイ,カワセヘンドウナドノショジジョウニヨリ,ゲンザイジョウノカカクガコウトウシテオリマス."},
	{L"これまでも弊社はスタッフ一丸となり、諸経費の削減やコストを下げる努力を行なってまいりましたが、今後、経営を圧迫しかねない状況に陥っております。",L"コレマデモヘイシャスタッフイチガントナリ,ショケイヒノサクゲンヤコストヲサゲルドリョクヲオコナッテマイリマシタガ,コンゴ,ケイヒヲアッパクシカネナイジョウキョウニオチイッテオリマス."},
	{L"誠に遺憾ではありますが、検討を重ねた結果、来月から以下3商品の価格を改定させていただくことになりました。",L"マコトニイカンデハアリマスガ,ケントウヲカサネタケッカ,ライゲツカライカノ3ショウヒンノカカクヲカイテイサセテイタダクコトニナリマシタ."},
	{L"なにとぞご理解のほどお願いいたします。",L"ナニトゾゴリカイノホドオネガイイタシマス."},
	{L"このたびは、弊社の営業時間変更のご連絡でメールをいたしました。",L"コノタビハ,ヘイシャノエイギョウジカンヘンコウノゴレンラクデメールヲイタシマシタ."},
	{L"以下の店頭窓口の営業時間を次の通り変更いたしますので、ご確認のほどよろしくお願いいたします。",L"イカノテントウマドグチノエイギョウジカンヲツギノトオリヘンコウイタシマスノデ,ゴカクニンノホドヨロシクオネガイイタシマス."},
	{L"今後ともスタッフ一同さらに丁寧なサービスを心がけて参ります。",L"コンゴトモスタッフイチドウサラニテイネイナサービスヲココロガケテマイリマス."},
	{L"このたび長期のお休みをいただくことになりましたので、事前にご連絡をさせていただきました。",L"コノタビハチョウキノオヤスミヲイタダクコトニナリマシタノデ,ジゼンニゴレンラクヲサセテイタダキマシタ."},
	{L"私の不在期間中は、弊社の田中が担当させていただきますので、何かありましたら以下までご連絡ください。",L"ワタシノフザイキカンチュウハ,ヘイシャノタナカガタントウサセテイタダキマスノデ,ナニカアリマシタラカキマデゴレンラククダサイ."},
	{L"ご連絡の際は、私をCCに入れていただけますと幸いです。",L"ゴレンラクノサイハ,ワタシヲCCニイレテイタダケマストサイワイデス."},
	{L"厳正に抽選させていただいた結果、ご当選されましたのでお知らせいたします。",L"ゲンセンニチュウセンサセテイタダイタケッカ,ゴトウセンサレマシタノデオシラセイタシマス."},
	{L"セミナーの日程は以下のとおりです。",L"セミナーのニッテイハイカノトオリデス."},
	{L"当日、鈴木様にお会いできるのを楽しみにしております。",L"トウジツ,スズキサマニオアイデキルノヲタノシミニシテオリマス."},
	{L"なお、万一ご欠席になる場合は、事前にご一報いただければ幸いです。",L"ナオ,マンガイチゴケッセキニナルバアイハ,ジゼンニゴイッポウイタダケレバサイワイデス."},
	{L"厳正に抽選させていただいた結果、誠に残念ですが落選となりました。",L"ゲンセイニチュウセンサセテイタダイタケッカ,マコトニザンネンデスガラクセントナリマシタ."},
	{L"興味を持ってお申込みをいただきましたのに、お断りせざるを得ない状況になり、本当に申し訳ございません。",L"キョウミヲモッテオモウシコミイタダキマシタノニ,オコトワリセザルヲエナイジョウキョウニナリ,ホントウニモウシワケゴザイマセン."},
	{L"また同様のイベントを開催させていただく際には、改めてご案内させていただきます。",L"マタドウヨウノイベントヲカイサイサセテイタダクサイニハ,アラタメテゴアンナイサセテイタダキマス。"},
	{L"本年の夏季休業は以下の日程となっておりますので、お知らせいたします。",L"ホンネンノカキキュウギョウハイカノニッテイトナッテオリマスノデ,オシラセイタシマス。"},
	{L"休業期間中はご不便をおかけいたしますが、ご了承くださいますようお願い申し上げます。",L"キュウギョウキカンチュウハゴフベンヲオカケイタシマスガ,ゴリョウショウクダサイマスヨウオネガイモウシアゲマス."},
	{L"計画停電実施に伴い弊社でも業務体制の変更をいたします。",L"ケイカクテイデンジッシニトモナイヘイシャデモギョウムタイセイノヘンコウヲイタシマス."},
};

struct sentence
{
	std::wstring words;
	std::wstring kana;
};

typedef enum {
	GS_TITLE,
	GS_COUNTDOWN,
	GS_GAMESTART,
	GS_GAMEEND,
	GS_GAMEOVER,
	GS_RANKINGVIW,
	GS_NAMEINPUT,
	GS_SETTINGS,
} GAME_STATE;

struct game
{
	game()
	{
		nCategory = 1;
		nTypeCount = 0;
		nMissCount = 0;
		nQuestionCount = 0;
		nMaxQuestionCount = 10;
		list.clear();
	}

	BOOL LoadWordsFromDatabase();
	LPWSTR GetRomeFromKana(LPCWSTR);

	void start()
	{
		nCursor = 0;
		nTypeCount = 0;
		nMissCount = 0;
		nQuestionCount = 0;
		nMaxQuestionCount = 10;
		LoadWordsFromDatabase();

		lstrcpy(szQuestionKana, list[nQuestionCount].kana.c_str());
		lstrcpy(szQuestionRome, GetRomeFromKana(szQuestionKana));

		GetLocalTime(&stGameStartTime);

		nGameState = GS_GAMESTART;
	}
	void next()
	{
		if (nQuestionCount + 1 < nMaxQuestionCount)
		{
			nQuestionCount++;

			lstrcpy(szQuestionKana, list[nQuestionCount].kana.c_str());
			lstrcpy(szQuestionRome, GetRomeFromKana(szQuestionKana));
			
			szInputRome[0] = 0;
			nCursor = 0;
		}
		else
		{
			GetLocalTime(&stGameEndTime);

			nGameState = GS_GAMEEND;
		}
	}
	std::vector<sentence> list;
	GAME_STATE nGameState;
	int nTypeCount;
	int nMissCount;
	int nQuestionCount;
	int nMaxQuestionCount;
	SYSTEMTIME stGameStartTime;
	SYSTEMTIME stGameEndTime;
	int nCursor; // 現在の入力位置
	WCHAR szInputRome[5]; // 現在入力されたローマ字
	int nCategory;
	WCHAR szQuestionKana[256]; // カタカナを配列で用意しておく
	WCHAR szQuestionRome[1024]; // ローマ字を配列で用意しておく
};

template<class Interface> inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

void PlaySound(int nSoundKind)
{
	if (0 <= nSoundKind && nSoundKind < MAX_SOUND_COUNT)
	{
		dsb1[nSoundKind]->SetCurrentPosition(0);
		dsb1[nSoundKind]->Play(0, 0, 0);
	}
}

void GetAppFolderPath(OUT LPWSTR lpszFolderPath)
{
	if (SHGetSpecialFolderPath(hWnd, lpszFolderPath, CSIDL_LOCAL_APPDATA, 0))
	{
		PathAppend(lpszFolderPath, szWindowClass);
		if (!PathFileExists(lpszFolderPath))
		{
			CreateDirectory(lpszFolderPath, NULL);
		}
	}
}

LPCWSTR GetWordsDataBaseFilePath()
{
	static WCHAR szFilePath[MAX_PATH] = {};
	if (szFilePath[0] == L'\0')
	{
		GetAppFolderPath(szFilePath);
		PathAppend(szFilePath, L"words.mdb");
	}
	return szFilePath;
}

LPCWSTR GetKeysDataBaseFilePath()
{
	static WCHAR szFilePath[MAX_PATH] = {};
	if (szFilePath[0] == L'\0')
	{
		GetAppFolderPath(szFilePath);
		PathAppend(szFilePath, L"keys.mdb");
	}
	return szFilePath;
}

BOOL SQLExecute(HWND hWnd, LPCWSTR lpszMDBFilePath, LPCWSTR lpszSQL)
{
	BOOL bRet = FALSE;
	HRESULT hr;
	_ConnectionPtr pCon(NULL);
	hr = pCon.CreateInstance(__uuidof(Connection));
	if (SUCCEEDED(hr))
	{
		TCHAR szString[1024];
		wsprintf(szString, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", lpszMDBFilePath);
		hr = pCon->Open(szString, _bstr_t(""), _bstr_t(""), adOpenUnspecified);
		if (SUCCEEDED(hr))
		{
			bRet = TRUE;
			try
			{
				_CommandPtr pCommand(NULL);
				pCommand.CreateInstance(__uuidof(Command));
				pCommand->ActiveConnection = pCon;
				pCommand->CommandText = lpszSQL;
				pCommand->Execute(NULL, NULL, adCmdText);
			}
			catch (_com_error& e)
			{
				MessageBox(hWnd, e.Description(), 0, 0);
				bRet = FALSE;
			}
			pCon->Close();
		}
		pCon = NULL;
	}
	return bRet;
}

void CreateWordsDatabaseFile(HWND hWnd)
{
	// データベースファイルがない場合は作成。
	LPCWSTR lpszDataBaseFilePath = GetWordsDataBaseFilePath();
	if (!PathFileExists(lpszDataBaseFilePath))
	{
		BOOL bCreateDBError = FALSE;
		WCHAR szAttributes[1024];
		wsprintf(szAttributes, L"CREATE_DB=\"%s\" General\0", lpszDataBaseFilePath);
		if (!SQLConfigDataSource(hWnd, ODBC_ADD_DSN, L"Microsoft Access Driver (*.mdb)", szAttributes))
		{
			bCreateDBError = TRUE;
		}

		if (bCreateDBError)
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}

		// テーブル作成
		if (!SQLExecute(hWnd, lpszDataBaseFilePath, L"CREATE TABLE item(id COUNTER NOT NULL PRIMARY KEY, category LONG NOT NULL, words VARCHAR NOT NULL, kana VARCHAR NOT NULL);"))
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}

		for (auto i : words)
		{
			WCHAR szSQL[1024];
			swprintf_s(szSQL, L"INSERT INTO item(category, words, kana) VALUES (%d, '%s', '%s');", 1, i.first.c_str(), i.second.c_str());
			if (!SQLExecute(hWnd, lpszDataBaseFilePath, szSQL))
			{
				DeleteFile(lpszDataBaseFilePath);
				break;
			}
		}
	}
}

void CreateKeysDatabaseFile(HWND hWnd)
{
	// データベースファイルがない場合は作成。
	LPCWSTR lpszDataBaseFilePath = GetKeysDataBaseFilePath();
	if (!PathFileExists(lpszDataBaseFilePath))
	{
		BOOL bCreateDBError = FALSE;
		WCHAR szAttributes[1024];
		wsprintf(szAttributes, L"CREATE_DB=\"%s\" General\0", lpszDataBaseFilePath);
		if (!SQLConfigDataSource(hWnd, ODBC_ADD_DSN, L"Microsoft Access Driver (*.mdb)", szAttributes))
		{
			bCreateDBError = TRUE;
		}

		if (bCreateDBError)
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}

		// テーブル作成
		if (!SQLExecute(hWnd, lpszDataBaseFilePath, L"CREATE TABLE item(rome VARCHAR NOT NULL PRIMARY KEY, kana VARCHAR NOT NULL, priority DATETIME NOT NULL);"))
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}

		for (auto i : rome)
		{
			WCHAR szSQL[1024];
			swprintf_s(szSQL, L"INSERT INTO item(rome, kana, priority) VALUES ('%s', '%s', %lf);", i.first.c_str(), i.second.kana1.c_str(), i.second.priority);
			if (!SQLExecute(hWnd, lpszDataBaseFilePath, szSQL))
			{
				DeleteFile(lpszDataBaseFilePath);
				break;
			}
		}
	}
}


LPWSTR game::GetRomeFromKana(LPCWSTR lpszKana)
{
	static WCHAR szRome[1024];
	LPCWSTR p = lpszKana;
	szRome[0] = 0;
	while (*p != L'\0')
	{
		switch (*p)
		{
		case L'０':
			lstrcat(szRome, L"0");
			p++;
			continue;
		case L'１':
			lstrcat(szRome, L"1");
			p++;
			continue;
		case L'２':
			lstrcat(szRome, L"2");
			p++;
			continue;
		case L'３':
			lstrcat(szRome, L"3");
			p++;
			continue;
		case L'４':
			lstrcat(szRome, L"4");
			p++;
			continue;
		case L'５':
			lstrcat(szRome, L"5");
			p++;
			continue;
		case L'６':
			lstrcat(szRome, L"6");
			p++;
			continue;
		case L'７':
			lstrcat(szRome, L"7");
			p++;
			continue;
		case L'８':
			lstrcat(szRome, L"8");
			p++;
			continue;
		case L'９':
			lstrcat(szRome, L"9");
			p++;
			continue;
			break;
		case L'ー':
			lstrcat(szRome, L"-");
			p++;
			continue;
		case L'、':
			lstrcat(szRome, L",");
			p++;
			continue;
		case L'。':
			lstrcat(szRome, L".");
			p++;
			continue;
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
		case L',':
		case L'-':
		case L'+':
		case L'?':
		case L'/':
		case L':':
		case L';':
		case L'`':
		case L'~':
		case L'.':
			{
				WCHAR szKigo[2] = { *p, 0 };
				lstrcat(szRome, szKigo);
				p++;
				continue;
			}
			break;
		}

		{
			int nStep = 0;
			BOOL bRet = FALSE;
			_ConnectionPtr pCon(NULL);
			HRESULT hr = pCon.CreateInstance(__uuidof(Connection));
			if (SUCCEEDED(hr))
			{
				TCHAR szString[1024];
				wsprintf(szString, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", GetKeysDataBaseFilePath());
				hr = pCon->Open(szString, _bstr_t(""), _bstr_t(""), adOpenUnspecified);
				if (SUCCEEDED(hr))
				{
					try
					{
						_RecordsetPtr pRecordset(NULL);
						hr = pRecordset.CreateInstance(__uuidof(Recordset));
						if (SUCCEEDED(hr))
						{
							_CommandPtr pCommand(NULL);
							pCommand.CreateInstance(__uuidof(Command));
							pCommand->ActiveConnection = pCon;

							// 3文字探す
							if (*(p + 1) != 0 && *(p + 2) != 0)
							{
								WCHAR szKana[4] = { *p, *(p + 1), *(p + 2), 0 };
								WCHAR szSQL[256];
								wsprintf(szSQL, L"select top %d * from item where kana = '%s' order by priority DESC;", 1, szKana);
								pCommand->CommandText = szSQL;
								pRecordset = pCommand->Execute(NULL, NULL, adCmdText);
								if (!pRecordset->EndOfFile)
								{
									// 先頭のレコードへ移動
									pRecordset->MoveFirst();
									_variant_t rome = pRecordset->Fields->GetItem((long)0)->Value;
									lstrcat(szRome, rome.bstrVal);
									nStep = 3;
									bRet = TRUE;
								}
								pRecordset->Close();
							}

							// 2文字探す
							if (!bRet && *(p + 1) != 0)
							{
								WCHAR szKana[3] = { *p, *(p + 1), 0 };
								WCHAR szSQL[256];
								wsprintf(szSQL, L"select top %d * from item where kana = '%s' order by priority DESC;", 1, szKana);
								pCommand->CommandText = szSQL;
								pRecordset = pCommand->Execute(NULL, NULL, adCmdText);
								if (!pRecordset->EndOfFile)
								{
									// 先頭のレコードへ移動
									pRecordset->MoveFirst();
									_variant_t rome = pRecordset->Fields->GetItem((long)0)->Value;
									lstrcat(szRome, rome.bstrVal);
									nStep = 2;
									bRet = TRUE;
								}
								pRecordset->Close();
							}

							// 1文字探す
							if (!bRet)
							{
								WCHAR szKana[2] = { *p, 0 };
								WCHAR szSQL[256];
								wsprintf(szSQL, L"select top %d * from item where kana = '%s' order by priority DESC;", 1, szKana);
								pCommand->CommandText = szSQL;
								pRecordset = pCommand->Execute(NULL, NULL, adCmdText);
								if (!pRecordset->EndOfFile)
								{
									// 先頭のレコードへ移動
									pRecordset->MoveFirst();
									_variant_t rome = pRecordset->Fields->GetItem((long)0)->Value;
									lstrcat(szRome, rome.bstrVal);
									nStep = 1;
									bRet = TRUE;
								}
								pRecordset->Close();
							}
						}
						pRecordset = NULL;
					}
					catch (_com_error& e)
					{
						MessageBox(hWnd, e.Description(), 0, 0);
						bRet = FALSE;
					}
					pCon->Close();
				}
				pCon = NULL;
			}
			if (bRet)
			{
				p += nStep;
				continue;
			}
		}
	}
	return szRome;
}

BOOL game::LoadWordsFromDatabase()
{
	srand((unsigned)GetTickCount());

	list.clear();

	BOOL bRet = FALSE;
	_ConnectionPtr pCon(NULL);
	HRESULT hr = pCon.CreateInstance(__uuidof(Connection));
	if (SUCCEEDED(hr))
	{
		WCHAR szString[1024];
		wsprintf(szString, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", GetWordsDataBaseFilePath());
		hr = pCon->Open(szString, _bstr_t(""), _bstr_t(""), adOpenUnspecified);
		if (SUCCEEDED(hr))
		{
			try
			{
				_RecordsetPtr pRecordset(NULL);
				hr = pRecordset.CreateInstance(__uuidof(Recordset));
				if (SUCCEEDED(hr))
				{
					_CommandPtr pCommand(NULL);
					pCommand.CreateInstance(__uuidof(Command));
					pCommand->ActiveConnection = pCon;
					WCHAR szSQL[256];
					wsprintf(szSQL, L"select top %d * from item where category = %d order by Rnd([id]);", nMaxQuestionCount, nCategory);
					pCommand->CommandText = szSQL;
					pRecordset = pCommand->Execute(NULL, NULL, adCmdText);
					if (!pRecordset->EndOfFile)
					{
						try
						{
							// 先頭のレコードへ移動
							pRecordset->MoveFirst();
							while (!pRecordset->EndOfFile)
							{
								sentence data;

								_variant_t words = pRecordset->Fields->GetItem((long)2)->Value;
								data.words = std::wstring(words.bstrVal, SysStringLen(words.bstrVal));

								_variant_t kana = pRecordset->Fields->GetItem((long)3)->Value;
								data.kana = std::wstring(kana.bstrVal, SysStringLen(kana.bstrVal));

								list.push_back(data);

								pRecordset->MoveNext();
							}
							bRet = TRUE;
						}
						catch (_com_error& e)
						{
							OutputDebugString(e.Description());
							bRet = FALSE;
						}
					}
					pRecordset->Close();
				}
				pRecordset = NULL;
			}
			catch (_com_error& e)
			{
				OutputDebugString(e.Description());
				bRet = FALSE;
			}
			pCon->Close();
		}
		pCon = NULL;
	}
	return bRet;
}

BOOL GetScaling(HWND hWnd, UINT* pnX, UINT* pnY)
{
	BOOL bSetScaling = FALSE;
	const HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	if (hMonitor)
	{
		HMODULE hShcore = LoadLibrary(L"shcore");
		if (hShcore)
		{
			typedef HRESULT __stdcall GetDpiForMonitor(HMONITOR, int, UINT*, UINT*);
			GetDpiForMonitor* fnGetDpiForMonitor = reinterpret_cast<GetDpiForMonitor*>(GetProcAddress(hShcore, "GetDpiForMonitor"));
			if (fnGetDpiForMonitor)
			{
				UINT uDpiX, uDpiY;
				if (SUCCEEDED(fnGetDpiForMonitor(hMonitor, 0, &uDpiX, &uDpiY)) && uDpiX > 0 && uDpiY > 0)
				{
					*pnX = uDpiX;
					*pnY = uDpiY;
					bSetScaling = TRUE;
				}
			}
			FreeLibrary(hShcore);
		}
	}
	if (!bSetScaling)
	{
		HDC hdc = GetDC(NULL);
		if (hdc)
		{
			*pnX = GetDeviceCaps(hdc, LOGPIXELSX);
			*pnY = GetDeviceCaps(hdc, LOGPIXELSY);
			ReleaseDC(NULL, hdc);
			bSetScaling = TRUE;
		}
	}
	if (!bSetScaling)
	{
		*pnX = DEFAULT_DPI;
		*pnY = DEFAULT_DPI;
		bSetScaling = TRUE;
	}
	return bSetScaling;
}

BOOL JudgeKigou(DWORD dwKeyCode, LPCWSTR lpszKana, int nCursor)
{
	switch (lpszKana[nCursor])
	{
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
		if (dwKeyCode == lpszKana[nCursor]) return TRUE;
		break;
	case L'。':
	case L'．':
	case L'.':
		if (dwKeyCode == VK_OEM_PERIOD) return TRUE;
		break;
	case L',':
	case L'、':
		if (dwKeyCode == VK_OEM_COMMA) return TRUE;
		break;
	case L'-':
	case L'ー':
		if (dwKeyCode == VK_OEM_MINUS) return TRUE;
		break;
	case L'+':
		if (dwKeyCode == VK_OEM_PLUS) return TRUE;
		break;
	case L'?':
		if (dwKeyCode == VK_OEM_2) return TRUE;
		break;
	case L'/':
		if (dwKeyCode == VK_OEM_2) return TRUE;
		break;
	case L':':
		if (dwKeyCode == VK_OEM_1) return TRUE;
		break;
	case L';':
		if (dwKeyCode == VK_OEM_1) return TRUE;
		break;
	case L'`':
		if (dwKeyCode == VK_OEM_3) return TRUE;
		break;
	case L'~':
		if (dwKeyCode == VK_OEM_3) return TRUE;
		break;		
	}
	return FALSE;
}

BOOL JudgeKana(LPCWSTR lpszInputAlphabet, LPCWSTR lpszKana, int nCursor, LPWSTR lpszOutputKana)
{
	std::map<std::wstring, kana>::const_iterator i = rome.lower_bound(lpszInputAlphabet);
	while (i != rome.end())
	{
		const std::wstring& rome = i->first;
		if (rome.compare(0, lstrlenW(lpszInputAlphabet), lpszInputAlphabet) != 0)
		{
			return FALSE;
		}

		const std::wstring& kana = i->second.kana1;
		if (StrCmpN(lpszKana + nCursor, kana.c_str(), kana.length()) == 0)
		{
			lstrcpy(lpszOutputKana, kana.c_str());
			return TRUE;
		}

		i++;
	}
	return FALSE;
}

struct question
{
	std::wstring words;
	std::vector<std::wstring> rome;
	std::vector<int> time;
};

BOOL IsKigo(WPARAM wParam)
{
	switch (wParam)
	{
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
	case VK_OEM_PERIOD:
	case VK_OEM_COMMA:
	case VK_OEM_MINUS:
	case VK_OEM_PLUS:
	case VK_OEM_1:
	case VK_OEM_2:
	case VK_OEM_3:
		return TRUE;
	}
	return FALSE;
}

BOOL IsKigo(WCHAR c)
{
	switch (c)
	{
	case L'０':
	case L'１':
	case L'２':
	case L'３':
	case L'４':
	case L'５':
	case L'６':
	case L'７':
	case L'８':
	case L'９':
	case L'ー':
	case L'、':
	case L'。':
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
	case L',':
	case L'-':
	case L'+':
	case L'?':
	case L'/':
	case L':':
	case L';':
	case L'`':
	case L'~':
	case L'.':
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static ID2D1Factory* m_pD2DFactory;
	static IWICImagingFactory* m_pWICFactory;
	static IDWriteFactory* m_pDWriteFactory;
	static ID2D1HwndRenderTarget* m_pRenderTarget;
	static IDWriteTextFormat* m_pTextFormat;
	static ID2D1SolidColorBrush* m_pBlackBrush;
	static ID2D1SolidColorBrush* m_pGrayBrush;

	static IDWriteTextLayout* m_pTextLayout;

	static HFONT hFont;
	static UINT uDpiX = DEFAULT_DPI, uDpiY = DEFAULT_DPI;
	
	static game g;

	switch (msg)
	{
	case WM_CREATE:
		{
			CoInitialize(NULL);

			CreateKeysDatabaseFile(hWnd);


			CreateWordsDatabaseFile(hWnd);
			static const WCHAR msc_fontName[] = L"Verdana";
			static const FLOAT msc_fontSize = 50;
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
			if (SUCCEEDED(hr))
				hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
			if (SUCCEEDED(hr))
				hr = m_pDWriteFactory->CreateTextFormat(msc_fontName, 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, msc_fontSize, L"", &m_pTextFormat);
			//if (SUCCEEDED(hr))
			//	hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			//if (SUCCEEDED(hr))
			//	hr = m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			if (FAILED(hr))
				return -1;
		}
		SendMessage(hWnd, WM_APP, 0, 0);

		// ゲーム開始
		// ランダムで問題を読み込む

		g.start();

		break;
	case WM_SIZE:
		if (m_pRenderTarget)
		{
			D2D1_SIZE_U size = { LOWORD(lParam), HIWORD(lParam) };
			m_pRenderTarget->Resize(size);
		}
		break;
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, 0, 0);
		break;
	case WM_PAINT:
		{
			HRESULT hr = S_OK;
			if (!m_pRenderTarget)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

				hr = m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(), DEFAULT_DPI, DEFAULT_DPI, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pGrayBrush);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();
				m_pRenderTarget->BeginDraw();
				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

				{// 問題文を表示
					LPCWSTR lpszText = g.list[g.nQuestionCount].words.c_str();
					DWRITE_TEXT_METRICS tTextMetrics;
					{
						IDWriteTextLayout* pTextLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(
							lpszText
							, lstrlenW(lpszText)
							, m_pTextFormat
							, renderTargetSize.width
							, renderTargetSize.height
							, &pTextLayout
						);
						pTextLayout->GetMetrics(&tTextMetrics);
						SafeRelease(&pTextLayout);
					}

					D2D1_RECT_F tTextRectF = D2D1::RectF(
						renderTargetSize.width / 2 - tTextMetrics.width / 2
						, renderTargetSize.height / 2 - tTextMetrics.height / 2 - 2 * tTextMetrics.height
						, renderTargetSize.width / 2 - tTextMetrics.width / 2 + tTextMetrics.width
						, renderTargetSize.height / 2 - tTextMetrics.height / 2 - 2 * tTextMetrics.height + tTextMetrics.height
					);

					hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
					m_pRenderTarget->DrawText(lpszText, lstrlen(lpszText), m_pTextFormat, tTextRectF, m_pBlackBrush);
				}

				{// カタカナを表示
					LPCWSTR lpszText = g.list[g.nQuestionCount].kana.c_str();
					DWRITE_TEXT_METRICS tTextMetrics;
					{
						IDWriteTextLayout* pTextLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(
							lpszText
							, lstrlenW(lpszText)
							, m_pTextFormat
							, renderTargetSize.width
							, renderTargetSize.height
							, &pTextLayout
						);
						pTextLayout->GetMetrics(&tTextMetrics);
						SafeRelease(&pTextLayout);
					}

					D2D1_RECT_F tTextRectF = D2D1::RectF(
						renderTargetSize.width / 2 - tTextMetrics.width / 2
						, renderTargetSize.height / 2 - tTextMetrics.height / 2
						, renderTargetSize.width / 2 - tTextMetrics.width / 2 + tTextMetrics.width
						, renderTargetSize.height / 2 - tTextMetrics.height / 2 + tTextMetrics.height
					);

					hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
					m_pRenderTarget->DrawText(lpszText, g.nCursor, m_pTextFormat, tTextRectF, m_pGrayBrush);

					hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
					m_pRenderTarget->DrawText(lpszText + g.nCursor, lstrlen(lpszText + g.nCursor), m_pTextFormat, tTextRectF, m_pBlackBrush);

				}

				{// ローマ字を表示
					DWRITE_TEXT_METRICS tTextMetrics;
					{
						IDWriteTextLayout* pTextLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(
							g.szQuestionRome
							, lstrlenW(g.szQuestionRome)
							, m_pTextFormat
							, renderTargetSize.width
							, renderTargetSize.height
							, &pTextLayout
						);
						pTextLayout->GetMetrics(&tTextMetrics);
						SafeRelease(&pTextLayout);
					}

					D2D1_RECT_F tTextRectF = D2D1::RectF(
						renderTargetSize.width / 2 - tTextMetrics.width / 2
						, renderTargetSize.height / 2 - tTextMetrics.height / 2 + tTextMetrics.height
						, renderTargetSize.width / 2 - tTextMetrics.width / 2 + tTextMetrics.width
						, renderTargetSize.height / 2 - tTextMetrics.height / 2 + tTextMetrics.height + tTextMetrics.height
					);

					hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
					m_pRenderTarget->DrawText(g.szQuestionRome, lstrlen(g.szQuestionRome), m_pTextFormat, tTextRectF, m_pBlackBrush);
				}



				hr = m_pRenderTarget->EndDraw();
				if (hr == D2DERR_RECREATE_TARGET)
				{
					hr = S_OK;
					SafeRelease(&m_pRenderTarget);
					SafeRelease(&m_pBlackBrush);
					SafeRelease(&m_pGrayBrush);
				}
			}
		}
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (g.nGameState == GAME_STATE::GS_GAMESTART)
		{
			if (IsKigo(wParam))
			{
				BOOL bRedraw = FALSE;
				if (g.szQuestionKana[g.nCursor] == L'ン' && lstrlen(g.szInputRome) == 1 && g.szInputRome[0] == L'N')
				{
					g.nCursor++;
					g.nTypeCount++;
					PlaySound(0);
					g.szInputRome[0] = g.szInputRome[1];
					g.szInputRome[1] = 0;
					bRedraw = TRUE;
				}
				if (JudgeKigou(wParam, g.szQuestionKana, g.nCursor))
				{
					g.nCursor++;
					g.nTypeCount++;
					PlaySound(0);
					if (g.szQuestionKana[g.nCursor] == L'\0')
					{
						g.next();
					}
					bRedraw = TRUE;
				}
				else
				{
					g.nMissCount++;
					PlaySound(1);
					g.szInputRome[0] = 0;
				}
				if (bRedraw)
				{
					InvalidateRect(hWnd, 0, 0);
				}
			}
			else if (L'A' <= wParam && wParam <= L'Z')
			{
				WCHAR szInput[2];
				wsprintf(szInput, L"%c", wParam);
				lstrcat(g.szInputRome, szInput);
				debuglog(L"szInputRome = %s\r\n", g.szInputRome);

				BOOL bRedraw = FALSE;

				if (g.szQuestionKana[g.nCursor] == L'ン' && lstrlen(g.szInputRome) == 2 && g.szInputRome[0] == L'N' && g.szInputRome[1] != L'Y')
				{
					// ンは2文字目に関係なく確定
					g.nCursor++;
					g.nTypeCount++;
					PlaySound(0);
					g.szInputRome[0] = g.szInputRome[1];
					g.szInputRome[1] = 0;
					debuglog(L"szInputRome(after) = %s\r\n", g.szInputRome);
					bRedraw = TRUE;
				}

				WCHAR szOutput[4] = {};
				BOOL bRet = JudgeKana(g.szInputRome, g.szQuestionKana, g.nCursor, szOutput);
				if (bRet)
				{
					debuglog(L"szOutput = %s\r\n", szOutput);
					if (L'A' == wParam || L'I' == wParam || L'U' == wParam || L'E' == wParam || L'O' == wParam || (g.szInputRome[0] == L'N' && g.szInputRome[1] == L'N'))
					{
						// 母音またはNを2回入力した場合は、ローマ字入力を確定させ次の文字に移る。
						g.nCursor += lstrlen(szOutput);
						g.nTypeCount += lstrlen(szOutput);
						PlaySound(0);

						g.szInputRome[0] = 0;
						if (g.szQuestionKana[g.nCursor] == L'\0')
						{
							g.next();
						}
						bRedraw = TRUE;
					}
				}
				else
				{
					g.nMissCount++;
					PlaySound(1);
					g.szInputRome[0] = 0;
				}
				if (bRedraw)
				{
					InvalidateRect(hWnd, 0, 0);
				}
			}
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
		}
		break;
	case WM_NCCREATE:
		{
			const HMODULE hModUser32 = GetModuleHandle(L"user32");
			if (hModUser32)
			{
				typedef BOOL(WINAPI* fnTypeEnableNCScaling)(HWND);
				const fnTypeEnableNCScaling fnEnableNCScaling = (fnTypeEnableNCScaling)GetProcAddress(hModUser32, "EnableNonClientDpiScaling");
				if (fnEnableNCScaling)
				{
					fnEnableNCScaling(hWnd);
				}
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_APP:
		GetScaling(hWnd, &uDpiX, &uDpiY);
		DeleteObject(hFont);
		hFont = CreateFontW(-POINT2PIXEL(10), 0, 0, 0, FW_NORMAL, 0, 0, 0, SHIFTJIS_CHARSET, 0, 0, 0, 0, L"MS Shell Dlg");
		break;
	case WM_DPICHANGED:
		SendMessage(hWnd, WM_APP, 0, 0);
		break;
	case WM_DESTROY:
		SafeRelease(&m_pD2DFactory);
		SafeRelease(&m_pDWriteFactory);
		SafeRelease(&m_pRenderTarget);
		SafeRelease(&m_pTextFormat);
		SafeRelease(&m_pBlackBrush);
		SafeRelease(&m_pGrayBrush);
		DeleteObject(hFont);
		CoUninitialize();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_CLASS_NAME, szWindowClass, MAX_LOADSTRING);

	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szWindowClass
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);

	DSBUFFERDESC dsbd = {};
	LPBYTE pMem1, pMem2;
	DWORD dwSize1, dwSize2;
	IDirectSound* ds[MAX_SOUND_COUNT];
	int nIndex = 0;
	for (int i = 0; i < MAX_SOUND_COUNT; i++)
	{
		DirectSoundCreate(NULL, &(ds[i]), NULL);
		(ds[i])->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		HRSRC hrsrc = FindResource(hInstance, MAKEINTRESOURCE(nSoundResourceID[i]), TEXT("WAVE"));
		HGLOBAL hRData = LoadResource(hInstance, hrsrc);
		DWORD* pRData = (DWORD*)LockResource(hRData);
		dsbd.dwBufferBytes = *(pRData + 10);
		dsbd.lpwfxFormat = (LPWAVEFORMATEX)(pRData + 5);
		(ds[i])->CreateSoundBuffer(&dsbd, &(dsb1[i]), NULL);
		(dsb1[i])->Lock(0, dsbd.dwBufferBytes, (void**)&pMem1, &dwSize1, (void**)&pMem2, &dwSize2, 0);
		memcpy(pMem1, (LPBYTE)(pRData + 11), dwSize1);
		if (dwSize2 != 0) memcpy(pMem2, (LPBYTE)(pRData + 11) + dwSize1, dwSize2);
		(dsb1[i])->Unlock(pMem1, dwSize1, pMem2, dwSize2);
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	for (auto i : ds)
	{
		i->Release();
	}
	return (int)msg.wParam;
}
