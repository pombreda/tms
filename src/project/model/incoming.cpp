
#include "incoming.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/proxy_field.hpp>
// project
#include <project/model/company.hpp>
#include <project/model/contact_person.hpp>
#include <model/user.hpp>
// boost 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/lexical_cast.hpp>

using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace tms::common::model;
using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace boost;

string Incoming::GenerateID() {
  ptime now = second_clock::local_time();
  ContraptionArrayP contraptions = Incoming::GetModel()->All();
  time_facet *facet = new time_facet("%y");
  ostringstream sout;
  sout.imbue(locale(cout.getloc(), facet));
  sout << now;
  std::string year = sout.str();
  
  std::string id ="0";
  if (contraptions->size() > 0) {
    int p = contraptions->size() - 1;
    while (p >= 0 && contraptions->at(p)->Get<std::string>("ID") == "") {
      --p;
    }
    if (p >= 0) {
      ContraptionP last = contraptions->at(p);
      std::string last_id = last->Get<std::string>("ID");
      size_t found = last_id.find(" /Вх-");
      if (found != std::string::npos) {
        try {
          id = lexical_cast<std::string>(1 + lexical_cast<int>(last_id.substr(0, found)));
        } catch (...) {
        }
      }
    }
  }
  
  return id + " /Вх-" + year;    
}

void Incoming::Initialize() {
  vector<Field*> ret;
  ret.push_back(new StringField("ID"));
  ret.push_back(new StringField("received_at"));
  ret.push_back(new StringField("reception_type"));
  ret.push_back(new StringField("passed_at"));
  ret.push_back(new StringField("subject"));
  

  IntField *company_id = new IntField("company_id");
  ret.push_back(company_id);

  HasOneField* company
    = new HasOneField("company", 
		       boost::ref(*Company::GetModel()),
		       company_id);

  ret.push_back(company);
  ret.push_back(new ProxyField<std::string>("company_name",
					    company,
					    "short_name"));

  IntField *contact_person_id = new IntField("contact_person_id");
  ret.push_back(contact_person_id);
  HasOneField* contact_person 
      = new HasOneField("contact_person", 
                         boost::ref(*ContactPerson::GetModel()),
                         contact_person_id);

  ret.push_back(contact_person);
  ret.push_back(new ProxyField<std::string>("contact_name",
					    contact_person,
					    "name"));
  ret.push_back(new ProxyField<std::string>("email",
					    contact_person,
					    "email"));
  ret.push_back(new ProxyField<std::string>("phone",
					    contact_person,
					    "phone"));

  ret.push_back(new StringField("addressee"));
  InitFields(ret);
}

