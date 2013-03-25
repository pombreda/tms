#include "frm_grid.hpp"
// wx
#include <wx/filedlg.h>
// std
#include <fstream>
#include <sstream>
// common
#include <string/string.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
// project
#include <project/model/incoming.hpp>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

// oops
#include <iostream>
namespace tms {
namespace client {
using namespace common::string;
using namespace common::contraption;
using namespace project::model;
using namespace boost;
using namespace gregorian;
using namespace posix_time;

void FrmGrid::OnImportIncomingClick() {
  wxFileDialog *fd = new wxFileDialog(this, wxString::FromUTF8("Хрен вам, а не импорт"), "", "", "*.csv");

  if (fd->ShowModal() == wxID_OK) {
    std::string path = fd->GetPath().utf8_str().data();
    fstream fin(path.c_str());
    std::string fline;
    // Skip 2 lines of header
    getline(fin, fline);
    getline(fin, fline);

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
      }
      ContraptionP incoming = Incoming::GetModel()->New();
      // 0.ID
      try {
        incoming->Set<std::string>("ID", fields[0]);
      } catch (...) {
      }
      // 1.ReceivedAt
      try {
        ptime received_at;
        time_input_facet *ifacet = new time_input_facet("%d.%m.%y %H:%M");
        time_facet *ofacet = new time_facet("%Y-%m-%d %H:%M");
        istringstream sin(fields[1]);
        ostringstream sout;
        sin.imbue(locale(std::locale::classic(), ifacet));
        sout.imbue(locale(std::locale::classic(), ofacet));
        sin >> received_at;
        sout << received_at;
        incoming->Set<std::string>("received_at", sout.str());
      } catch (...) {
      }
      // 2.ReceptionType
      try {
        if (fields[2] == "э") {
          incoming->Set<std::string>("reception_type", "e-mail");
        } else if (fields[2] == "ф") {
          incoming->Set<std::string>("reception_type", "факс");
        } else if (fields[2] == "п") {
          incoming->Set<std::string>("reception_type", "почта");
        } else if (fields[2] == "т") {
          incoming->Set<std::string>("reception_type", "телефон");
        }
      } catch (...) {
      }
      // 3.CompanyName
      try {
        ModelP company_model = Company::GetModel();
        const SimpleFieldT<string> *short_name
            = dynamic_cast<const StringField*>(
                company_model->GetField("short_name"));

        FilterP filter = Compare(short_name, kEqual, fields[3]);
        ContraptionArrayP companies = company_model->Filter(filter);
        ContraptionP company;
        if (companies->size() == 0) {
          cerr << "No model, creating new" << endl;
          company = Company::GetModel()->New();
          company->Set<std::string>("short_name", fields[3]);
          company->Set<std::string>("ID", Company::GenerateID());
          company->Save();
        } else {
          company = companies->at(0);
        }
        incoming->Set<ContraptionP>("company", company);
      } catch (...) {
      }
      // 5.ContactPerson
      try {
        ModelP contact_person_model = ContactPerson::GetModel();
        const StringField *name
            = dynamic_cast<const StringField*>(
                contact_person_model->GetField("name"));
        const IntField *company_id
            = dynamic_cast<const IntField*>(
                contact_person_model->GetField("company_id"));
        
        ContraptionP company = incoming->Get<ContraptionP>("company");
        FilterP filter = And(
            Compare(name, kEqual, fields[5]),
            Compare(company_id, kEqual, company->Get<int>("id")));
        ContraptionArrayP contact_persons = contact_person_model->Filter(filter);
        ContraptionP contact_person;
        if (contact_persons->size() == 0) {
          cerr << "No cp model, creating new" << endl;
          contact_person = ContactPerson::GetModel()->New();
          contact_person->Set<std::string>("name", fields[5]);
          contact_person->Set<int>("company_id", company->Get<int>("id"));
          contact_person->Save();
        } else {
          contact_person = contact_persons->at(0);
        }
        incoming->Set<ContraptionP>("contact_person", contact_person);
        if (!company->Get<ContraptionP>("contact_person")) {
          company->Set<ContraptionP>("contact_person", contact_person);
          company->Save();
        }
        // 7.CityCode
        if (contact_person->Get<std::string>("city_code") == "") {
          contact_person->Set<std::string>("city_code", fields[7]);
        }
        // 8.Phone
        if (contact_person->Get<std::string>("phone") == "") {
          contact_person->Set<std::string>("phone", fields[8]);
        }
        // 9.Phone
        if (contact_person->Get<std::string>("fax") == "") {
          contact_person->Set<std::string>("fax", fields[9]);
        }
        // 10.E-mail
        if (contact_person->Get<std::string>("email") == "") {
          contact_person->Set<std::string>("email", fields[10]);
        }
        contact_person->Save();
      } catch (...) {
      }
      // 11.Subject
      try {
        incoming->Set<std::string>("subject", fields[11]);
      } catch (...) {
      }

      // 13.PassedAt
      try {
        if (fields[13]!= "") {
          ptime received_at;
          time_input_facet *ifacet = new time_input_facet("%d.%m.%y %H:%M");
          time_facet *ofacet = new time_facet("%Y-%m-%d %H:%M");
          istringstream sin(fields[13]);
          ostringstream sout;
          sin.imbue(locale(std::locale::classic(), ifacet));
          sout.imbue(locale(std::locale::classic(), ofacet));
          sin >> received_at;
          sout << received_at;
          incoming->Set<std::string>("passed_at", sout.str());
        }
      } catch (...) {
      }

      incoming->Save();
      getline(fin, fline);
    }
  }
}
}
}
