#ifndef I18N_H
#define I18N_H

namespace i18n {

const char* getSystemLanguage();
void loadLanguage(const char* langCode);
const char* get(const char* key);
int getLocalizedFontType();

}

#endif
