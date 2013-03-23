#include "frm_grid.hpp"
// wx
#include <wx/filedlg.h>
// std
#include <fstream>
// common
#include <string/string.hpp>
// oops
#include <iostream>
namespace tms {
namespace client {
using namespace common::string;
void FrmGrid::OnImportIncomingClick() {
  wxFileDialog *fd = new wxFileDialog(this, wxString::FromUTF8("Хрен вам, а не импорт"), "", "", "*.csv");
  if (fd->ShowModal() == wxID_OK) {
    std::string path = fd->GetPath().utf8_str().data();
    fstream fin(path.c_str());
    std::string fline;
    getline(fin, fline);
    while (!fin.eof()) {
      vector<std::string> fields(0);
      std::wstring line = WStringFromCP1251String(fline);
      size_t pos = -1;
      while (pos != line.size()) {
        ++pos;
        size_t beg = pos;
        pos = line.find(L';', pos);
        if (pos == std::wstring::npos) {
          pos = line.size();
        }
        std::wstring token = line.substr(beg, pos - beg);
        fields.push_back(UTF8StringFromWString(token));
        cerr << fields.back() << endl;
      }
      cerr << endl;
      getline(fin, fline);
    }
  }
}
}
}
