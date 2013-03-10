
#include "incoming.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/proxy_field.hpp>
// project
#include <project/model/company.hpp>
#include <project/model/contact_person.hpp>
#include <model/user.hpp>

using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace tms::common::model;
using namespace std;

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

