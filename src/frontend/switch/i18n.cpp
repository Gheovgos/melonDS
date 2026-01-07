#include "i18n.h"

#include <switch.h>
#include <string>
#include <unordered_map>
#include <cstdio>
#include <cstring>

#define I18N_PATH "/switch/melonDS/lang/"

namespace i18n {

static std::unordered_map<std::string, std::string> translations;
static int localizedFontType = -1;

// Load language file from /switch/melonDS/lang/{langCode}.json
void loadLanguage(const char* langCode) {
    translations.clear();

    char path[48];
    snprintf(path, sizeof(path), "%s%s.json", I18N_PATH, langCode);

    FILE* f = fopen(path, "r");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = new char[size + 1];
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);

    char* p = buf;
    while (*p && *p != '{') p++;
    if (*p) p++;

    while (*p) {
        while (*p && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t' || *p == ',')) p++;
        if (*p == '}' || !*p) break;

        if (*p++ != '"') break;
        char* keyStart = p;
        while (*p && *p != '"') p++;
        std::string key(keyStart, p - keyStart);
        if (*p) p++;

        while (*p && *p != ':') p++;
        if (*p) p++;
        while (*p && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')) p++;

        if (*p++ != '"') break;
        char* valStart = p;
        while (*p && *p != '"') p++;
        std::string val(valStart, p - valStart);
        if (*p) p++;

        translations[key] = val;
    }

    delete[] buf;
}

// Get translated text, returns original key if not found
const char* get(const char* key) {
    auto it = translations.find(key);
    if (it != translations.end()) {
        return it->second.c_str();
    }
    return key;
}

// Get Switch system language code (e.g. "zh_CN", "ja", "en")
const char* getSystemLanguage() {
    u64 languageCode;
    SetLanguage setLanguage;

    if (R_FAILED(setGetSystemLanguage(&languageCode))) {
        return "en";
    }
    if (R_FAILED(setMakeLanguage(languageCode, &setLanguage))) {
        return "en";
    }

    switch (setLanguage) {
        case SetLanguage_ZHCN:
        case SetLanguage_ZHHANS:
            localizedFontType = PlSharedFontType_ChineseSimplified;
            return "zh_CN";
        case SetLanguage_ZHTW:
        case SetLanguage_ZHHANT:
            localizedFontType = PlSharedFontType_ChineseSimplified;
            return "zh_TW";
        case SetLanguage_JA:
            return "ja";
        case SetLanguage_KO:
            localizedFontType = PlSharedFontType_KO;
            return "ko";
        case SetLanguage_FR:
        case SetLanguage_FRCA:
            return "fr";
        case SetLanguage_DE:
            return "de";
        case SetLanguage_IT:
            return "it";
        case SetLanguage_ES:
        case SetLanguage_ES419:
            return "es";
        case SetLanguage_PT:
        case SetLanguage_PTBR:
            return "pt";
        case SetLanguage_RU:
            return "ru";
        case SetLanguage_NL:
            return "nl";
        default:
            return "en";
    }
}

// Get localized font type, returns -1 if no extra font needed
int getLocalizedFontType() {
    return localizedFontType;
}

}
