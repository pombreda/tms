#include "company.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/proxy_field.hpp>
#include <model/user.hpp>
// project
#include <project/model/contact_person.hpp>
// oops
#include <iostream>
// boost
#include <boost/lexical_cast.hpp>

using namespace tms::project::model;
using namespace tms::common::model;
using namespace tms::common::contraption;
using namespace std;
using namespace boost;

string Company::GenerateID () {
  ContraptionArrayP contraptions = Company::GetModel()->All();
  std::string id = "0";
  if (contraptions->size() > 0) {
    int p = contraptions->size() - 1;
      while (p >= 0 && contraptions->at(p)->Get<std::string>("ID") == "") {
	--p;
      }
      if (p >= 0) {
	ContraptionP last = contraptions->at(p);
	std::string last_id = last->Get<std::string>("ID");
        try {
          id = lexical_cast<std::string>(1 + lexical_cast<int>(last_id));
        } catch (...) {
        }
      }
  }
  return id;
}

void Company::Initialize() {
  vector<Field*> ret;
  ret.push_back(new StringField("ID"));
  ret.push_back(new StringField("full_name"));
  ret.push_back(new StringField("short_name"));
  
  IntField *contact_person_id = new IntField("contact_person_id");
  ret.push_back(contact_person_id);
  HasOneField* contact_person_field 
      = new HasOneField("contact_person", 
                        boost::ref(*ContactPerson::GetModel()),
                        _other_id_column = contact_person_id);
  ret.push_back(contact_person_field);
  ret.push_back(new ProxyField<std::string>("contact_name",
					    contact_person_field,
					    "name"));
  ret.push_back(new ProxyField<std::string>("email",
					    contact_person_field,
					    "email"));
  ret.push_back(new ProxyField<std::string>("phone",
					    contact_person_field,
					    "phone"));

  ret.push_back(new HasManyField("contact_persons", 
				 boost::ref(*ContactPerson::GetModel()),
				 boost::ref(*ContactPerson::GetModel()),
				 _id_column = "company_id",
				 _other_id_column = "id"));


  ret.push_back(new StringField("registered_office"));
  ret.push_back(new StringField("address"));
  ret.push_back(new StringField("bank"));
  ret.push_back(new StringField("personal_account"));
  ret.push_back(new StringField("group", _backend_name="group_"));
  ret.push_back(new StringField("bank_city"));
  ret.push_back(new StringField("correspondent_account"));
  ret.push_back(new StringField("tin"));
  ret.push_back(new StringField("bic"));
  ret.push_back(new StringField("okonh"));
  ret.push_back(new StringField("psrn"));
  ret.push_back(new StringField("note"));
  InitFields(ret);
}

