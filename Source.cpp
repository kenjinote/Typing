#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dsound")
#pragma comment(lib, "legacy_stdio_definitions")
#pragma comment(lib, "windowscodecs.lib")
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "sqlite3.h"
#include "words_data.h"
#include <d2d1.h>
#include <dwrite_3.h>
#include <dwrite.h>
#include <dsound.h>
#include <wincodec.h>
#include <map>
#include <vector>
#include <string>
#include "resource.h"
#define MAX_SOUND_COUNT 6
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
int nSoundResourceID[MAX_SOUND_COUNT] = { IDR_WAVE1,IDR_WAVE2,IDR_WAVE3,IDR_WAVE4,IDR_WAVE5,IDR_WAVE6 };
struct kana
{
	kana(LPCWSTR s, DATE d) :kana1(s), priority(d) {};
	std::wstring kana1;
	DATE priority;
};
std::map<std::wstring, kana> g_rome{
	{L"A",{L"ア",1.0}},
	{L"BA",{L"バ",0.0}},
	{L"BBA",{L"ッバ",1.0}},
	{L"BBE",{L"ッベ",1.0}},
	{L"BBI",{L"ッビ",1.0}},
	{L"BBO",{L"ッボ",1.0}},
	{L"BBU",{L"ッブ",1.0}},
	{L"BBYA",{L"ッビャ",1.0}},
	{L"BBYE",{L"ッビェ",1.0}},
	{L"BBYI",{L"ッビィ",1.0}},
	{L"BBYO",{L"ッビョ",1.0}},
	{L"BBYU",{L"ッビュ",1.0}},
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
	{L"CCA",{L"ッカ",0.0}},
	{L"CCE",{L"ッセ",0.0}},
	{L"CCHA",{L"ッチャ",0.0}},
	{L"CCHE",{L"ッチェ",0.0}},
	{L"CCHI",{L"ッチ",0.0}},
	{L"CCHO",{L"ッチョ",0.0}},
	{L"CCHU",{L"ッチュ",0.0}},
	{L"CCI",{L"ッシ",0.0}},
	{L"CCO",{L"ッコ",0.0}},
	{L"CCU",{L"ック",0.0}},
	{L"CCYA",{L"ッチャ",0.0}},
	{L"CCYE",{L"ッチェ",0.0}},
	{L"CCYI",{L"ッチィ",0.0}},
	{L"CCYO",{L"ッチョ",0.0}},
	{L"CCYU",{L"ッチュ",0.0}},
	{L"CE",{L"セ",0.0}},
	{L"CHA",{L"チャ",0.0}},
	{L"CHE",{L"チェ",0.0}},
	{L"CHI",{L"チ",0.0}},
	{L"CHO",{L"チョ",0.0}},
	{L"CHU",{L"チュ",0.0}},
	{L"CI",{L"シ",0.0}},
	{L"CO",{L"コ",0.0}},
	{L"CU",{L"ク",0.0}},
	{L"CYA",{L"チャ",0.0}},
	{L"CYE",{L"チェ",0.0}},
	{L"CYI",{L"チィ",0.0}},
	{L"CYO",{L"チョ",0.0}},
	{L"CYU",{L"チュ",0.0}},
	{L"DA",{L"ダ",0.0}},
	{L"DDA",{L"ッダ",1.0}},
	{L"DDE",{L"ッデ",1.0}},
	{L"DDHA",{L"ッデャ",0.0}},
	{L"DDHE",{L"ッデェ",0.0}},
	{L"DDHI",{L"ッディ",0.0}},
	{L"DDHO",{L"ッデョ",0.0}},
	{L"DDHU",{L"ッデュ",0.0}},
	{L"DDI",{L"ッヂ",1.0}},
	{L"DDO",{L"ッド",1.0}},
	{L"DDU",{L"ッヅ",1.0}},
	{L"DDWA",{L"ッドァ",0.0}},
	{L"DDWE",{L"ッドェ",0.0}},
	{L"DDWI",{L"ッドィ",0.0}},
	{L"DDWO",{L"ッドォ",0.0}},
	{L"DDWU",{L"ッドゥ",0.0}},
	{L"DE",{L"デ",0.0}},
	{L"DHA",{L"デャ",0.0}},
	{L"DHE",{L"デェ",0.0}},
	{L"DHI",{L"ディ",0.0}},
	{L"DHO",{L"デョ",0.0}},
	{L"DHU",{L"デュ",0.0}},
	{L"DI",{L"ディ",0.0}},
	{L"DJI",{L"ヂ",0.0}},
	{L"DO",{L"ド",0.0}},
	{L"DU",{L"ヅ",1.0}},
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
	{L"E",{L"エ",1.0}},
	{L"FA",{L"ファ",1.0}},
	{L"FE",{L"フェ",1.0}},
	{L"FFA",{L"ッファ",0.0}},
	{L"FFE",{L"ッフェ",0.0}},
	{L"FFI",{L"ッフィ",0.0}},
	{L"FFO",{L"ッフォ",0.0}},
	{L"FFU",{L"ッフ",0.0}},
	{L"FFWA",{L"ッファ",0.0}},
	{L"FFWE",{L"ッフェ",0.0}},
	{L"FFWI",{L"ッフィ",0.0}},
	{L"FFWO",{L"ッフォ",0.0}},
	{L"FFWU",{L"ッフゥ",0.0}},
	{L"FFYA",{L"ッフャ",0.0}},
	{L"FFYE",{L"ッフェ",0.0}},
	{L"FFYI",{L"ッフィ",0.0}},
	{L"FFYO",{L"ッフョ",0.0}},
	{L"FFYU",{L"ッフュ",0.0}},
	{L"FI",{L"フィ",1.0}},
	{L"FO",{L"フォ",1.0}},
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
	{L"GGA",{L"ッガ",1.0}},
	{L"GGE",{L"ッゲ",1.0}},
	{L"GGI",{L"ッギ",1.0}},
	{L"GGO",{L"ッゴ",1.0}},
	{L"GGU",{L"ッグ",1.0}},
	{L"GGWA",{L"ッグァ",0.0}},
	{L"GGWE",{L"ッグェ",0.0}},
	{L"GGWI",{L"ッグィ",0.0}},
	{L"GGWO",{L"ッグォ",0.0}},
	{L"GGWU",{L"ッグゥ",0.0}},
	{L"GGYA",{L"ッギャ",1.0}},
	{L"GGYE",{L"ッギェ",1.0}},
	{L"GGYI",{L"ッギィ",1.0}},
	{L"GGYO",{L"ッギョ",1.0}},
	{L"GGYU",{L"ッギュ",1.0}},
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
	{L"HA",{L"ハ",1.0}},
	{L"HE",{L"ヘ",1.0}},
	{L"HHA",{L"ッハ",1.0}},
	{L"HHE",{L"ッヘ",1.0}},
	{L"HHI",{L"ッヒ",1.0}},
	{L"HHO",{L"ッポ",1.0}},
	{L"HHU",{L"ッフ",1.0}},
	{L"HHYA",{L"ッヒャ",0.0}},
	{L"HHYE",{L"ッヒェ",0.0}},
	{L"HHYI",{L"ッヒィ",0.0}},
	{L"HHYO",{L"ッヒョ",0.0}},
	{L"HHYU",{L"ッヒュ",0.0}},
	{L"HI",{L"ヒ",1.0}},
	{L"HO",{L"ホ",1.0}},
	{L"HU",{L"フ",1.0}},
	{L"HYA",{L"ヒャ",0.0}},
	{L"HYE",{L"ヒェ",0.0}},
	{L"HYI",{L"ヒィ",0.0}},
	{L"HYO",{L"ヒョ",0.0}},
	{L"HYU",{L"ヒュ",0.0}},
	{L"I",{L"イ",1.0}},
	{L"JA",{L"ジャ",1.0}},
	{L"JE",{L"ジェ",1.0}},
	{L"JI",{L"ジ",1.0}},
	{L"JJA",{L"ッジャ",1.0}},
	{L"JJE",{L"ッジェ",1.0}},
	{L"JJI",{L"ッジ",1.0}},
	{L"JJO",{L"ッジォ",1.0}},
	{L"JJU",{L"ッジュ",1.0}},
	{L"JJYA",{L"ッジャ",0.0}},
	{L"JJYE",{L"ッジェ",0.0}},
	{L"JJYI",{L"ッジィ",0.0}},
	{L"JJYO",{L"ッジョ",0.0}},
	{L"JJYU",{L"ッジュ",0.0}},
	{L"JO",{L"ジョ",1.0}},
	{L"JU",{L"ジュ",1.0}},
	{L"JYA",{L"ジャ",1.0}},
	{L"JYE",{L"ジェ",1.0}},
	{L"JYI",{L"ジィ",1.0}},
	{L"JYO",{L"ジョ",1.0}},
	{L"JYU",{L"ジュ",1.0}},
	{L"KA",{L"カ",1.0}},
	{L"KE",{L"ケ",1.0}},
	{L"KI",{L"キ",1.0}},
	{L"KKA",{L"ッカ",1.0}},
	{L"KKE",{L"ッケ",1.0}},
	{L"KKI",{L"ッキ",1.0}},
	{L"KKO",{L"ッコ",1.0}},
	{L"KKU",{L"ック",1.0}},
	{L"KKWA",{L"ックァ",0.0}},
	{L"KKYA",{L"ッキャ",0.0}},
	{L"KKYE",{L"ッキェ",0.0}},
	{L"KKYI",{L"ッキィ",0.0}},
	{L"KKYO",{L"ッキョ",0.0}},
	{L"KKYU",{L"ッキュ",0.0}},
	{L"KO",{L"コ",1.0}},
	{L"KU",{L"ク",1.0}},
	{L"KWA",{L"クァ",0.0}},
	{L"KYA",{L"キャ",1.0}},
	{L"KYE",{L"キェ",1.0}},
	{L"KYI",{L"キィ",1.0}},
	{L"KYO",{L"キョ",1.0}},
	{L"KYU",{L"キュ",1.0}},
	{L"LA",{L"ァ",0.0}},
	{L"LE",{L"ェ",0.0}},
	{L"LI",{L"ィ",0.0}},
	{L"LKA",{L"ヵ",0.0}},
	{L"LKE",{L"ヶ",0.0}},
	{L"LLA",{L"ッァ",0.0}},
	{L"LLE",{L"ッェ",0.0}},
	{L"LLI",{L"ッィ",0.0}},
	{L"LLO",{L"ッォ",0.0}},
	{L"LLU",{L"ッゥ",0.0}},
	{L"LO",{L"ォ",0.0}},
	{L"LTSU",{L"ッ",0.0}},
	{L"LTU",{L"ッ",0.0}},
	{L"LU",{L"ゥ",0.0}},
	{L"LWA",{L"ヮ",0.0}},
	{L"LYA",{L"ャ",0.0}},
	{L"LYE",{L"ェ",0.0}},
	{L"LYI",{L"ィ",0.0}},
	{L"LYO",{L"ョ",0.0}},
	{L"LYU",{L"ュ",0.0}},
	{L"MA",{L"マ",0.0}},
	{L"ME",{L"メ",0.0}},
	{L"MI",{L"ミ",0.0}},
	{L"MMA",{L"ッマ",1.0}},
	{L"MME",{L"ッメ",1.0}},
	{L"MMI",{L"ッミ",1.0}},
	{L"MMO",{L"ッモ",1.0}},
	{L"MMU",{L"ッム",1.0}},
	{L"MMYA",{L"ッミャ",1.0}},
	{L"MMYE",{L"ッミェ",1.0}},
	{L"MMYI",{L"ッミィ",1.0}},
	{L"MMYO",{L"ッミョ",1.0}},
	{L"MMYU",{L"ッミュ",1.0}},
	{L"MO",{L"モ",0.0}},
	{L"MU",{L"ム",0.0}},
	{L"MYA",{L"ミャ",0.0}},
	{L"MYE",{L"ミェ",0.0}},
	{L"MYI",{L"ミィ",0.0}},
	{L"MYO",{L"ミョ",0.0}},
	{L"MYU",{L"ミュ",0.0}},
	{L"NA",{L"ナ",1.0}},
	{L"NE",{L"ネ",1.0}},
	{L"NI",{L"ニ",1.0}},
	{L"NN",{L"ン",1.0}},
	{L"NO",{L"ノ",1.0}},
	{L"NU",{L"ヌ",1.0}},
	{L"NYA",{L"ニャ",0.0}},
	{L"NYE",{L"ニェ",0.0}},
	{L"NYI",{L"ニィ",0.0}},
	{L"NYO",{L"ニョ",0.0}},
	{L"NYU",{L"ニュ",0.0}},
	{L"O",{L"オ",1.0}},
	{L"PA",{L"パ",0.0}},
	{L"PE",{L"ペ",0.0}},
	{L"PI",{L"ピ",0.0}},
	{L"PO",{L"ポ",0.0}},
	{L"PPA",{L"ッバ",1.0}},
	{L"PPE",{L"ッベ",1.0}},
	{L"PPI",{L"ッピ",1.0}},
	{L"PPO",{L"ッポ",1.0}},
	{L"PPU",{L"ップ",1.0}},
	{L"PPYA",{L"ッピャ",0.0}},
	{L"PPYE",{L"ッピェ",0.0}},
	{L"PPYI",{L"ッピィ",0.0}},
	{L"PPYO",{L"ッピョ",0.0}},
	{L"PPYU",{L"ッピュ",0.0}},
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
	{L"QQWA",{L"ックァ",0.0}},
	{L"QQWE",{L"ックェ",0.0}},
	{L"QQWI",{L"ックィ",0.0}},
	{L"QQWO",{L"ックォ",0.0}},
	{L"QQWU",{L"ックゥ",0.0}},
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
	{L"RRYA",{L"ッリャ",0.0}},
	{L"RRYE",{L"ッリェ",0.0}},
	{L"RRYI",{L"ッリィ",0.0}},
	{L"RRYO",{L"ッリョ",0.0}},
	{L"RRYU",{L"ッリュ",0.0}},
	{L"RU",{L"ル",0.0}},
	{L"RYA",{L"リャ",1.0}},
	{L"RYO",{L"リョ",1.0}},
	{L"RYU",{L"リュ",1.0}},
	{L"SA",{L"サ",1.0}},
	{L"SE",{L"セ",1.0}},
	{L"SHA",{L"シャ",0.0}},
	{L"SHE",{L"シェ",0.0}},
	{L"SHI",{L"シ",0.0}},
	{L"SHO",{L"ショ",0.0}},
	{L"SHU",{L"シュ",0.0}},
	{L"SI",{L"シ",1.0}},
	{L"SO",{L"ソ",1.0}},
	{L"SSA",{L"ッサ",1.0}},
	{L"SSE",{L"ッセ",1.0}},
	{L"SSHA",{L"ッシャ",0.0}},
	{L"SSHE",{L"ッシェ",0.0}},
	{L"SSHI",{L"ッシ",0.0}},
	{L"SSHO",{L"ッショ",0.0}},
	{L"SSHU",{L"ッシュ",0.0}},
	{L"SSI",{L"ッシ",1.0}},
	{L"SSO",{L"ッソ",1.0}},
	{L"SSU",{L"ッス",1.0}},
	{L"SSWA",{L"ッスァ",0.0}},
	{L"SSWE",{L"ッスェ",0.0}},
	{L"SSWI",{L"ッスィ",0.0}},
	{L"SSWO",{L"ッスォ",0.0}},
	{L"SSWU",{L"ッスゥ",0.0}},
	{L"SSYA",{L"ッシャ",0.0}},
	{L"SSYE",{L"ッシェ",0.0}},
	{L"SSYI",{L"ッシィ",0.0}},
	{L"SSYO",{L"ッショ",0.0}},
	{L"SSYU",{L"ッシュ",0.0}},
	{L"SU",{L"ス",1.0}},
	{L"SWA",{L"スァ",0.0}},
	{L"SWE",{L"スェ",0.0}},
	{L"SWI",{L"スィ",0.0}},
	{L"SWO",{L"スォ",0.0}},
	{L"SWU",{L"スゥ",0.0}},
	{L"SYA",{L"シャ",1.0}},
	{L"SYE",{L"シェ",1.0}},
	{L"SYE",{L"シェ",1.0}},
	{L"SYI",{L"シィ",1.0}},
	{L"SYO",{L"ショ",1.0}},
	{L"SYU",{L"シュ",1.0}},
	{L"TA",{L"タ",1.0}},
	{L"TE",{L"テ",1.0}},
	{L"THA",{L"テャ",0.0}},
	{L"THE",{L"テェ",0.0}},
	{L"THI",{L"ティ",0.0}},
	{L"THO",{L"テョ",0.0}},
	{L"THU",{L"テュ",0.0}},
	{L"TI",{L"チ",1.0}},
	{L"TO",{L"ト",1.0}},
	{L"TSA",{L"ツァ",0.0}},
	{L"TSE",{L"ツェ",0.0}},
	{L"TSI",{L"ツィ",0.0}},
	{L"TSO",{L"ツォ",0.0}},
	{L"TSU",{L"ツ",0.0}},
	{L"TTA",{L"ッタ",1.0}},
	{L"TTE",{L"ッテ",1.0}},
	{L"TTHA",{L"ッテャ",0.0}},
	{L"TTHE",{L"ッテェ",0.0}},
	{L"TTHI",{L"ッティ",0.0}},
	{L"TTHO",{L"ッテョ",0.0}},
	{L"TTHU",{L"ッテュ",0.0}},
	{L"TTI",{L"ッチ",1.0}},
	{L"TTO",{L"ット",1.0}},
	{L"TTSA",{L"ッツァ",0.0}},
	{L"TTSE",{L"ッツェ",0.0}},
	{L"TTSI",{L"ッツィ",0.0}},
	{L"TTSO",{L"ッツォ",0.0}},
	{L"TTSU",{L"ッツ",0.0}},
	{L"TTU",{L"ッツ",1.0}},
	{L"TTWA",{L"ットァ",0.0}},
	{L"TTWE",{L"ットェ",0.0}},
	{L"TTWI",{L"ットィ",0.0}},
	{L"TTWO",{L"ットォ",0.0}},
	{L"TTWU",{L"ットゥ",0.0}},
	{L"TTYA",{L"ッチャ",0.0}},
	{L"TTYE",{L"ッチェ",0.0}},
	{L"TTYI",{L"ッチィ",0.0}},
	{L"TTYO",{L"ッチョ",0.0}},
	{L"TTYU",{L"ッチュ",0.0}},
	{L"TU",{L"ツ",1.0}},
	{L"TWA",{L"トァ",0.0}},
	{L"TWE",{L"トェ",0.0}},
	{L"TWI",{L"トィ",0.0}},
	{L"TWO",{L"トォ",0.0}},
	{L"TWU",{L"トゥ",0.0}},
	{L"TYA",{L"チャ",1.0}},
	{L"TYE",{L"チェ",1.0}},
	{L"TYI",{L"チィ",1.0}},
	{L"TYO",{L"チョ",1.0}},
	{L"TYU",{L"チュ",1.0}},
	{L"U",{L"ウ",1.0}},
	{L"VA",{L"ヴァ",0.0}},
	{L"VE",{L"ヴェ",0.0}},
	{L"VI",{L"ヴィ",0.0}},
	{L"VO",{L"ヴォ",0.0}},
	{L"VU",{L"ヴ",0.0}},
	{L"VVA",{L"ッヴァ",0.0}},
	{L"VVE",{L"ッヴェ",1.0}},
	{L"VVI",{L"ッヴィ",1.0}},
	{L"VVO",{L"ッヴォ",0.0}},
	{L"VVU",{L"ッヴ",0.0}},
	{L"VVYA",{L"ッヴャ",0.0}},
	{L"VVYE",{L"ッヴェ",0.0}},
	{L"VVYI",{L"ッヴィ",0.0}},
	{L"VVYO",{L"ッヴョ",0.0}},
	{L"VVYU",{L"ッヴュ",0.0}},
	{L"VYA",{L"ヴャ",0.0}},
	{L"VYE",{L"ヴェ",0.0}},
	{L"VYI",{L"ヴィ",0.0}},
	{L"VYO",{L"ヴョ",0.0}},
	{L"VYU",{L"ヴュ",0.0}},
	{L"WA",{L"ワ",0.0}},
	{L"WE",{L"ウェ",0.0}},
	{L"WHA",{L"ウァ",0.0}},
	{L"WHE",{L"ウェ",0.0}},
	{L"WHI",{L"ウィ",0.0}},
	{L"WHO",{L"ウォ",0.0}},
	{L"WHU",{L"ウ",0.0}},
	{L"WI",{L"ウィ",0.0}},
	{L"WO",{L"ヲ",0.0}},
	{L"WU",{L"ウ",0.0}},
	{L"WWA",{L"ッワ",0.0}},
	{L"WWE",{L"ッウェ",0.0}},
	{L"WWI",{L"ッウィ",0.0}},
	{L"WWO",{L"ッヲ",0.0}},
	{L"WWU",{L"ッウ",0.0}},
	{L"XA",{L"ァ",0.0}},
	{L"XE",{L"ェ",0.0}},
	{L"XI",{L"ィ",0.0}},
	{L"XKA",{L"ヵ",0.0}},
	{L"XKE",{L"ヶ",0.0}},
	{L"XN",{L"ン",0.0}},
	{L"XO",{L"ォ",0.0}},
	{L"XTSU",{L"ッ",0.0}},
	{L"XTU",{L"ッ",0.0}},
	{L"XU",{L"ゥ",0.0}},
	{L"XWA",{L"ヮ",0.0}},
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
	{L"YI",{L"イ",0.0}},
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
std::map<std::wstring, std::wstring> g_kana{
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
	GS_GAMESTATUS,
	GS_RANKING,
	GS_NAMEINPUT,
	GS_SETTINGS,
} GAME_STATE;
void PlaySound(int nSoundKind)
{
	if (0 <= nSoundKind && nSoundKind < MAX_SOUND_COUNT && dsb1[nSoundKind] != nullptr)
	{
		dsb1[nSoundKind]->SetCurrentPosition(0);
		dsb1[nSoundKind]->Play(0, 0, 0);
	}
}
struct game
{
	game() :szInputRome{}, szQuestionKana{}, szQuestionRome{}
	{
		nGameState = GS_TITLE;
		nMaxTimeCount = 60;
		nTimeCount = nMaxTimeCount;
		nCursorKana = 0;
		nCursorRome = 0;
		nCategory = 1;
		nTypeCountKana = 0;
		nTypeCountRome = 0;
		bMiss = FALSE;
		nMissCount = 0;
		nQuestionCount = 0;
		nMaxQuestionCount = nMaxTimeCount * 100;
		nCountDown = 0;
		list.clear();
	}
	BOOL LoadWordsFromDatabase();
	LPWSTR GetRomeFromKana(LPCWSTR);
	void start()
	{
		list.clear();
		nCursorKana = 0;
		nCursorRome = 0;
		nTypeCountKana = 0;
		nTypeCountRome = 0;
		bMiss = FALSE;
		nMissCount = 0;
		nQuestionCount = 0;
		nMaxQuestionCount = nMaxTimeCount * 100;
		nMaxTimeCount = 60;
		nTimeCount = nMaxTimeCount;
		szInputRome[0] = 0;
		LoadWordsFromDatabase();
		lstrcpy(szQuestionKana, list[nQuestionCount].kana.c_str());
		lstrcpy(szQuestionRome, GetRomeFromKana(szQuestionKana));
		nGameState = GS_GAMESTART;
	}
	void next()
	{
		nQuestionCount++;
		lstrcpy(szQuestionKana, list[nQuestionCount % list.size()].kana.c_str());
		lstrcpy(szQuestionRome, GetRomeFromKana(szQuestionKana));			
		szInputRome[0] = 0;
		nCursorKana = 0;
		nCursorRome = 0;
		bMiss = FALSE;
	}
	std::vector<sentence> list;
	GAME_STATE nGameState;
	int nTypeCountKana;
	int nTypeCountRome;
	BOOL bMiss;
	int nMissCount;
	int nQuestionCount;
	int nMaxQuestionCount;
	int nTimeCount;
	int nMaxTimeCount;
	int nCursorKana; // 現在のかな入力位置
	int nCursorRome; // 現在のローマ字入力位置
	WCHAR szInputRome[5]; // 現在入力されたローマ字
	int nCategory;
	WCHAR szQuestionKana[256]; // カタカナを配列で用意しておく
	WCHAR szQuestionRome[1024]; // ローマ字を配列で用意しておく
	std::vector<int> kanakugiri; // どこで区切るかの情報
	int nCountDown;
};
template<class Interface> inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}
void GetAppFolderPath(OUT LPWSTR lpszFolderPath)
{
	if (SHGetSpecialFolderPath(hWnd, lpszFolderPath, CSIDL_COMMON_APPDATA, 0))
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
		PathAppend(szFilePath, L"words.db");
	}
	return szFilePath;
}
LPCWSTR GetKeysDataBaseFilePath()
{
	static WCHAR szFilePath[MAX_PATH] = {};
	if (szFilePath[0] == L'\0')
	{
		GetAppFolderPath(szFilePath);
		PathAppend(szFilePath, L"keys.db");
	}
	return szFilePath;
}
LPCWSTR GetRankingDataBaseFilePath()
{
	static WCHAR szFilePath[MAX_PATH] = {};
	if (szFilePath[0] == L'\0')
	{
		GetAppFolderPath(szFilePath);
		PathAppend(szFilePath, L"ranking.db");
	}
	return szFilePath;
}
int sqlite3_exec_w(sqlite3* db, LPCWSTR sql, int (*callback)(void*,int,char**,char**), void* arg, char** errmsg)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, sql, -1, NULL, 0, NULL, NULL);
	if (len == 0) return SQLITE_ERROR;
	char* utf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, sql, -1, utf8, len, NULL, NULL);
	int rc = sqlite3_exec(db, utf8, callback, arg, errmsg);
	delete[] utf8;
	return rc;
}
sqlite3_stmt* sqlite3_prepare_w(sqlite3* db, LPCWSTR sql)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, sql, -1, NULL, 0, NULL, NULL);
	if (len == 0) return nullptr;
	char* utf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, sql, -1, utf8, len, NULL, NULL);
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(db, utf8, -1, &stmt, nullptr);
	delete[] utf8;
	return stmt;
}
BOOL SQLExecute(HWND hWnd, LPCWSTR lpszDBFilePath, LPCWSTR lpszSQL)
{
	sqlite3* db = nullptr;
	if (sqlite3_open16(lpszDBFilePath, &db) == SQLITE_OK)
	{
		char* errmsg = nullptr;
		int rc = sqlite3_exec_w(db, lpszSQL, NULL, NULL, &errmsg);
		if (rc != SQLITE_OK && errmsg)
		{
			WCHAR szErr[1024];
			MultiByteToWideChar(CP_UTF8, 0, errmsg, -1, szErr, 1024);
			MessageBox(hWnd, szErr, 0, 0);
			sqlite3_free(errmsg);
		}
		sqlite3_close(db);
		return rc == SQLITE_OK;
	}
	return FALSE;
}
void CreateKeysDatabaseFile(HWND hWnd)
{
	LPCWSTR lpszDataBaseFilePath = GetKeysDataBaseFilePath();
	if (!PathFileExists(lpszDataBaseFilePath))
	{
		if (!SQLExecute(hWnd, lpszDataBaseFilePath, L"CREATE TABLE item(rome TEXT NOT NULL PRIMARY KEY, kana TEXT NOT NULL, priority REAL NOT NULL);"))
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}
		sqlite3* db = nullptr;
		if (sqlite3_open16(lpszDataBaseFilePath, &db) == SQLITE_OK)
		{
			sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
			for (auto i : g_rome)
			{
				WCHAR szSQL[1024];
				swprintf_s(szSQL, L"INSERT INTO item(rome, kana, priority) VALUES ('%s', '%s', %lf);", i.first.c_str(), i.second.kana1.c_str(), (double)i.second.priority);
				sqlite3_exec_w(db, szSQL, NULL, NULL, NULL);
			}
			sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
			sqlite3_close(db);
		}
	}
}
void InsertRanking(HWND hWnd, DWORD category, LPSYSTEMTIME date, double score, double typecount, double misscount)
{
	LPCWSTR lpszDataBaseFilePath = GetRankingDataBaseFilePath();
	if (PathFileExists(lpszDataBaseFilePath))
	{
		WCHAR szSQL[1024];
		double date1;
		SystemTimeToVariantTime(date, &date1);
		swprintf_s(szSQL, L"INSERT INTO item(category, date1, score1, typecount1, misscount1) VALUES (%d, %lf, %lf, %lf, %lf);",
			category, date1, score, typecount, misscount);
		SQLExecute(hWnd, lpszDataBaseFilePath, szSQL);
	}
}

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

void CreateRankingDatabaseFile(HWND hWnd)
{
	LPCWSTR lpszDataBaseFilePath = GetRankingDataBaseFilePath();
	if (!PathFileExists(lpszDataBaseFilePath))
	{
		if (!SQLExecute(hWnd, lpszDataBaseFilePath, L"CREATE TABLE item(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, category INTEGER NOT NULL, date1 REAL NOT NULL, score1 REAL NOT NULL, typecount1 REAL NOT NULL, misscount1 REAL NOT NULL);"))
		{
			DeleteFile(lpszDataBaseFilePath);
			return;
		}
	}
}
BOOL IsAlphabet(WCHAR c)
{
	if ('A' <= c && c <= 'Z' ||
		'a' <= c && c <= 'z')
	{
		return TRUE;
	}
	return FALSE;
}
LPWSTR game::GetRomeFromKana(LPCWSTR lpszKana)
{
	static WCHAR szRome[1024];
	LPCWSTR p = lpszKana;
	szRome[0] = 0;
	while (*p != L'\0')
	{
		if (IsAlphabet(*p))
		{
			WCHAR c[2] = { towupper(*p) };
			lstrcat(szRome, c);
			p++;
			continue;
		}
		switch (*p)
		{
		case L'　':
		case L' ':
			lstrcat(szRome, L" ");
			p++;
			continue;
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
		int nStep = 0;
		if (*(p + 1) != 0 && *(p + 2) != 0)
		{
			WCHAR szKana[4] = { *p, *(p + 1), *(p + 2), 0 };
			auto it = g_kana.find(szKana);
			if (it != g_kana.end())
			{
				lstrcat(szRome, (*it).second.c_str());
				nStep = 3;
			}
		}
		// 2文字探す
		if (nStep == 0 && *(p + 1) != 0)
		{
			WCHAR szKana[3] = { *p, *(p + 1), 0 };
			auto it = g_kana.find(szKana);
			if (it != g_kana.end())
			{
				lstrcat(szRome, (*it).second.c_str());
				nStep = 2;
			}
		}
		// 1文字探す
		if (nStep == 0)
		{
			WCHAR szKana[2] = { *p, 0 };
			auto it = g_kana.find(szKana);
			if (it != g_kana.end())
			{
				lstrcat(szRome, (*it).second.c_str());
				nStep = 1;
			}
		}
		if (nStep > 0)
		{
			p += nStep;
			continue;
		}
		break;
	}
	return szRome;
}
BOOL game::LoadWordsFromDatabase()
{
	BOOL bRet = FALSE;
	sqlite3* db = nullptr;
	if (sqlite3_open16(GetWordsDataBaseFilePath(), &db) == SQLITE_OK)
	{
		WCHAR szSQL[256];
		wsprintf(szSQL, L"SELECT * FROM item WHERE category = %d ORDER BY RANDOM() LIMIT %d;", nCategory, nMaxQuestionCount);
		sqlite3_stmt* stmt = sqlite3_prepare_w(db, szSQL);
		if (stmt)
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				sentence data;
				const WCHAR* words = (const WCHAR*)sqlite3_column_text16(stmt, 2);
				if (words) data.words = words;
				
				const WCHAR* kana = (const WCHAR*)sqlite3_column_text16(stmt, 3);
				if (kana) data.kana = kana;
				list.push_back(data);
				bRet = TRUE;
			}
			sqlite3_finalize(stmt);
		}
		sqlite3_close(db);
	}
	if (list.empty())
	{
		sentence data;
		data.words = L"問題データがありません";
		data.kana = L"モンダイデータガアリマセン";
		list.push_back(data);
	}
	return bRet;
}

struct HistoryItem {
	double date;
	double score;
	double accuracy;
};

void GetRankingHistoryFromDatabase(std::vector<HistoryItem>& history, int nCategory)
{
	history.clear();
	sqlite3* db = nullptr;
	if (sqlite3_open16(GetRankingDataBaseFilePath(), &db) == SQLITE_OK)
	{
		WCHAR szSQL[256];
		wsprintf(szSQL, L"SELECT date1, score1, typecount1, misscount1 FROM item WHERE category = %d ORDER BY date1 ASC LIMIT 3650;", nCategory);
		sqlite3_stmt* stmt = sqlite3_prepare_w(db, szSQL);
		if (stmt)
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				HistoryItem item;
				item.date = sqlite3_column_double(stmt, 0);
				item.score = sqlite3_column_double(stmt, 1);
				double typecount = sqlite3_column_double(stmt, 2);
				double misscount = sqlite3_column_double(stmt, 3);
				item.accuracy = 0;
				if (typecount + misscount > 0)
				{
					item.accuracy = (typecount / (typecount + misscount)) * 100.0;
				}
				history.push_back(item);
			}
			sqlite3_finalize(stmt);
		}
		sqlite3_close(db);
	}
}

BOOL GetRankingFromDatabase(std::wstring& ranking, int nCategory)
{
	ranking = L"[ランキング]\r\n";
	BOOL bRet = FALSE;
	sqlite3* db = nullptr;
	if (sqlite3_open16(GetRankingDataBaseFilePath(), &db) == SQLITE_OK)
	{
		WCHAR szSQL[256];
		int id = 0;
		wsprintf(szSQL, L"SELECT * FROM item WHERE category = %d ORDER BY date1 DESC LIMIT 1;", nCategory);
		sqlite3_stmt* stmt = sqlite3_prepare_w(db, szSQL);
		if (stmt)
		{
			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				id = sqlite3_column_int(stmt, 0);
			}
			sqlite3_finalize(stmt);
		}
		if (id > 0)
		{
			wsprintf(szSQL, L"SELECT * FROM item WHERE category = %d ORDER BY score1 DESC LIMIT 10;", nCategory);
			stmt = sqlite3_prepare_w(db, szSQL);
			if (stmt)
			{
				int i = 0;
				while (sqlite3_step(stmt) == SQLITE_ROW)
				{
					const int id_ = sqlite3_column_int(stmt, 0);
					double date = sqlite3_column_double(stmt, 2);
					double score = sqlite3_column_double(stmt, 3);
					double typecount = sqlite3_column_double(stmt, 4);
					double misscount = sqlite3_column_double(stmt, 5);
					
					WCHAR szText[1024];
					SYSTEMTIME st;
					VariantTimeToSystemTime(date, &st);
					wsprintf(szText, L"%d位: %d (%04d/%02d/%02d %02d:%02d:%02d)", i + 1, (int)score, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
					if (id == id_) {
						lstrcat(szText, L" ☚ 直近");
					}
					lstrcat(szText, L"\r\n");
					ranking += szText;
					i++;
				}
				bRet = TRUE;
				sqlite3_finalize(stmt);
			}
		}
		sqlite3_close(db);
	}
	ranking += L"[SPACE]キーでタイトルへ";
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
BOOL JudgeFromKigou(DWORD dwKeyCode, LPCWSTR lpszKana, int nCursorKana)
{
	switch (lpszKana[nCursorKana])
	{
	case L'0':
	case L'０':
		if (dwKeyCode == L'0') return TRUE;
		break;
	case L'1':
	case L'１':
		if (dwKeyCode == L'1') return TRUE;
		break;
	case L'2':
	case L'２':
		if (dwKeyCode == L'2') return TRUE;
		break;
	case L'3':
	case L'３':
		if (dwKeyCode == L'3') return TRUE;
		break;
	case L'4':
	case L'４':
		if (dwKeyCode == L'4') return TRUE;
		break;
	case L'5':
	case L'５':
		if (dwKeyCode == L'5') return TRUE;
		break;
	case L'6':
	case L'６':
		if (dwKeyCode == L'6') return TRUE;
		break;
	case L'7':
	case L'７':
		if (dwKeyCode == L'7') return TRUE;
		break;
	case L'8':
	case L'８':
		if (dwKeyCode == L'8') return TRUE;
		break;
	case L'9':
	case L'９':
		if (dwKeyCode == L'9') return TRUE;
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
	case L'＋':
		if (dwKeyCode == VK_OEM_PLUS) return TRUE;
		break;
	case L'?':
	case L'？':
		if (dwKeyCode == VK_OEM_2) return TRUE;
		break;
	case L'/':
	case L'／':
		if (dwKeyCode == VK_OEM_2) return TRUE;
		break;
	case L':':
	case L'：':
		if (dwKeyCode == VK_OEM_1) return TRUE;
		break;
	case L';':
	case L'；':
		if (dwKeyCode == VK_OEM_1) return TRUE;
		break;
	case L'`':
	case L'‘':
		if (dwKeyCode == VK_OEM_3) return TRUE;
		break;
	case L'~':
	case L'～':
		if (dwKeyCode == VK_OEM_3) return TRUE;
		break;		
	}
	return FALSE;
}
BOOL JudgeFromKana(IN LPCWSTR lpszInputAlphabet, IN LPCWSTR lpszKana, IN int nCursorKana, OUT LPWSTR lpszOutputKana, OUT LPWSTR lpszOutputRome)
{
	std::map<std::wstring, kana>::const_iterator i = g_rome.lower_bound(lpszInputAlphabet);
	while (i != g_rome.end())
	{
		const std::wstring& rome = i->first;
		if (rome.compare(0, lstrlenW(lpszInputAlphabet), lpszInputAlphabet) != 0)
		{
			return FALSE;
		}
		const std::wstring& kana = i->second.kana1;
		if (StrCmpN(lpszKana + nCursorKana, kana.c_str(), (int)kana.length()) == 0)
		{
			lstrcpy(lpszOutputKana, kana.c_str());
			lstrcpy(lpszOutputRome, i->first.c_str());
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
BOOL IsBoin(WCHAR c)
{
	switch (c)
	{
	case L'A':
	case L'I':
	case L'U':
	case L'E':
	case L'O':
		return TRUE;
	}
	return FALSE;
}
HRESULT LoadResourceBitmap(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory,
	PCWSTR resourceName,
	PCWSTR resourceType,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap** ppBitmap
)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;
	// Locate the resource.
	imageResHandle = FindResourceW(GetModuleHandle(0), resourceName, resourceType);
	HRESULT hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(GetModuleHandle(0), imageResHandle);
		hr = imageResDataHandle ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);
		hr = pImageFile ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(GetModuleHandle(0), imageResHandle);
		hr = imageFileSize ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = pIWICFactory->CreateStream(&pStream);
	}
	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = pIWICFactory->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		if (SUCCEEDED(hr))
		{
			//create a Direct2D bitmap from the WIC bitmap.
			hr = pRenderTarget->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				ppBitmap
			);
		}
	}
	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	return hr;
}
void GetLocalFont(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType, LPVOID* ppFontMemory, LPINT pFontMemorySize)
{
	HRSRC hRsrc = FindResource(hModule, lpName, lpType);
	if (hRsrc) {
		HGLOBAL hResource = LoadResource(hModule, hRsrc);
		*ppFontMemory = LockResource(hResource);
		*pFontMemorySize = SizeofResource(hModule, hRsrc);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static ID2D1Factory* m_pD2DFactory;
	static IWICImagingFactory* m_pWICFactory;
	static IDWriteFactory* m_pDWriteFactory;
	static ID2D1HwndRenderTarget* m_pRenderTarget;
	static IDWriteTextFormat* m_pTextFormat;
	static IDWriteTextFormat* m_pMediumTextFormat; // 追加: ランキングテキスト用 (サイズ中)
	static IDWriteTextFormat* m_pSmallTextFormat;  // 追加: グラフ目盛り・凡例用 (サイズ小)
	static ID2D1SolidColorBrush* m_pBlackBrush;
	static ID2D1SolidColorBrush* m_pGrayBrush;
	static ID2D1SolidColorBrush* m_pRedBrush;
	static ID2D1SolidColorBrush* m_pBlueBrush;
	static ID2D1SolidColorBrush* m_pGreenBrush;
	static IDWriteInMemoryFontFileLoader* pInMemoryFontFileLoader;
	static IDWriteFontFile* pFontFile;
	static IDWriteFontCollection1* pFontCollection;
	static IDWriteFontSetBuilder1* fontSetBuilder;
	static IDWriteFontSet* pFontSet;
	static LPVOID pFontMemory;
	static INT nFontMemorySize;
	static UINT uDpiX = DEFAULT_DPI, uDpiY = DEFAULT_DPI;
	static std::wstring ranking;
	static std::vector<HistoryItem> rankingHistory;
	static POINT ptMouse = { -1, -1 };
	static WCHAR szFontFamilyName[256] = L"メイリオ"; // ← ここに追加	
	static game g;
	switch (msg)
	{
	case WM_MOUSEMOVE:
		ptMouse.x = (int)(short)LOWORD(lParam);
		ptMouse.y = (int)(short)HIWORD(lParam);
		if (g.nGameState == GAME_STATE::GS_RANKING) {
			InvalidateRect(hWnd, 0, 0);
		}
		break;
	case WM_CREATE:
		{
			if (g_kana.empty())
			{
				for (const auto& item : g_rome)
				{
					auto it = g_kana.find(item.second.kana1);
					// まだ登録がないか、優先度(priority)が1.0以上の標準入力であれば登録・上書き
					if (it == g_kana.end() || item.second.priority >= 1.0)
					{
						g_kana[item.second.kana1] = item.first;
					}
				}
			}
			(void)CoInitialize(NULL);
			CreateKeysDatabaseFile(hWnd);
			CreateRankingDatabaseFile(hWnd);
			CreateWordsDatabaseFile(hWnd);
			static const FLOAT msc_fontSize = 32;
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
			if (SUCCEEDED(hr))
				hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
			
			if (SUCCEEDED(hr))
			{
				IDWriteFactory5* pDWriteFactory5 = nullptr;
				if (SUCCEEDED(m_pDWriteFactory->QueryInterface(__uuidof(IDWriteFactory5), reinterpret_cast<void**>(&pDWriteFactory5))))
				{
					if (SUCCEEDED(pDWriteFactory5->CreateInMemoryFontFileLoader(&pInMemoryFontFileLoader)))
					{
						if (SUCCEEDED(pDWriteFactory5->RegisterFontFileLoader(pInMemoryFontFileLoader)))
						{
							GetLocalFont(GetModuleHandle(0), MAKEINTRESOURCE(IDR_FONT1), RT_FONT, &pFontMemory, &nFontMemorySize);
							if (SUCCEEDED(pInMemoryFontFileLoader->CreateInMemoryFontFileReference(pDWriteFactory5, pFontMemory, nFontMemorySize, nullptr, &pFontFile)))
							{
								if (SUCCEEDED(pDWriteFactory5->CreateFontSetBuilder(&fontSetBuilder)))
								{
									if (SUCCEEDED(fontSetBuilder->AddFontFile(pFontFile)))
									{
										if (SUCCEEDED(fontSetBuilder->CreateFontSet(&pFontSet)))
										{
											if (SUCCEEDED(pDWriteFactory5->CreateFontCollectionFromFontSet(pFontSet, &pFontCollection)))
											{
												UINT32 uFontCount = pFontCollection->GetFontFamilyCount();
												if (uFontCount > 0)
												{
													IDWriteFontFamily1* pFontFamily = nullptr;
													if (SUCCEEDED(pFontCollection->GetFontFamily(0, &pFontFamily)))
													{
														IDWriteLocalizedStrings* string = nullptr;
														if (SUCCEEDED(pFontFamily->GetFamilyNames(&string)))
														{
															string->GetString(0, szFontFamilyName, _countof(szFontFamilyName));
															string->Release();
														}
														pFontFamily->Release();
													}
												}
											}
										}
									}
								}
							}
						}
					}
					pDWriteFactory5->Release();
				}
			}
			if (SUCCEEDED(hr))
			{
				IDWriteFontCollection* pCollection = nullptr;
				if (pFontCollection) pCollection = pFontCollection;
				hr = m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, msc_fontSize, L"", &m_pTextFormat);

				// --- 以下2行を追加 ---
				hr = m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"", &m_pMediumTextFormat);
				hr = m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 11.0f, L"", &m_pSmallTextFormat);
			}
			if (SUCCEEDED(hr))
				hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&m_pWICFactory));
			if (FAILED(hr))
				return -1;
		}
		SendMessage(hWnd, WM_APP, 0, 0);
		// タイトル画面を表示
		g.nGameState = GS_TITLE;
		break;
	case WM_TIMER:
		debuglog(L"WM_TIMER %d", wParam);
		if (wParam == 1000) // ゲーム中のタイマ
		{
			if (g.nTimeCount <= 1)
			{
				KillTimer(hWnd, 1000);
				g.nGameState = GS_GAMEEND;
				{
					// ここでRankingに登録
					SYSTEMTIME time;
					GetLocalTime(&time);
					double dCorrectAnswerRate = 1.0 * (g.nTypeCountRome - g.nMissCount) / g.nTypeCountRome;
					InsertRanking(hWnd, 1, &time, g.nTypeCountRome * pow(dCorrectAnswerRate, 3.0), g.nTypeCountRome, g.nMissCount);
				}
				SetTimer(hWnd, 1001, 2000, NULL);
			}
			else
			{
				g.nTimeCount--;
			}
			InvalidateRect(hWnd, 0, 0);
		}
		else if (wParam == 1001) // GameEndから来たとき
		{
			KillTimer(hWnd, 1001);
			g.nGameState = GS_GAMESTATUS;
			PlaySound(5);
			InvalidateRect(hWnd, 0, 0);
		}
		else if (wParam == 1002)
		{
			if (g.nCountDown <= 1)
			{
				KillTimer(hWnd, 1002);
				PlaySound(4);
				g.start();
				SetTimer(hWnd, 1000, 1000, NULL);
			}
			else
			{
				PlaySound(3);
				g.nCountDown--;
			}
			InvalidateRect(hWnd, 0, 0);
		}
		break;
	case WM_SIZE:
		if (m_pRenderTarget)
		{
			D2D1_SIZE_U size = { LOWORD(lParam), HIWORD(lParam) };
			m_pRenderTarget->Resize(size);
		}
		if (m_pDWriteFactory)
		{
			float h = HIWORD(lParam);
			float baseSize = h * 0.055f;
			if (baseSize < 16.0f) baseSize = 16.0f;
			float mediumSize = baseSize * 0.56f;
			float smallSize = baseSize * 0.35f;
			SafeRelease(&m_pTextFormat);
			SafeRelease(&m_pMediumTextFormat);
			SafeRelease(&m_pSmallTextFormat);
			IDWriteFontCollection* pCollection = nullptr;
			if (pFontCollection) pCollection = pFontCollection;
			m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, baseSize, L"", &m_pTextFormat);
			m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, mediumSize, L"", &m_pMediumTextFormat);
			m_pDWriteFactory->CreateTextFormat(szFontFamilyName, pCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, smallSize, L"", &m_pSmallTextFormat);
		}
		break;
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, 0, 0);
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		{
			HRESULT hr = S_OK;
			if (!m_pRenderTarget)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
				hr = m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(), DEFAULT_DPI, DEFAULT_DPI, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);
				if (FAILED(hr))
				{
					hr = m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE, D2D1::PixelFormat(), DEFAULT_DPI, DEFAULT_DPI, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);
				}
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DimGray), &m_pBlackBrush);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pGrayBrush);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DodgerBlue), &m_pBlueBrush);
				if (SUCCEEDED(hr))
					hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LimeGreen), &m_pGreenBrush);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();
				m_pRenderTarget->BeginDraw();
				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
				if (g.nGameState == GAME_STATE::GS_GAMESTART)
				{
					if (0)
					{
						// スコアを表示
						WCHAR szText[1024];
						wsprintf(szText, L"%d秒　%d打鍵　%dミス", g.nTimeCount, g.nTypeCountRome, g.nMissCount);
						DWRITE_TEXT_METRICS tTextMetrics;
						{
							IDWriteTextLayout* pTextLayout = NULL;
							hr = m_pDWriteFactory->CreateTextLayout(
								szText
								, lstrlenW(szText)
								, m_pTextFormat
								, renderTargetSize.width
								, renderTargetSize.height
								, &pTextLayout
							);
							pTextLayout->GetMetrics(&tTextMetrics);
							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(0.0F, 0.0F),
								pTextLayout,
								m_pBlackBrush,
								D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
					}
					{// 問題文を表示
						LPCWSTR lpszText = g.list[g.nQuestionCount % g.list.size()].words.c_str();
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
							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
								, renderTargetSize.height / 2 - tTextMetrics.height / 2 - 2 * tTextMetrics.height),
								pTextLayout,
								m_pBlackBrush,
								D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
					}
					{// カタカナを表示
						LPCWSTR lpszText = g.list[g.nQuestionCount % g.list.size()].kana.c_str();
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
							DWRITE_TEXT_RANGE textRange = { 0, (UINT32)g.nCursorKana };
							pTextLayout->SetDrawingEffect(m_pGrayBrush, textRange);
							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
								, renderTargetSize.height / 2 - tTextMetrics.height / 2),
								pTextLayout,
								m_pBlackBrush,
								D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
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
							DWRITE_TEXT_RANGE textRange = { 0, (UINT32)g.nCursorRome };
							pTextLayout->SetDrawingEffect(m_pGrayBrush, textRange);
							if (g.bMiss)
							{
								DWRITE_TEXT_RANGE textRange = { (UINT32)g.nCursorRome, 1 };
								pTextLayout->SetDrawingEffect(m_pRedBrush, textRange);
							}
							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
								, renderTargetSize.height / 2 - tTextMetrics.height / 2 + tTextMetrics.height),
								pTextLayout,
								m_pBlackBrush,
								D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
					}
				}
				else if (g.nGameState == GAME_STATE::GS_GAMEEND)
				{
					LPCWSTR lpszText = L"終了";
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
						m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
							, renderTargetSize.height / 2 - tTextMetrics.height / 2),
							pTextLayout,
							m_pBlackBrush,
							D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
						SafeRelease(&pTextLayout);
					}
				}
				else if (g.nGameState == GAME_STATE::GS_GAMESTATUS)
				{
					// スコアを表示
					WCHAR szText[1024];
					double dCorrectAnswerRate = 1.0 * (g.nTypeCountRome - g.nMissCount) / g.nTypeCountRome;
					swprintf_s(szText, L"[結果]\r\nスコア：%.0f\r\n平均タイプ数：%.2f(回/秒)\r\n正確性：%.2f％\r\n打鍵数：%d回\r\nミスタイプ数：%d回\r\n\r\n[SPACE]キーでタイトルへ\r\n[R]キーでランキング\r\n[X]キーで終了",
						g.nTypeCountRome * pow(dCorrectAnswerRate, 3.0), g.nTypeCountRome / 60.0, 100.0 * dCorrectAnswerRate, g.nTypeCountRome, g.nMissCount);
					DWRITE_TEXT_METRICS tTextMetrics;
					{
						IDWriteTextLayout* pTextLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(
							szText
							, lstrlenW(szText)
							, m_pTextFormat
							, renderTargetSize.width
							, renderTargetSize.height
							, &pTextLayout
						);
						pTextLayout->GetMetrics(&tTextMetrics);
						m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
							, renderTargetSize.height / 2 - tTextMetrics.height / 2),
							pTextLayout,
							m_pBlackBrush,
							D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
						SafeRelease(&pTextLayout);
					}
				}
				else if (g.nGameState == GAME_STATE::GS_TITLE)
				{
					LPCWSTR lpszText = L"[SPACE]キーで開始\r\n[R]キーでランキング\r\n[X]キーで終了";
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

						// --- ここからキーボード描画追加 ---
						float kbWidth = renderTargetSize.width * 0.25f;
						if (kbWidth < 300.0f) kbWidth = 300.0f;
						float kbHeight = kbWidth * 0.35f; // アスペクト比を固定(1 : 0.35)

						// 各種マージンや枠線の太さも幅に比例させる
						float padding = kbWidth * 0.0125f;   // キーボード外枠の余白
						float keyMargin = kbWidth * 0.0075f; // キー同士の隙間
						float kbStroke = kbWidth * 0.0025f;  // キーボード外枠の線の太さ
						float keyStroke = kbWidth * 0.00125f;// キーの線の太さ

						float kbX = renderTargetSize.width / 2.0f - kbWidth / 2.0f;
						// テキストや画面サイズに合わせてY座標も動的に隙間を空ける
						float kbY = renderTargetSize.height / 2.0f - kbHeight / 2.0f - tTextMetrics.height / 2.0f - (renderTargetSize.height * 0.03f);

						// キーボードベース（黒系）
						D2D1_RECT_F kbRect = D2D1::RectF(kbX, kbY, kbX + kbWidth, kbY + kbHeight);
						m_pRenderTarget->FillRectangle(&kbRect, m_pBlackBrush);
						m_pRenderTarget->DrawRectangle(&kbRect, m_pBlackBrush, kbStroke);

						// 各行のキーの相対幅 (単位: 1U = 通常キー1個分)
						float rowWidths[5][15] = {
							{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.0f },
							{ 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 0.0f },
							{ 1.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.25f, 0.0f, 0.0f },
							{ 2.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.75f, 0.0f, 0.0f, 0.0f },
							{ 1.25f, 1.25f, 1.25f, 6.25f, 1.25f, 1.25f, 1.25f, 1.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
						};

						int rows = 5;

						// 内側の有効描画エリア
						float innerWidth = kbWidth - padding * 2.0f;
						float innerHeight = kbHeight - padding * 2.0f;
						float rowHeight = innerHeight / rows;

						for (int r = 0; r < rows; r++) {
							float currentX = kbX + padding;
							float currentY = kbY + padding + r * rowHeight;

							for (int c = 0; c < 15; c++) {
								float u = rowWidths[r][c];
								if (u == 0.0f) break;

								float cellWidth = innerWidth * (u / 15.0f);

								D2D1_RECT_F keyRect = D2D1::RectF(
									currentX + keyMargin / 2.0f,
									currentY + keyMargin / 2.0f,
									currentX + cellWidth - keyMargin / 2.0f,
									currentY + rowHeight - keyMargin / 2.0f
								);

								m_pRenderTarget->FillRectangle(&keyRect, m_pGrayBrush);
								m_pRenderTarget->DrawRectangle(&keyRect, m_pBlackBrush, keyStroke);

								currentX += cellWidth;
							}
						}

						// --- ここまでキーボード描画 ---

						// 既存のテキストの描画位置調整（キーボードからの距離も動的に）
						float textY = kbY + kbHeight + (renderTargetSize.height * 0.03f);

						m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
							, textY),
							pTextLayout,
							m_pBlackBrush,
							D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);

						SafeRelease(&pTextLayout);
					}
				}
				else if (g.nGameState == GAME_STATE::GS_COUNTDOWN)
				{
					if (g.nCountDown <= 3)
					{
						WCHAR szText[256];
						wsprintf(szText, L"%d", g.nCountDown);
						DWRITE_TEXT_METRICS tTextMetrics;
						{
							IDWriteTextLayout* pTextLayout = NULL;
							hr = m_pDWriteFactory->CreateTextLayout(
								szText
								, lstrlenW(szText)
								, m_pTextFormat
								, renderTargetSize.width
								, renderTargetSize.height
								, &pTextLayout
							);
							pTextLayout->GetMetrics(&tTextMetrics);
							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2 - tTextMetrics.width / 2
								, renderTargetSize.height / 2 - tTextMetrics.height / 2),
								pTextLayout,
								m_pBlackBrush,
								D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
					}
				}
				else if (g.nGameState == GAME_STATE::GS_RANKING)
				{
					DWRITE_TEXT_METRICS tTextMetrics;
					float halfHeight = renderTargetSize.height / 2.0f;
					
					// 1. テキストランキングを上部に描画
					{
						IDWriteTextLayout* pTextLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(
							ranking.c_str(), (UINT32)ranking.length(), m_pMediumTextFormat,
							renderTargetSize.width, renderTargetSize.height, &pTextLayout); // ← halfHeight - 20.0f から変更
						if (pTextLayout != nullptr) {
							pTextLayout->GetMetrics(&tTextMetrics);
							m_pRenderTarget->DrawTextLayout(
								D2D1::Point2F(renderTargetSize.width / 2.0f - tTextMetrics.width / 2.0f, 20.0f),
								pTextLayout, m_pBlackBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pTextLayout);
						}
					}

					// 2. 過去5年分のグラフを下部に描画
					if (rankingHistory.size() >= 2)
					{
						// 最も幅を取る「100%」の文字サイズを計測して余白を決定
						float maxLabelWidth = 50.0f;
						float labelHeight = 20.0f;
						IDWriteTextLayout* pDummyLayout = NULL;
						if (SUCCEEDED(m_pDWriteFactory->CreateTextLayout(L"100%", 4, m_pSmallTextFormat, 1000.0f, 100.0f, &pDummyLayout))) {
							DWRITE_TEXT_METRICS tm;
							pDummyLayout->GetMetrics(&tm);
							maxLabelWidth = tm.width;
							labelHeight = tm.height;
							SafeRelease(&pDummyLayout);
						}

						// 文字の高さ(labelHeight)に比例した動的なマージンを計算
						float margin = labelHeight * 0.5f;

						// 左側の余白をしっかり確保（文字幅 + マージン2つ分）
						float chartX = maxLabelWidth + margin * 2.0f;
						float chartY = 20.0f + tTextMetrics.height + margin;
						float chartW = renderTargetSize.width - chartX - margin * 2.0f;

						// --- ここから変更 ---
						// 下部の余白: 年号テキスト分 + 凡例テキスト分 + 隙間
						float bottomPadding = labelHeight * 2.5f + margin * 2.0f;
						float chartH = renderTargetSize.height - chartY - bottomPadding;
						// --- ここまで変更 ---

						if (chartH < 50.0f) chartH = 50.0f;
						double minDate = rankingHistory[0].date;
						double maxDate = rankingHistory[rankingHistory.size() - 1].date;
						if (maxDate == minDate) maxDate = minDate + 1.0;
						
						double maxScore = 1.0;
						for (auto& item : rankingHistory) {
							if (item.score > maxScore) maxScore = item.score;
						}

						// X,Y軸
						m_pRenderTarget->DrawLine(D2D1::Point2F(chartX, chartY), D2D1::Point2F(chartX, chartY + chartH), m_pBlackBrush, 2.0f);
						m_pRenderTarget->DrawLine(D2D1::Point2F(chartX, chartY + chartH), D2D1::Point2F(chartX + chartW, chartY + chartH), m_pBlackBrush, 2.0f);

						// Y軸グリッド(10%ごとの正答率目安線)
						IDWriteTextLayout* pLabelLayout = NULL;
						for (int pct = 50; pct <= 100; pct += 10) { // 0から50スタートに変更
							float yGrid = chartY + chartH - (float)((pct - 50) / 50.0) * chartH; // 計算式を変更
							if (pct > 50 && pct < 100) { // 0より大きいの条件を50より大きいへ変更
								m_pRenderTarget->DrawLine(D2D1::Point2F(chartX, yGrid), D2D1::Point2F(chartX + chartW, yGrid), m_pGrayBrush, 0.5f);
							}
							WCHAR szLbl[32];
							wsprintf(szLbl, L"%d%%", pct);

							// 枠を十分に大きくして改行を防ぐ
							if (SUCCEEDED(m_pDWriteFactory->CreateTextLayout(szLbl, lstrlenW(szLbl), m_pSmallTextFormat, 1000.0f, 100.0f, &pLabelLayout))) {
								pLabelLayout->GetMetrics(&tTextMetrics);

								// グラフのY軸から margin 分だけ左に離して配置する
								float labelX = chartX - tTextMetrics.width - margin;
								float labelY = yGrid - tTextMetrics.height / 2.0f;

								m_pRenderTarget->DrawTextLayout(D2D1::Point2F(labelX, labelY), pLabelLayout, m_pGrayBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
								SafeRelease(&pLabelLayout);
							}
						}

						// X軸グリッド(年の切り替わり)
						int lastYear = -1;
						for (size_t i = 0; i < rankingHistory.size(); i++) {
							SYSTEMTIME st;
							VariantTimeToSystemTime(rankingHistory[i].date, &st);
							if (st.wYear != lastYear) {
								lastYear = st.wYear;
								float xYear = chartX + (float)((rankingHistory[i].date - minDate) / (maxDate - minDate)) * chartW;
								m_pRenderTarget->DrawLine(D2D1::Point2F(xYear, chartY), D2D1::Point2F(xYear, chartY + chartH), m_pGrayBrush, 0.5f);

								WCHAR szYear[32];
								wsprintf(szYear, L"%d年", st.wYear);

								if (SUCCEEDED(m_pDWriteFactory->CreateTextLayout(szYear, lstrlenW(szYear), m_pSmallTextFormat, 1000.0f, 100.0f, &pLabelLayout))) {
									pLabelLayout->GetMetrics(&tTextMetrics);

									float labelX = xYear - tTextMetrics.width / 2.0f;
									// X軸のすぐ下に配置
									float labelY = chartY + chartH + (margin * 0.5f);

									m_pRenderTarget->DrawTextLayout(D2D1::Point2F(labelX, labelY), pLabelLayout, m_pGrayBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
									SafeRelease(&pLabelLayout);
								}
							}
						}

						// 折れ線描画
						double minScore = maxScore / 2.0;

						// グラフ範囲外にはみ出さないよう、0.0〜1.0に丸めるヘルパー関数
						auto getClampRatio = [](double val, double min_val, double max_val) {
							if (max_val <= min_val) return 0.0;
							double ratio = (val - min_val) / (max_val - min_val);
							if (ratio < 0.0) return 0.0;
							if (ratio > 1.0) return 1.0;
							return ratio;
							};

						for (size_t i = 0; i < rankingHistory.size() - 1; i++) {
							const auto& p1 = rankingHistory[i];
							const auto& p2 = rankingHistory[i + 1];

							float x1 = chartX + (float)((p1.date - minDate) / (maxDate - minDate)) * chartW;
							float x2 = chartX + (float)((p2.date - minDate) / (maxDate - minDate)) * chartW;

							// minScore 〜 maxScore で計算
							float y1Score = chartY + chartH - (float)getClampRatio(p1.score, minScore, maxScore) * chartH;
							float y2Score = chartY + chartH - (float)getClampRatio(p2.score, minScore, maxScore) * chartH;

							// 50.0 〜 100.0 で計算
							float y1Acc = chartY + chartH - (float)getClampRatio(p1.accuracy, 50.0, 100.0) * chartH;
							float y2Acc = chartY + chartH - (float)getClampRatio(p2.accuracy, 50.0, 100.0) * chartH;

							m_pRenderTarget->DrawLine(D2D1::Point2F(x1, y1Score), D2D1::Point2F(x2, y2Score), m_pBlueBrush, 1.5f);
							m_pRenderTarget->DrawLine(D2D1::Point2F(x1, y1Acc), D2D1::Point2F(x2, y2Acc), m_pGreenBrush, 1.5f);
						}

						// ホバーツールチップ
						int hoveredIndex = -1;
						float minHoverDist = 10000.0f;
						for (size_t i = 0; i < rankingHistory.size(); i++) {
							float px = chartX + (float)((rankingHistory[i].date - minDate) / (maxDate - minDate)) * chartW;
							float pyAcc = chartY + chartH - (float)getClampRatio(rankingHistory[i].accuracy, 50.0, 100.0) * chartH;
							float pyScore = chartY + chartH - (float)getClampRatio(rankingHistory[i].score, minScore, maxScore) * chartH;

							float distAccSq = (px - ptMouse.x) * (px - ptMouse.x) + (pyAcc - ptMouse.y) * (pyAcc - ptMouse.y);
							float distScoreSq = (px - ptMouse.x) * (px - ptMouse.x) + (pyScore - ptMouse.y) * (pyScore - ptMouse.y);

							float distSq = distAccSq < distScoreSq ? distAccSq : distScoreSq;
							if (distSq < 100.0f && distSq < minHoverDist) {
								hoveredIndex = (int)i;
								minHoverDist = distSq;
							}
						}

						if (hoveredIndex != -1) {
							SYSTEMTIME st;
							VariantTimeToSystemTime(rankingHistory[hoveredIndex].date, &st);
							WCHAR szTooltip[256];
							swprintf(szTooltip, L"%04d/%02d/%02d\nポイント: %d\n正答率: %.1f%%", st.wYear, st.wMonth, st.wDay, (int)rankingHistory[hoveredIndex].score, rankingHistory[hoveredIndex].accuracy);
							
							IDWriteTextLayout* pTooltipLayout = NULL;
							if (SUCCEEDED(m_pDWriteFactory->CreateTextLayout(szTooltip, lstrlenW(szTooltip), m_pSmallTextFormat, 300, 100, &pTooltipLayout))) {
								pTooltipLayout->GetMetrics(&tTextMetrics);

								// 基本はマウスカーソルの左側に配置する
								float tipX = (float)ptMouse.x - 15.0f - tTextMetrics.width;
								float tipY = (float)ptMouse.y + 15.0f;

								// 左端ではみ出す場合は、カーソルの右側に配置する
								if (tipX - 5.0f < 0.0f) {
									tipX = (float)ptMouse.x + 15.0f;
								}

								// 下端ではみ出す場合は、カーソルの上側に配置する
								if (tipY + tTextMetrics.height + 5.0f > renderTargetSize.height) {
									tipY = (float)ptMouse.y - 15.0f - tTextMetrics.height;
								}

								D2D1_RECT_F bgRect = D2D1::RectF(tipX - 5.0f, tipY - 5.0f, tipX + tTextMetrics.width + 5.0f, tipY + tTextMetrics.height + 5.0f);								ID2D1SolidColorBrush* pBgBrush = NULL;
								m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.8f), &pBgBrush);
								if (pBgBrush) {
									m_pRenderTarget->FillRectangle(&bgRect, pBgBrush);
									SafeRelease(&pBgBrush);
								}
								
								ID2D1SolidColorBrush* pWhiteBrush = NULL;
								m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &pWhiteBrush);
								if (pWhiteBrush) {
									m_pRenderTarget->DrawTextLayout(D2D1::Point2F(tipX, tipY), pTooltipLayout, pWhiteBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
									SafeRelease(&pWhiteBrush);
								}
								SafeRelease(&pTooltipLayout);
							}
						}

						// 凡例
						std::wstring legend = L"■ 獲得ポイント (Max: " + std::to_wstring((int)maxScore) + L")  ■ 正答率 (推移)";
						IDWriteTextLayout* pLegendLayout = NULL;
						hr = m_pDWriteFactory->CreateTextLayout(legend.c_str(), (UINT32)legend.length(), m_pSmallTextFormat, chartW, 100.0f, &pLegendLayout);
						if (pLegendLayout) {
							DWRITE_TEXT_RANGE rangeBlue = { 0, 1 };
							size_t greenPos = legend.find(L"■", 1);
							DWRITE_TEXT_RANGE rangeGreen = { (UINT32)greenPos, 1 };

							pLegendLayout->SetDrawingEffect(m_pBlueBrush, rangeBlue);
							if (greenPos != std::wstring::npos) pLegendLayout->SetDrawingEffect(m_pGreenBrush, rangeGreen);

							pLegendLayout->GetMetrics(&tTextMetrics);

							// --- ここから変更 ---
							// ウィンドウの下端から上にオフセットして配置（見切れを確実に防ぐ）
							float legendY = renderTargetSize.height - tTextMetrics.height - margin;
							// --- ここまで変更 ---

							m_pRenderTarget->DrawTextLayout(D2D1::Point2F(renderTargetSize.width / 2.0f - tTextMetrics.width / 2.0f, legendY),
								pLegendLayout, m_pBlackBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
							SafeRelease(&pLegendLayout);
						}
					}
				}
				hr = m_pRenderTarget->EndDraw();
				if (hr == D2DERR_RECREATE_TARGET)
				{
					SafeRelease(&m_pRenderTarget);
					SafeRelease(&m_pBlackBrush);
					SafeRelease(&m_pGrayBrush);
					SafeRelease(&m_pRedBrush);
					SafeRelease(&m_pBlueBrush);
					SafeRelease(&m_pGreenBrush);
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
				g.nTypeCountRome++;
				if (g.szQuestionKana[g.nCursorKana] == L'ン' && g.szQuestionKana[g.nCursorKana] != L'\0' &&
					g.szInputRome[0] == L'N' && g.szInputRome[1] == L'\0')
				{
					g.nCursorKana++;
					g.nCursorRome++;
					g.nTypeCountKana++;
					g.szInputRome[0] = g.szInputRome[1];
					g.szInputRome[1] = 0;
				}
				if (JudgeFromKigou((DWORD)wParam, g.szQuestionKana, g.nCursorKana))
				{
					g.bMiss = FALSE;
					g.nCursorKana++;
					g.nCursorRome++;
					g.nTypeCountKana++;
					PlaySound(0);
					if (g.szQuestionKana[g.nCursorKana] == L'\0')
					{
						g.next();
					}
				}
				else
				{
					g.bMiss = TRUE;
					g.nMissCount++;
					PlaySound(1);
					g.szInputRome[lstrlen(g.szInputRome) - 1] = 0;
				}
				InvalidateRect(hWnd, 0, 0);
			}
			else if (IsAlphabet((WCHAR)wParam))
			{
				WCHAR szInput[2];
				wsprintf(szInput, L"%c", wParam);
				lstrcat(g.szInputRome, szInput);
				debuglog(L"szInputRome = %s\r\n", g.szInputRome);
				g.nTypeCountRome++;
				if (g.szQuestionKana[g.nCursorKana] == g.szInputRome[0]) // 英字
				{
					g.bMiss = FALSE;
					g.nCursorKana++;
					g.nCursorRome++;
					g.nTypeCountKana++;
					g.szInputRome[0] = 0;
					PlaySound(0);
					if (g.szQuestionKana[g.nCursorKana] == L'\0')
					{
						g.next();
					}
				}
				else
				{
					if (g.szQuestionKana[g.nCursorKana] == L'ン' && g.szQuestionKana[g.nCursorKana] != L'\0' &&
						lstrlen(g.szInputRome) == 2 && g.szInputRome[0] == L'N' && g.szInputRome[1] != L'N' && g.szInputRome[1] != L'Y' && IsBoin(g.szInputRome[1]) == FALSE)
					{
						// ンは2文字目に関係なく確定
						g.nCursorKana++;
						g.nCursorRome++;
						g.nTypeCountKana++;
						g.szInputRome[0] = g.szInputRome[1];
						g.szInputRome[1] = 0;
						debuglog(L"szInputRome(after) = %s\r\n", g.szInputRome);
					}
					WCHAR szOutputKana[4] = {};
					WCHAR szOutputRome[5] = {};
					BOOL bRet = JudgeFromKana(g.szInputRome, g.szQuestionKana, g.nCursorKana, szOutputKana, szOutputRome);
					if (bRet)
					{
						g.bMiss = FALSE;
						if (g.szQuestionRome[g.nCursorRome] != szInput[0])
						{
							lstrcpy(&(g.szQuestionRome[g.nCursorRome - lstrlen(g.szInputRome) + 1]), szOutputRome);
							lstrcat(g.szQuestionRome, g.GetRomeFromKana(&(g.szQuestionKana[g.nCursorKana + lstrlen(szOutputKana)])));
						}
						g.nCursorRome++;
						PlaySound(0);
						debuglog(L"szOutput = %s\r\n", szOutputKana);
						if (L'A' == wParam || L'I' == wParam || L'U' == wParam || L'E' == wParam || L'O' == wParam || (g.szInputRome[0] == L'N' && g.szInputRome[1] == L'N'))
						{
							// 母音またはNを2回入力した場合は、ローマ字入力を確定させ次の文字に移る。
							g.nCursorKana += lstrlen(szOutputKana);
							g.nTypeCountKana += lstrlen(szOutputKana);
							g.szInputRome[0] = 0;
							if (g.szQuestionKana[g.nCursorKana] == L'\0')
							{
								g.next();
							}
						}
					}
					else
					{
						g.bMiss = TRUE;
						g.nMissCount++;
						PlaySound(1);
						g.szInputRome[lstrlen(g.szInputRome) - 1] = 0;
					}
				}
				InvalidateRect(hWnd, 0, 0);
			}
			else if (wParam == VK_ESCAPE)
			{
				KillTimer(hWnd, 1000);
				g.nGameState = GS_TITLE;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
		}
		else if (g.nGameState == GS_GAMEEND)
		{
			if (wParam == VK_SPACE)
			{
				KillTimer(hWnd, 1001);
				g.nGameState = GS_GAMESTATUS;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
		}
		else if (g.nGameState == GS_GAMESTATUS)
		{
			if (wParam == VK_SPACE)
			{
				g.nGameState = GS_TITLE;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
			else if (wParam == 'R')
			{
				GetRankingFromDatabase(ranking, 1);
				GetRankingHistoryFromDatabase(rankingHistory, 1);
				g.nGameState = GS_RANKING;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
			else if (wParam == 'X')
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		else if (g.nGameState == GAME_STATE::GS_TITLE)
		{
			if (wParam == VK_SPACE)
			{
				g.nCountDown = 4;
				g.nGameState = GS_COUNTDOWN;
				SetTimer(hWnd, 1002, 1000, NULL);
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
			else if (wParam == 'R')
			{
				GetRankingFromDatabase(ranking, 1);
				GetRankingHistoryFromDatabase(rankingHistory, 1);
				g.nGameState = GS_RANKING;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
			else if (wParam == 'X')
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		else if (g.nGameState == GAME_STATE::GS_RANKING)
		{
			if (wParam == VK_SPACE)
			{
				g.nGameState = GS_TITLE;
				PlaySound(5);
				InvalidateRect(hWnd, 0, 0);
			}
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
		break;
	case WM_DPICHANGED:
		SendMessage(hWnd, WM_APP, 0, 0);
		break;
	case WM_DESTROY:
		SafeRelease(&pFontSet);
		SafeRelease(&fontSetBuilder);
		SafeRelease(&pFontCollection);
		SafeRelease(&pFontFile);
		SafeRelease(&pInMemoryFontFileLoader);
		SafeRelease(&m_pD2DFactory);
		SafeRelease(&m_pDWriteFactory);
		SafeRelease(&m_pRenderTarget);
		SafeRelease(&m_pTextFormat);
		SafeRelease(&m_pMediumTextFormat); // 追加
		SafeRelease(&m_pSmallTextFormat);  // 追加
		SafeRelease(&m_pBlackBrush);
		SafeRelease(&m_pGrayBrush);
		SafeRelease(&m_pRedBrush);
		SafeRelease(&m_pWICFactory);
		CoUninitialize();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
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
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szWindowClass
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	if (!hWnd) return FALSE;
	DSBUFFERDESC dsbd = {};
	IDirectSound* ds[MAX_SOUND_COUNT];
	for (int i = 0; i < MAX_SOUND_COUNT; i++)
	{
		ds[i] = nullptr;
		dsb1[i] = nullptr;
		if (SUCCEEDED(DirectSoundCreate(NULL, &(ds[i]), NULL)))
		{
			(ds[i])->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		HRSRC hrsrc = FindResource(hInstance, MAKEINTRESOURCE(nSoundResourceID[i]), TEXT("WAVE"));
		if (hrsrc)
		{
			HGLOBAL hRData = LoadResource(hInstance, hrsrc);
			if (hRData)
			{
				DWORD* pRData = (DWORD*)LockResource(hRData);
				if (pRData)
				{
					LPBYTE pMem1, pMem2;
					DWORD dwSize1, dwSize2;
					dsbd.dwBufferBytes = *(pRData + 10);
					dsbd.lpwfxFormat = (LPWAVEFORMATEX)(pRData + 5);
					if (SUCCEEDED((ds[i])->CreateSoundBuffer(&dsbd, &(dsb1[i]), NULL)))
					{
						if (SUCCEEDED((dsb1[i])->Lock(0, dsbd.dwBufferBytes, (void**)&pMem1, &dwSize1, (void**)&pMem2, &dwSize2, 0)))
						{
							memcpy(pMem1, (LPBYTE)(pRData + 11), dwSize1);
							if (dwSize2 != 0) memcpy(pMem2, (LPBYTE)(pRData + 11) + dwSize1, dwSize2);
							(dsb1[i])->Unlock(pMem1, dwSize1, pMem2, dwSize2);
						}
					}
				}
			}
		}
		}
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
		if (i) i->Release();
	}
	return (int)msg.wParam;
}
