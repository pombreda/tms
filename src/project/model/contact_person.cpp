
#include "contact_person.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/proxy_field.hpp>
// project
#include <project/model/company.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void ContactPerson::Initialize() {
  vector<Field*> ret;
  ret.push_back(new StringField("name"));
  ret.push_back(new StringField("position"));
  ret.push_back(new StringField("city_code"));
  ret.push_back(new StringField("email"));
  ret.push_back(new StringField("note"));
  ret.push_back(new StringField("phone"));
  ret.push_back(new StringField("fax"));
  IntField *company_id = new SimpleFieldT<int>("company_id");
  ret.push_back(company_id);
  HasOneField *company_field 
    = new HasOneField("company", 
                      boost::ref(*Company::GetModel()),
                      company_id);
  ret.push_back(company_field);
  ret.push_back(new ProxyField<std::string>("company_name",
					    company_field,
					    "short_name"));

  InitFields(ret);
}

